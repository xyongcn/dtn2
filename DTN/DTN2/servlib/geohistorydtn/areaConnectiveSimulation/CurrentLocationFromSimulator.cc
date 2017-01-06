 //说明  : 从模拟器(Realsimulator)获取到所需要的地图分层数据
 // 分层数据时由Areainfo保存，如果发生变化调用GeoHistoryRouter里面的movetoArea即可完成

//#include "CurrentLocationFromSimulator.h"

#ifndef _GEOROUTER_H_
#define _GEOROUTER_H_
#include "servlib/geohistorydtn/routing/GeoHistoryRouter.h"
#endif


namespace dtn{

	void CurrentLocationFromSimulator::run()
	{
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"请求经纬度和分层信息线程开始");
		char recvBuf[MAXLINE+1];
		//byte[] sendBuf = new byte[20];//纬度：经度：
		char buf[MAXLINE];
		//编辑要发送给realsimulator的信息，
		//其中lon,lat,distance都为0，表示请求当前时间的经纬度
		int srcip=IpHelper::ipstr2int("127.0.0.1");
		int logtitude=0;
		int latitude=0;
		int distance=0;
		char srcip_char[5];
		char logtitude_char[5];
		char latitude_char[5];
		char distance_char[5];
		ByteHelper::int_to_byte_array(srcip,srcip_char);
		ByteHelper::int_to_byte_array(logtitude,logtitude_char);
		ByteHelper::int_to_byte_array(latitude,latitude_char);
		ByteHelper::int_to_byte_array(distance,distance_char);

		if(ByteHelper::endian_test())//big endian
		{
			for (int k = 0; k < 4; k++)
			{
				buf[k] = srcip_char[k];
				buf[k+4] = latitude_char[k];
				buf[k+8] = logtitude_char[k];
				buf[k+12] = distance_char[k];
			}
		}
		else
		{    //程序走的是这个分支

			for (int k = 0; k < 4; k++)
			{
				buf[k] = srcip_char[k];
				buf[k+4] = latitude_char[3-k];
				buf[k+8] = logtitude_char[3-k];
				buf[k+12] = distance_char[3-k];
			}
		}
		int num=1;
		while(true)
		{
			if(shutdown_flag)
				break;
			//发送请求当前经纬度到realsimulatior
			sendto(query_loc_socket, buf, MAXLINE, 0, (struct sockaddr *)&(servaddr_query),servlen_query);
			//接收到realsimulatior发送的经纬度
			int n = recvfrom(reply_loc_socket, recvBuf, MAXLINE,0, (struct sockaddr *)&(cliaddr_reply),&(servlen_reply));
			if(n == -1)
			{
				 cout<<"从RealSimulator中接受信息错误,来自于CurrentLocationFromSimulator"<<endl;
				 continue;
			}
			recvBuf[n]=0;
			//处理接收到的数据
			char srcip_r[5];
			char lat_r[5];
			char log_r[5];
			char dis[5];

			if(ByteHelper::endian_test())//big endian
			{
				for (int k = 0; k < 4; k++)
				{
					srcip_r[k] = recvBuf[k];
					lat_r[k] = recvBuf[k + 4];
					log_r[k] = recvBuf[k + 8];
					dis[k] = recvBuf[k +12];
				}
			}
			else
			{    //程序走的是这个分支

				for (int k = 0; k < 4; k++)
				{
					srcip_r[k] = recvBuf[k];
					lat_r[3-k] = recvBuf[k + 4];
					log_r[3-k] = recvBuf[k + 8];
					dis[3-k] = recvBuf[k +12];
				}
			}
			//将字节转为int
			int lat_int = ByteHelper::byte_array_to_int(lat_r);
			int log_int = ByteHelper::byte_array_to_int(log_r);
			int dis_int = ByteHelper::byte_array_to_int(dis);
			double lat_dou =LocationHelper::gpsint2double(lat_int);
			double log_dou =LocationHelper::gpsint2double(log_int);
			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,
					"获取请求到的经纬度信息:%.6lf,%.6lf",log_dou,lat_dou);

			AreaLayerInfo areaLayerInfo=QuestAreaInfo::Getinstance()->getLayerInfo(reply_loc_socket,host.c_str(), log_dou, lat_dou);
			string s=areaLayerInfo.toString();
			if(num==10)
			{
				printf("获取请求到的经纬度信息:%.6lf,%.6lf\n",log_dou,lat_dou);
				cout<<"该经纬度所在的区域:";
				cout<<s<<endl;
				cout<<"______________________________________"<<endl;
				num=0;
			}

			BundleRouter *router=BundleDaemon::GetInstance()->getrouter();
			GeoHistoryRouter *geoHistoryRouter=dynamic_cast<GeoHistoryRouter *>(router);
			if(geoHistoryRouter!=NULL)
			{
				AreaInfo *areaInfo=new AreaInfo(areaLayerInfo);
				geoHistoryRouter->movetoArea(areaInfo);
			}
			++num;
			sleep(3);

		}
	}


CurrentLocationFromSimulator * CurrentLocationFromSimulator::Instance=NULL;
const string CurrentLocationFromSimulator::host="127.0.0.1";
const string CurrentLocationFromSimulator::tag="CurrentLocationFromSimulator";

}

 /*int main()
 {
#include"newFunction/geohistorydtn/areaConnectiveSimulation/CurrentLocationFromSimulator.h"
	 CurrentLocationFromSimulator *location=CurrentLocationFromSimulator::Getinstance();
	 location->start();

	 return 0;
 }*/


