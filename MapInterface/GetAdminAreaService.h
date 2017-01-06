
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
#include<sqlite3.h>
#include<set>
#include<utility>


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

		//////////////////////////////////////////
		//bind 63302
		query_loc_socket2 = socket(AF_INET, SOCK_DGRAM, 0);
		bzero(&servaddr_query2, sizeof(servaddr_query2));
		servaddr_query2.sin_family = AF_INET;
		servaddr_query2.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr_query2.sin_port = htons(QUERY_LOCATION_PORT2);

		if(bind(query_loc_socket2, (struct sockaddr *)&servaddr_query2, sizeof(servaddr_query2)) == -1)
		{
			cout<<"query bind error 63302"<<endl;
		}
		servlen_query2=sizeof(servaddr_query2);

		//init 10007
		bzero(&servaddr_reply2, sizeof(servaddr_reply2));
		servaddr_reply2.sin_family = AF_INET;
		servaddr_reply2.sin_port = htons(REPLY_LOCATION_PORT2);
		if(inet_pton(AF_INET, host.c_str(), &servaddr_reply2.sin_addr) <= 0)
		{
			printf("[%s] is not a valid IPaddress\n",host.c_str() );
		}
		reply_loc_socket2 = socket(AF_INET, SOCK_DGRAM, 0);
		servlen_reply2=sizeof(servaddr_reply2);

		/////////////////////////////////////////



		//bind 63301
		query_loc_socket = socket(AF_INET, SOCK_DGRAM, 0);
		bzero(&servaddr_query, sizeof(servaddr_query));
		servaddr_query.sin_family = AF_INET;
		servaddr_query.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr_query.sin_port = htons(QUERY_LOCATION_PORT);

		if(bind(query_loc_socket, (struct sockaddr *)&servaddr_query, sizeof(servaddr_query)) == -1)
		{
			cout<<"query bind error 63301"<<endl;
		}
				servlen_query=sizeof(servaddr_query);

		//init 10006
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
	BottomtoTopAreaTree areatree;
	hash_map<string,AreaLayerInfo> areaLayerlist;
	MapSqllite *mysql;
	static const string TAG;
	static const string host;
	static const int accuracy=6;

	int query_loc_socket ;
	int reply_loc_socket;

	int query_loc_socket2 ;
	int reply_loc_socket2;

	struct sockaddr_in servaddr_query, cliaddr_query;
	struct sockaddr_in servaddr_reply;

	struct sockaddr_in servaddr_query2, cliaddr_query2;
	struct sockaddr_in servaddr_reply2;

	static const int QUERY_LOCATION_PORT = 63301;
	static const int REPLY_LOCATION_PORT = 10006;

	static const int QUERY_LOCATION_PORT2 = 63302;
	static const int REPLY_LOCATION_PORT2 = 10007;

	socklen_t servlen_query;
	socklen_t servlen_reply;

	socklen_t servlen_query2;
	socklen_t servlen_reply2;
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
		int toparea=mysql->findToparea();
		idRelationlist.push_back(toparea);
		mysql->querySubarea(p, idRelationlist[0], &idRelationlist);
		AreaLayerInfo *areaLayerInfo=new AreaLayerInfo();
		for(int i=0;i<idRelationlist.size();i++){
			areaLayerInfo->arealist.push_back(idRelationlist[i]);
		}
		areaLayerlist[ip]= *areaLayerInfo;


		return areaLayerInfo;
	}

	vector< pair<int,int> > FindAreaOrder(int areaid)
	{
		vector< pair<int,int> > result;
		list<AreaTreeNode *>::iterator it;
		for(it=areatree.buttonarea.begin();it!=areatree.buttonarea.end();++it)
		{
			if(areaid==(*it)->id)
			{
				result.push_back(make_pair(((*it)->id),(*it)->level));
				AreaTreeNode *temp=(*it)->father;
				while(temp)
				{
					result.push_back(make_pair((temp->id),temp->level));
					temp=temp->father;
				}
				return result;
			}
			else
			{
				AreaTreeNode *father=(*it)->father;
				while(father)
				{
					if(father->id==areaid)
						break;
					else
						father=father->father;
				}
				if(father!=NULL)
				{
					result.push_back(make_pair((father->id),father->level));
					AreaTreeNode *temp=father->father;
					while(temp)
					{
						result.push_back(make_pair((temp->id),temp->level));
						temp=temp->father;
					}
					return result;
				}
			}
		}
	}

	void init()
	{
		mysql=MapSqllite::Getinstance();
		pthread_t id;
		pthread_t id2;
		int ret;
		int ret2;
		ret=pthread_create(&id,NULL,GetAdminAreaService::run,this);
		ret2=pthread_create(&id,NULL,GetAdminAreaService::run2,this);
	    pthread_join(id,NULL);
	    pthread_join(id2,NULL);
	}

	static void *run2(void *arg)
	{
		GetAdminAreaService* thread = (GetAdminAreaService*)arg;
		/****获取地图区域的树形结构****/
		thread->mysql->findAreaOrder(&thread->areatree);
		/*************************/
		char recvBuf[MAXLINE];
		//byte[] sendBuf = new byte[20];//纬度：经度：
		char buf[MAXLINE];
		while(true)
		{
			int m=recvfrom(thread->query_loc_socket2, recvBuf, MAXLINE, 0,  (struct sockaddr *)&(thread->cliaddr_query2), &(thread->servlen_query2));
			recvBuf[m]=0;
			char queryArea[5];

			if(ByteHelper::endian_test())//big endian
			{
				for (int k = 0; k < 4; k++)
					queryArea[k] = recvBuf[k];
			}
			else
			{    //程序走的是这个分支
				for (int k = 0; k < 4; k++)
					queryArea[3-k] = recvBuf[k];
			}
			//将字节转为int
			int queryArea_int = ByteHelper::byte_array_to_int(queryArea);
			printf("____________________________________\n");
			cout<<"接收到区域id:"<<queryArea_int<<endl;
			vector< pair<int,int> > r;
			r=thread->FindAreaOrder(queryArea_int);
			char size[5];
			ByteHelper::int_to_byte_array(r.size(),size);
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
			cout<<"它的父区域为:";
			for(int i=0;i<r.size();i++)
			{
				char info_char[5];
				cout<<r[i].first<<" ";
				ByteHelper::int_to_byte_array(r[i].first,info_char);
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
			cout<<endl<<"____________________________________"<<endl;
			int n=sendto(thread->reply_loc_socket2, buf, MAXLINE, 0, (struct sockaddr *)&(thread->servaddr_reply2),thread->servlen_reply2);
			if(n == -1)
			{
				cout<<"send error"<<endl;
				 continue;
			}
		}
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


