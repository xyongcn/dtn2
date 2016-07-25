
/**
 * 位置交互接口
 * 接收位置请求，反馈位置信息
 * 经纬度的表示为xxx.xxxxxx，精确到小数点后6位
 * 表示
 * 在 启动
 */
#include<iostream>
#include<stdio.h>
#include <pthread.h>
#include <sys/types.h>
 #include <sys/socket.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
#include"rsLocation.h"
#include"ByteHelper.h"
#include<math.h>
#include"Distance.h"
 #include <arpa/inet.h>

using namespace std;
#define MAXLINE 256

class InteractorThread
{
private:
	static InteractorThread *Instance;

	InteractorThread()
	{

		//bind 10003
				query_loc_socket = socket(AF_INET, SOCK_DGRAM, 0);
				bzero(&servaddr_query, sizeof(servaddr_query));
				servaddr_query.sin_family = AF_INET;
				servaddr_query.sin_addr.s_addr = htonl(INADDR_ANY);
				servaddr_query.sin_port = htons(QUERY_LOCATION_PORT);

				if(bind(query_loc_socket, (struct sockaddr *)&servaddr_query, sizeof(servaddr_query)) == -1)
				{
					cout<<"query bind error";
				}
				servlen_query=sizeof(servaddr_query);

				//init 10004
				bzero(&servaddr_reply, sizeof(servaddr_reply));
				servaddr_reply.sin_family = AF_INET;
				servaddr_reply.sin_port = htons(REPLY_LOCATION_PORT);
				if(inet_pton(AF_INET, host.c_str(), &servaddr_reply.sin_addr) <= 0)
				{
					printf("[%s] is not a valid IPaddress\n",host.c_str() );
				 }
				reply_loc_socket = socket(AF_INET, SOCK_DGRAM, 0);
				servlen_reply=sizeof(servaddr_reply);
			/*	if(connect(reply_loc_socket, (struct sockaddr *)&servaddr_reply, servlen_reply) == -1)
				{
					cout<<"connect error"<<endl;
					exit(1);
				 }*/
	}

public:
	int query_loc_socket ;
	int reply_loc_socket;
	struct sockaddr_in servaddr_query, cliaddr_query;
	struct sockaddr_in servaddr_reply;

	static const int QUERY_LOCATION_PORT = 10003;
	static const int REPLY_LOCATION_PORT = 10004;
	static const int accuracy = 6;
	static const string host;
	socklen_t servlen_query;
	socklen_t servlen_reply;
	static InteractorThread* Getinstance()
	{
	        if(Instance == NULL)  //判断是否第一次调用
	        	Instance = new InteractorThread;


	        return Instance ;
	}

	void init()
	{
		pthread_t id;
		int ret;
		ret=pthread_create(&id,NULL,InteractorThread::run,this);
	    pthread_join(id,NULL);
	}

	static void *run(void *arg)
	{

		InteractorThread* this_thread = (InteractorThread*)arg;
		char recvBuf[MAXLINE];
		//byte[] sendBuf = new byte[20];//纬度：经度：
		char buf[MAXLINE];


		while(true)
		{

			int m=recvfrom(this_thread->query_loc_socket, recvBuf, MAXLINE, 0,  (struct sockaddr *)&(this_thread->cliaddr_query), &(this_thread->servlen_query));
			recvBuf[m]=0;

			//请求当前结点当前时间的经纬度
			rsLocation *rsLoc =rsLocation::GetInstance();
			Location location = rsLoc->getRsLocation();
			double latitude = location.getLatitude();
			double longitude = location.getLongitude();
			cout<<"当前结点当前时间经纬度："<<"latitude:"<<latitude<<"  ";
			printf("longitude:%.6lf\n",longitude);

			//处理接收到的请求，该请求包分别为scrip,lon,lat,distance，
			//若lon,lat为0，说明只是请求当前结点位置，
			//若lon,lat不为0，说明请求的是当前结点和邻居结点的距离
			char srcip[5];
			char neigh_lat[5];
			char neigh_lng[5];
			char dis[5];

			if(ByteHelper::endian_test())//big endian
			{
				for (int k = 0; k < 4; k++)
				{
					srcip[k] = recvBuf[k];
					neigh_lat[k] = recvBuf[k + 4];
					neigh_lng[k] = recvBuf[k + 8];
					dis[k] = recvBuf[k +12];
				}
			}
			else
			{    //程序走的是这个分支

				for (int k = 0; k < 4; k++)
				{
					srcip[k] = recvBuf[k];
					neigh_lat[3-k] = recvBuf[k + 4];
					neigh_lng[3-k] = recvBuf[k + 8];
					dis[3-k] = recvBuf[k +12];
				}
			}
			//将字节转为int
			int neigh_lat_int = ByteHelper::byte_array_to_int(neigh_lat);
			int neigh_lng_int = ByteHelper::byte_array_to_int(neigh_lng);
			if( (neigh_lat_int == 0) && (neigh_lng_int==0) )//节点位置请求
			{
				cout<<"LOCAL LOCATION QUERY"<<endl;
			}
			else
			{
				double neigh_lat_double = neigh_lat_int * 1.0 / pow(10, accuracy);
				double neigh_lng_double = neigh_lng_int * 1.0 / pow(10, accuracy);
				cout<<neigh_lat_int<<"  "<<neigh_lng_int<<endl;
				double distance = Distance::getDistance(latitude, longitude, neigh_lat_double, neigh_lng_double);
				ByteHelper::int_to_byte_array((int)distance,dis);

				printf("distance:%.9lf\n",distance);

				if(Distance::canConnected(distance))//距离为m，转为int型，再转byte存入dis
				{

					cout<<"Can communicate with that one!"<<endl;
				}
				else//debug
				{
					cout<<"Cannot communicate with that one!"<<endl;
				}
			}
			//将经纬度转换成int类型便于在上下层交换,纬度为y，经度为x
			int x = (int)(longitude * pow(10, accuracy));
			int y = (int)(latitude * pow(10, accuracy));
			//debug
			string msg;
			char temp[20];
			sprintf(temp,"%d",x);
			msg.append(temp);
			msg.append("	");
			sprintf(temp,"%d",y);
			msg.append(temp);
			//Log.i("int经纬度", msg);
			//将int型的数据转换成byte型数组
			char xb[5];
			ByteHelper::int_to_byte_array(x,xb);
			char yb[5];
			ByteHelper::int_to_byte_array(y,yb);
			//将接收到的数据复制到待发送数据区buf
			strncpy(buf,recvBuf,MAXLINE);
			if(ByteHelper::endian_test())
			{
				for(int i = 0; i < 4; i++)
				{
					buf[i+4] = yb[i];//纬度
					buf[i+8] = xb[i];//经度
					buf[i+12] = dis[i];//距离

				}
			}
			else
			{
				for(int i = 0; i < 4; i++)
				{
					buf[i+4] = yb[3-i];//纬度
					buf[i+8] = xb[3-i];//经度
					buf[i+12] = dis[3-i];//距离

				}
			}

			int n=sendto(this_thread->reply_loc_socket, buf, MAXLINE, 0, (struct sockaddr *)&(this_thread->servaddr_reply),this_thread->servlen_reply);
			if(n == -1)
			{
				 cout<<"read error"<<endl;
				 continue;
			}
			cout<<"_______________________________________________________"<<endl;
		//	sleep(1000);

		}
	}
};

InteractorThread * InteractorThread::Instance=NULL;
const string InteractorThread::host="127.0.0.1";



