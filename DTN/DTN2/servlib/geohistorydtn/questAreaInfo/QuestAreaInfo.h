 #include <sys/types.h>
 #include <sys/socket.h>
 #include <string.h>
 #include <netinet/in.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <iostream>
 #include <string>
#include <pthread.h>

#include "AreaLayerInfo.h"
#include "servlib/geohistorydtn/area/AreaInfo.h"
#include "servlib/geohistorydtn/Log/GeohistoryLog.h"
#include "geohistorydtn/areaConnectiveSimulation/LocationHelper.h"
#include "geohistorydtn/areaConnectiveSimulation/IpHelper.h"
#include "geohistorydtn/areaConnectiveSimulation/ByteHelper.h"



#define MAXLINE 256
using namespace std;

namespace dtn{
class QuestAreaInfo
{
	private:
	static const string tag;
	static QuestAreaInfo *Instance;

	QuestAreaInfo()
	{
		geohistoryLog=GeohistoryLog::GetInstance();
		// init 63301
		bzero(&servaddr_query, sizeof(servaddr_query));
		servaddr_query.sin_family = AF_INET;
		servaddr_query.sin_port = htons(QUERY_LOCATION_PORT);
		if(inet_pton(AF_INET, host.c_str(), &servaddr_query.sin_addr) <= 0)
		{
			printf("[%s] is not a valid IPaddress\n",host.c_str());
		}
		servlen_query=sizeof(servaddr_query);
		query_loc_socket = socket(AF_INET, SOCK_DGRAM, 0);
	/*	if(connect(query_loc_socket, (struct sockaddr *)&servaddr_query, servlen_query) == -1)
		{
			cout<<"connect error"<<endl;
			exit(1);
		 }*/

		//bind 10004
	/*	bzero(&servaddr_reply, sizeof(servaddr_reply));
		servaddr_reply.sin_family = AF_INET;
		servaddr_reply.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr_reply.sin_port = htons(REPLY_LOCATION_PORT);
		reply_loc_socket = socket(AF_INET, SOCK_DGRAM, 0);
		if(bind(reply_loc_socket, (struct sockaddr *)&servaddr_reply, sizeof(servaddr_reply)) == -1)
		{
			cout<<"reply bind error";
		}
		servlen_reply=sizeof(servaddr_reply);*/

	}

	public:
		static const int QUERY_LOCATION_PORT = 63301;
		static const int REPLY_LOCATION_PORT = 10004;
		struct sockaddr_in servaddr_query;
		struct sockaddr_in servaddr_reply;
		struct sockaddr_in cliaddr_reply;
		int query_loc_socket ;
		//int reply_loc_socket;
		socklen_t servlen_query;
		socklen_t servlen_reply;
		static const string host;
		GeohistoryLog *geohistoryLog;

		static QuestAreaInfo* Getinstance()
		{
		        if(Instance == NULL)  //判断是否第一次调用
		        	Instance = new QuestAreaInfo;


		        return Instance ;
		}
	/*	void init()
		{
			pthread_t id;
			int ret;
			ret=pthread_create(&id,NULL,QuestAreaInfo::run,this);
		    pthread_join(id,NULL);
		}*/
		/**
		 * 请求该(ip,经度,维度)下，经纬度对应的分层信息
		 * @param ip : 用int类型表示的ip地址
		 * @param longitude
		 * @param latitude
		 * @return 返回的分层信息是从上层到底层的形式，最顶层的区域id在第一位
		 */
		AreaLayerInfo getLayerInfo(int reply_loc_socket,string ip,double longitude,double latitude)
		{
		//	QuestAreaInfo* this_thread = (QuestAreaInfo*)arg;
			char recvBuf[MAXLINE+1];
			char buf[MAXLINE];
			int srcip=IpHelper::ipstr2int(ip);
			int longtitude_int=int(longitude*1000000);
			int latitude_int=int(latitude*1000000);
			char srcip_char[5];
			char logtitude_char[5];
			char latitude_char[5];
			ByteHelper::int_to_byte_array(srcip,srcip_char);
			ByteHelper::int_to_byte_array(longtitude_int,logtitude_char);
			ByteHelper::int_to_byte_array(latitude_int,latitude_char);

			if(ByteHelper::endian_test())//big endian
			{
				for (int k = 0; k < 4; k++)
				{
					buf[k] = srcip_char[k];
					buf[k+4] = latitude_char[k];
					buf[k+8] = logtitude_char[k];
				}
			}
			else
			{    //程序走的是这个分支

				for (int k = 0; k < 4; k++)
				{
					buf[k] = srcip_char[k];
					buf[k+4] = latitude_char[3-k];
					buf[k+8] = logtitude_char[3-k];
				}
			}
			//int num=1;
			//while(true)
			//{
				sendto(query_loc_socket, buf, MAXLINE, 0, (struct sockaddr *)&(servaddr_query),servlen_query);
				//接收到MapInterface该经纬度所在的各层区域
				int n = recvfrom(reply_loc_socket, recvBuf, MAXLINE,0, (struct sockaddr *)&(cliaddr_reply),&(servlen_reply));
				if(n == -1)
				{
					 cout<<"从MapInterface中接受该经纬度所在的各层区域信息错误,来自于QuestAreaInfo"<<endl;
					 AreaLayerInfo info;
					 return info;
				}
				recvBuf[n]=0;
				//处理接收到的数据
				char srcip_r[5];
				char size_r[5];
				int size_int;
				char *area;

				if(ByteHelper::endian_test())//big endian
				{
					for (int k = 0; k < 4; k++)
					{
						srcip_r[k] = recvBuf[k];
						size_r[k] = recvBuf[k + 4];
					}
					size_int=ByteHelper::byte_array_to_int(size_r);
					area=new char[size_int*4];
					for(int j=0;j<size_int;++j)
					{
						for(int k=0;k<4;++k)
							area[j*4+k]=recvBuf[k+8+j*4];
					}
				}
				else
				{    //程序走的是这个分支
					for (int k = 0; k < 4; k++)
					{
						srcip_r[k] = recvBuf[k];
						size_r[3-k] = recvBuf[k + 4];
					}
					size_int=ByteHelper::byte_array_to_int(size_r);
					area=new char[size_int*4];
					for(int j=0;j<size_int;++j)
					{
						for(int k=0;k<4;++k)
						{
							//cout<<j*4+3-k<<"  "<<k+8+j*4<<endl;
							area[j*4+3-k]=recvBuf[k+8+j*4];
						}

					}

				}
				//将字节转为int
				int area_int;
				char area_part[4];
				AreaLayerInfo info;
				for(int j=0;j<size_int;++j)
				{
					for(int k=0;k<4;++k)
						area_part[k]=area[j*4+k];
					area_int=ByteHelper::byte_array_to_int(area_part);
					info.arealist.push_back(area_int);
				}

				string s=info.toString();
				geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,
						"分层信息(由最顶层到最底层): %s",s.c_str());
				return info;
			//	sleep(5);

			//}
		}

	};

//QuestAreaInfo * QuestAreaInfo::Instance=NULL;
//const string QuestAreaInfo::tag="QuestAreaInfo";
//const string QuestAreaInfo::host="127.0.0.1";


}

/*	 int main()
	 {

		 QuestAreaInfo *c=QuestAreaInfo::Getinstance();
		 c->init();

		 return 0;
	 }*/
