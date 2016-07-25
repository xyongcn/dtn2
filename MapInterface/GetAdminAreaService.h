
/**
 * 区域交互接口
 * 接收请求，反馈区域信息
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
#include<math.h>
 #include <arpa/inet.h>
#include"ByteHelper.h"
#include <hash_map>
#include"MapSqllite.h"
#include"AreaLayerInfo.h"

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
    template<> struct hash<const string>
    {
        size_t operator()(const string& s) const
        { return hash<const char*>()( s.c_str() ); }
    };
    template<> struct hash<string>
    {
        size_t operator()(const string& s) const
        { return hash<const char*>()( s.c_str() ); }
    };
}

#define MAXLINE 256

class GetAdminAreaService
{
private:
	static GetAdminAreaService *Instance;

	GetAdminAreaService()
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

	hash_map<string,AreaLayerInfo> areaLayerlist;
	MapSqllite *mysql;
	static const string TAG;
	static const string host;
	static const int accuracy=6;

	int query_loc_socket ;
	int reply_loc_socket;
	struct sockaddr_in servaddr_query, cliaddr_query;
	struct sockaddr_in servaddr_reply;

	static const int QUERY_LOCATION_PORT = 63301;
	static const int REPLY_LOCATION_PORT = 10004;

	socklen_t servlen_query;
	socklen_t servlen_reply;
	static GetAdminAreaService* Getinstance()
	{
	        if(Instance == NULL)  //判断是否第一次调用
	        	Instance = new GetAdminAreaService();


	        return Instance ;
	}

	AreaLayerInfo *getLayerInfoByGps(string ip,double longitude,double latitude)
	{
		hash_map<string,AreaLayerInfo>::iterator it=areaLayerlist.find(ip);
		Point p(longitude, latitude);
		if(it!=areaLayerlist.end())
		{
			AreaLayerInfo *layerInfo=&(it->second);
			int layerid=layerInfo->arealist[layerInfo->arealist.size()-1];
			if(mysql->isPointInPolygon(p, layerid)==1)
			{
				cout<<TAG<<"所在的位置和上次所在的位置一样"<<endl;
				return layerInfo;
			}
			else
			{
				//和上次最底层区域不一样，由于只有三层，所以不再递归找到同样的一层了，直接重新计算
			}
		}
		vector<int> idRelationlist;
		idRelationlist.push_back(15711);
		mysql->querySubarea(p, idRelationlist[0], &idRelationlist);
		AreaLayerInfo *areaLayerInfo=new AreaLayerInfo();
		for(int i=0;i<idRelationlist.size();i++){
			areaLayerInfo->arealist.push_back(idRelationlist[i]);
		}
		areaLayerlist[ip]= *areaLayerInfo;


		return areaLayerInfo;
	}

	void init()
	{
		mysql=MapSqllite::Getinstance();
		pthread_t id;
		int ret;
		ret=pthread_create(&id,NULL,GetAdminAreaService::run,this);
	    pthread_join(id,NULL);
	}

	static void *run(void *arg)
	{

		GetAdminAreaService* this_thread = (GetAdminAreaService*)arg;
		char recvBuf[MAXLINE];
		//byte[] sendBuf = new byte[20];//纬度：经度：
		char buf[MAXLINE];


		while(true)
		{

			int m=recvfrom(this_thread->query_loc_socket, recvBuf, MAXLINE, 0,  (struct sockaddr *)&(this_thread->cliaddr_query), &(this_thread->servlen_query));
			recvBuf[m]=0;

			char ip_s[5];
			char lat[5];
			char lng[5];

			if(ByteHelper::endian_test())//big endian
			{
				for (int k = 0; k < 4; k++)
				{
					ip_s[k] = recvBuf[k];
					lat[k] = recvBuf[k + 4];
					lng[k] = recvBuf[k + 8];
				}
			}
			else
			{    //程序走的是这个分支

				for (int k = 0; k < 4; k++)
				{
					ip_s[k] = recvBuf[k];
					lat[3-k] = recvBuf[k + 4];
					lng[3-k] = recvBuf[k + 8];
				}
			}
			//将字节转为int
			int lat_int = ByteHelper::byte_array_to_int(lat);
			int lng_int = ByteHelper::byte_array_to_int(lng);
			int ip_int=ByteHelper::byte_array_to_int(ip_s);
			string ip= ByteHelper::int2ipstr(ip_int);

			double latitude = lat_int * 1.0 / pow(10, accuracy);
			double longitude = lng_int * 1.0 / pow(10, accuracy);
			if(longitude<-180 || longitude >180 || latitude<-90 || latitude>90)
			{
				cout<<"非法的经纬度"<<endl;
			}
			AreaLayerInfo *info;
			info=this_thread->getLayerInfoByGps(ip, longitude, latitude);
			for(vector<int>::iterator i=info->arealist.begin();i!=info->arealist.end();++i)
				cout<<*i<<"  ";
			cout<<endl;
			if(info!=NULL)
			{
				strcpy(buf,recvBuf);
				char size[5];
				ByteHelper::int_to_byte_array(info->arealist.size(),size);
				if(ByteHelper::endian_test())
				{
					for(int i = 0; i < 4; i++)
						buf[i+4] = size[i];
				}
				else
				{
					for(int i = 0; i < 4; i++)
						buf[i+4] = size[3-i];

				}
				for(int i=0;i<info->arealist.size();i++)
				{
					char info_char[5];
					ByteHelper::int_to_byte_array(info->arealist[i],info_char);
					if(ByteHelper::endian_test())
					{
						for(int k = 0; k < 4; k++)
							buf[k+4*(i+2)] = info_char[k];
					}
					else
					{
						for(int k = 0; k < 4; k++)
							buf[k+4*(i+2)] = info_char[3-k];
					}
				}
				int n=sendto(this_thread->reply_loc_socket, buf, MAXLINE, 0, (struct sockaddr *)&(this_thread->servaddr_reply),this_thread->servlen_reply);
				if(n == -1)
				{
					 cout<<"send error"<<endl;
						 continue;
				}

			}
		}
	}


};

GetAdminAreaService * GetAdminAreaService::Instance=NULL;
const string GetAdminAreaService::TAG="GetAdminAreaService";
const string GetAdminAreaService::host="127.0.0.1";


