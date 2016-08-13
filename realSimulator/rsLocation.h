
/**
 *
 *
 * 为不同模式（GPS/配置文件）的定位方式提供统一接口
 *
 */

#include<iostream>
#include"Location.h"
#include"ConfigLocation.h"

using namespace std;
 class rsLocation {

 private :
		 static rsLocation *Instance;
	 	 static const string TAG;
		 rsLocation()
		 {}

		 rsLocation(double latitude,double longitude)
		{
			this->rsLoc.setLatitude(latitude);
			this->rsLoc.setLongitude(longitude);
		}

 public:
		 Location rsLoc;





		    static rsLocation* GetInstance()
		    {
		        if(Instance == NULL)  //判断是否第一次调用
		        	Instance = new rsLocation();


		        return Instance ;
		    }



		Location getRsLocation()
		{
			//根据模式选择获取位置的方式
			//rsLoc = NULL;
			NodeInfo *node= NodeInfo::GetInstance();
			switch(node->Mode)
			{
			case NodeInfo::Config_Mode:
				rsLoc = this->getConfigLocation();
				break;
			case NodeInfo::GPS_Mode:
				rsLoc = this->getGpsLocation();
				break;
			default :
				cout<<"Error Mode!"<<endl;
			}
			return rsLoc;
		}

		Location getGpsLocation()
		{

		}

		 Location getConfigLocation()
		{
			//启动时记得先获取第一个位置
			Location *location = new Location();
			ConfigLocation *configLocation = ConfigLocation::GetInstance();
			ParseConfigFile *pcf = configLocation->getConfigLocation();
			//测试一下
			location->setLatitude(pcf->latitude);
			location->setLongitude(pcf->longitude);
			return *location;
		}



};
 rsLocation * rsLocation::Instance=NULL;
 const string rsLocation::TAG = "rsLocation";
