 //说明  : 从模拟器(Realsimulator)获取到所需要的地图分层数据
 // 分层数据时由Areainfo保存，如果发生变化调用GeoHistoryRouter里面的movetoArea即可完成



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
#include <oasys/thread/Thread.h>

#include "servlib/geohistorydtn/questAreaInfo/QuestAreaInfo.h"
#include "servlib/bundling/BundleDaemon.h"

//#ifndef _GEOROUTER_H_
//#define _GEOROUTER_H_

//#endif*/


class BundleDaemon;



using namespace std;
using namespace oasys;

#define MAXLINE 256

namespace dtn{

class CurrentLocationFromSimulator:public Thread
{
private:
	static const string tag;
	static CurrentLocationFromSimulator *Instance;
	CurrentLocationFromSimulator()
	:Thread("CurrentLocation", CREATE_JOINABLE)
	{
			shutdown_flag=false;
		// init 10005
			geohistoryLog=GeohistoryLog::GetInstance();
			bzero(&servaddr_query, sizeof(servaddr_query));
			servaddr_query.sin_family = AF_INET;
			servaddr_query.sin_port = htons(QUERY_LOCATION_PORT);
			if(inet_pton(AF_INET, host.c_str(), &servaddr_query.sin_addr) <= 0)
			{
				printf("[%s] is not a valid IPaddress\n",host.c_str() );
			 }
			servlen_query=sizeof(servaddr_query);
			query_loc_socket = socket(AF_INET, SOCK_DGRAM, 0);
		/*	if(connect(query_loc_socket, (struct sockaddr *)&servaddr_query, servlen_query) == -1)
			{
				cout<<"connect error"<<endl;
				exit(1);
			 }*/

			//bind 10006
			bzero(&servaddr_reply, sizeof(servaddr_reply));
			servaddr_reply.sin_family = AF_INET;
			servaddr_reply.sin_addr.s_addr = htonl(INADDR_ANY);
			servaddr_reply.sin_port = htons(REPLY_LOCATION_PORT);
			reply_loc_socket = socket(AF_INET, SOCK_DGRAM, 0);
			if(bind(reply_loc_socket, (struct sockaddr *)&servaddr_reply, sizeof(servaddr_reply)) == -1)
			{
				cout<<"reply bind error";
				geohistoryLog->LogAppend(geohistoryLog->ERROR_LEVEL,"无法建立请求分层信息的server,绑定失败");
			}
			servlen_reply=sizeof(servaddr_reply);

	}

public:
	static const int QUERY_LOCATION_PORT = 10005;//Realsimulator绑定
	static const int REPLY_LOCATION_PORT = 10006;//该程序绑定
	struct sockaddr_in servaddr_query;
	struct sockaddr_in servaddr_reply;
	struct sockaddr_in cliaddr_reply;
	int query_loc_socket ;
	int reply_loc_socket;
	socklen_t servlen_query;
	socklen_t servlen_reply;
	static const string host;
	bool shutdown_flag;
	pthread_t id;
	GeohistoryLog *geohistoryLog;

	static CurrentLocationFromSimulator* Getinstance()
	{
	        if(Instance == NULL)  //判断是否第一次调用
	        	Instance = new CurrentLocationFromSimulator;
	        return Instance ;
	}
	/*void init()
	{
		shutdown_flag=false;

		int ret;
		ret=pthread_create(&id,NULL,CurrentLocationFromSimulator::run,this);

	}*/

	 void shutdown()
	 {
		 shutdown_flag=true;
	 }

	void run();


};
//CurrentLocationFromSimulator * CurrentLocationFromSimulator::Instance=NULL;
//const string CurrentLocationFromSimulator::host="127.0.0.1";
//const string CurrentLocationFromSimulator::tag="CurrentLocationFromSimulator";

}

 /*int main()
 {
#include"newFunction/geohistorydtn/areaConnectiveSimulation/CurrentLocationFromSimulator.h"
	 CurrentLocationFromSimulator *location=CurrentLocationFromSimulator::Getinstance();
	 location->start();

	 return 0;
 }*/


