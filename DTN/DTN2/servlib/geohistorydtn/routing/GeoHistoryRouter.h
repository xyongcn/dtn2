#define BERKELEY_DB_VERSION 5.3

#define GOOGLE_PROFILE_ENABLED 0

#define HAVE_CFMAKERAW 1

#define HAVE_CFSETSPEED 1

#define HAVE_ERR_H 1

#define HAVE_EXECINFO_H 1

#define HAVE_GETADDRINFO 1

#define HAVE_GETHOSTBYNAME 1

#define HAVE_GETHOSTBYNAME_R 1

#define HAVE_GETOPT_LONG 1

#define HAVE_INET_ATON 1

#define HAVE_INET_PTON 1

#define HAVE_INTTYPES_H 1

#define HAVE_MEMORY_H 1

#define HAVE_NETAX25_AX25_H 1

#define HAVE_PTHREAD_SETSPECIFIC 1

#define HAVE_PTHREAD_YIELD 1

#define HAVE_PTRDIFF_T 1

#define HAVE_SCHED_YIELD 1

#define HAVE_STDINT_H 1

#define HAVE_STDLIB_H 1

#define HAVE_STRINGS_H 1

#define HAVE_STRING_H 1

#define HAVE_SYS_CDEFS_H 1

#define HAVE_SYS_STAT_H 1

#define HAVE_SYS_TYPES_H 1

#define HAVE_UINT32_T 1

#define HAVE_UNISTD_H 1

#define HAVE_U_INT32_T 1

#define HAVE_XDR_U_QUAD_T 1

#define LIBDB_ENABLED 1

#define LIBEXPAT_ENABLED 1

#define LIBODBC_ENABLED 1

#define OASYS_CONFIG_STATE 1

#define OASYS_DEBUG_LOCKING_ENABLED 1

#define OASYS_ZLIB_ENABLED 1

#define OASYS_ZLIB_HAS_COMPRESS_BOUND 1

#define PACKAGE_BUGREPORT ""

#define PACKAGE_NAME ""

#define PACKAGE_STRING ""

#define PACKAGE_TARNAME ""

#define PACKAGE_URL ""

#define PACKAGE_VERSION ""

#define RETSIGTYPE void

#define SIZEOF_OFF_T 8

#define STDC_HEADERS 1

#define TCLREADLINE_ENABLED 0

#define XERCES_C_ENABLED 1

#ifndef __cplusplus
/* #undef inline */
#endif


#include "servlib/routing/TableBasedRouter.h"

#include<string>
#include "servlib/geohistorydtn/routing/BlockingQueue.h"

/*#ifndef NEIGHBOURMANAGER_H_
#define NEIGHBOURMANAGER_H_
#include"servlib/geohistorydtn/neighbour/NeighbourManager.h"
#endif*/
#include "servlib/geohistorydtn/routing/ChanceValueSort.h"
#include "servlib/geohistorydtn/config/BundleConfig.h"


#include "applib/APIServer.h"
//#include <oasys/thread/Thread.h>
//#include "geohistorydtn/questAreaInfo/QuestAreaInfo.h"

//#ifndef	AREA_H_
//#define AREA_H_
//#include "geohistorydtn/area/Area.h"
//#endif

using namespace std;
using namespace oasys;




namespace dtn {
typedef Area* Arearef;
typedef RouteEntry* RouteEntryref;
//class TableBasedRouter;
class GeoHistoryRouter : public TableBasedRouter//,public Thread

{

protected:
	int route_bundle(Bundle* bundle);


private:
	class RouteAllBundleMsg:public Object_RouteMessage
	{
		public: RouteAllBundleMsg(){}
	};
	 RouteAllBundleMsg *routeAllBundle;
	 map<string,int> Forward2PayloadNumMap;
	 map<string,int> Forward1PayloadNumMap;
	 multimap<u_int32_t,string> bundleRecord;
	 static string BundleLogPath;
	 static string DataBundleLogPath;

	 pthread_mutex_t lockBundleLog;
	 pthread_mutex_t lockDataBundleLog;

	void lock_BundleLog()
	{
		pthread_mutex_lock(&lockBundleLog);
	}

	void unlock_BundleLog()
	{
		pthread_mutex_unlock(&lockBundleLog);
	}

	void lock_DataBundleLog()
	{
		pthread_mutex_lock(&lockDataBundleLog);
	}

	void unlock_DataBundleLog()
	{
		pthread_mutex_unlock(&lockDataBundleLog);
	}

	/**
	 * 路由的方式
	 */
	 struct RouteType
	 {
		static const int Flooding=-1;
		static const int Neighbour=-2;
	 };

	 void handle_routeAllBundle();
	 int route_neighbourArea_bundle(Bundle *bundle);
	 bool canDirectDelivery(Bundle *bundle);
	int get_matching_RouteEntryVec(Bundle *bundle,RouteEntryVec &entry_vec,
					int sameAreaLevel,map<Arearef,RouteEntryref> &sameLevelAreaMap);
	static string StringOfBundle(Bundle *bundle);
public:


	//pthread_mutex_t areaInfoQueue_Mutex;
	//queue<AreaInfo> areaInfoQueue;
	BlockingQueue <Object_RouteMessage*> areaInfoQueue;
	GeohistoryLog *geohistoryLog;
	//APIServer *api;
	GeoHistoryRouter();

	virtual ~GeoHistoryRouter(){}

	/////////////////////////处理link//////////

private:
	class SendBundleMsg:public Object_RouteMessage
	{
	public:
		string dest_eid;
		string fileroute;
		bool rctp;
		vector<int> areaid;
		int bundleType;

		SendBundleMsg(string dest_eid,string fileroute,bool rctp,vector<int> areaid,int bundleType)
		{
			this->dest_eid=dest_eid;
			this->fileroute=fileroute;
			this->rctp=rctp;
			this->areaid=areaid;
			this->bundleType=bundleType;
		}
	};
protected:
	void handle_link_created(LinkCreatedEvent *event);



	void handle_contact_up(ContactUpEvent* event);
	void handle_contact_down(ContactDownEvent* event);
	//针对收到要发送的bundle，需要转发的bundle，邻居交互信息的bundle的处理
	void handle_bundle_received(BundleReceivedEvent* event);

	///////////////////////////处理区域问题
public:
	Area *baseArea;
	AreaManager *areamanager;

	void movetoArea(AreaInfo *areaInfo)
	{
		areaInfoQueue.push(areaInfo);

	}

	bool isChangedArea(AreaInfo areainfo)
	{
		if(baseArea==NULL)
			return true;

		if(baseArea->getAreaId()==areainfo.getBaseAreaId())
		{
			return false;
		}
		else
			return true;
	}
	/**
	 * 作用：当该节点所在区域变动时，改变它相应的频率向量的时间计时
	 * @param oldarea：旧的底层区域
	 * @param newarea：新的底层区域
	 */
	void changeAreaTimeCount(Area *oldarea,Area *newarea)
	{
		//递归的结束条件
		if(newarea==NULL)
			return;

		//旧区域为空的变更计时器
		if(oldarea==NULL)
		{
		//	Log.d(tag,String.format("首次加载新区域的频率香料到计时器中（新区域：%d#%d）"
			//		,newarea.getAreaLevel(),newarea.getAreaId()));
			newarea->addTimeCount();
			changeAreaTimeCount(NULL,newarea->getFatherArea());

			//Log.d(tag,"进入新区域的频率向量:"+newarea.toString());
			return ;
		}

		/**
		 * 正常情况的变更计时器
		 */
		if(!oldarea->equals(newarea) && oldarea->getAreaLevel()==newarea->getAreaLevel())
		{
			//Log.d(tag,String.format("变动计时器中新旧区域的频率向量（旧区域：%d#%d 新区域：%d#%d）"
			//	, oldarea.getAreaLevel(),oldarea.getAreaId(),newarea.getAreaLevel(),newarea.getAreaId()));
			oldarea->removeTimeCount();
			newarea->addTimeCount();
			changeAreaTimeCount(oldarea->getFatherArea(),newarea->getFatherArea());

			//Log.d(tag,"原先区域的频率向量:"+oldarea.toString());
		//	Log.d(tag,"当前区域的频率向量:"+newarea.toString());
		}
		else
		{
			//Log.d(tag,String.format("新旧区域为同一层的同一个区域，变动相应的区域频率向量计时停止（旧区域：%d#%d 新区域：%d#%d）"
			//	, oldarea.getAreaLevel(),oldarea.getAreaId(),newarea.getAreaLevel(),newarea.getAreaId()));
		}
	}


	void handle_areamoving(AreaInfo *aInfo)
	{
		AreaInfo areaInfo=*aInfo;
		delete aInfo;
		if(isChangedArea(areaInfo))
		{
			//生成对应的Area
			Area *newBaseArea=areamanager->checkAreaInfo(areaInfo);
			if(newBaseArea!=NULL)
			{
				//变动相应的频率向量,因为每次计时到了就开始改变FVector，所以在变更区域的时候不需要进行变动频率向量
				newBaseArea->changeFVector(areaInfo);

				//加入到新的倒数时间列表里面,将原来的从倒数计时里面去掉
				changeAreaTimeCount(baseArea, newBaseArea);

				//更改当前的底层区域向量
				baseArea=newBaseArea;

				//将移动的记录保存到日志中
				//即新的各层区域的id ，level 和各个频率向量
				string s="move to new area";
				areamanager->writeAreaLogToFile(s,baseArea,areaInfo);
			}
		}
	}

	void writeDeliveryBundleLogToFile(Bundle *bundle)
	{
		lock_BundleLog();
		FILE * fr;
		errno=0;
		fr= fopen(BundleLogPath.c_str(),"a");
		if (NULL == fr)
		{
			if (EINVAL == errno)
			{
				printf("err:fopen log file %s failed\n",BundleLogPath.c_str());
			}
			else
			{
				printf("err:unknow\n");
			}
		}
		string temp;
		string father;

		tm currentTime=CurrentTimeManager::Getinstance()->currentTime;
		if(bundle->getBundleType()!=Bundle::ORIGINAL_BUNDLE &&
			bundle->dest().str()==BundleDaemon::GetInstance()->local_eid().str())
		{
			if(bundle->dest().str()==BundleDaemon::GetInstance()->local_eid().str())
				temp.append("delivery a new bundle:\n");
			char time_c[10];
			sprintf(time_c,"%d",currentTime.tm_mon+1);
			temp.append(time_c);
			//temp.append("月,");
			temp.append("month,");
			if(currentTime.tm_wday==0)
				//temp.append("星期天,");
				temp.append("Sunday,");
			else
			{
				sprintf(time_c,"%d",currentTime.tm_wday);
				//temp.append("星期");
				temp.append("week ");
				temp.append(time_c);
				temp.append(",");
			}
			sprintf(time_c,"%d",currentTime.tm_hour);
			temp.append(time_c);
			//temp.append("点");
			temp.append(" hour ");
			sprintf(time_c,"%d",currentTime.tm_min);
			temp.append(time_c);
			//temp.append("分钟\n");
			temp.append("minute\n");
			temp.append("bundle id:");
			sprintf(time_c,"%d",bundle->bundleid());
			temp.append(time_c);
			temp.append("\n");

			temp.append("bundle timestamp:");
			temp.append("seconds_:");
			sprintf(time_c,"%d",bundle->creation_ts().seconds_);
			temp.append("  seqno_:");
			sprintf(time_c,"%d",bundle->creation_ts().seqno_);
			temp.append(time_c);
			temp.append("\n");

			//temp.append("bundle的类型: ");
			temp.append("bundle's type: ");
			if(bundle->getBundleType()==1)
			{
				temp.append("data_bundle\n");
				//temp.append("bundle的目的区域: ");
				temp.append("bundle's destination area: ");
				sprintf(time_c,"%d",bundle->bottomArea());
				temp.append(time_c);
				temp.append("\n");
			}
			if(bundle->getBundleType()==2)
			{
				temp.append("neighbour_bundle\n");
			}
			//temp.append("bundle的源结点: ");
			temp.append("bundle's source node: ");
			temp.append(bundle->source().str());
			//temp.append("\nbundle的目的结点: ");
			temp.append("\nbundle's destination node: ");
			temp.append(bundle->dest().str());
			//temp.append("\nbundle的负载: ");
			temp.append("\nbundle's payload: ");
			sprintf(time_c,"%d",bundle->payload().length());
			temp.append(time_c);
			temp.append("\n\n");
		}

		fseek(fr, 0, SEEK_END);
		int h=fwrite(temp.c_str(),sizeof(char),strlen(temp.c_str()),fr);
		fflush(fr);

		if(EOF == fclose(fr))
		{
			printf("err:fclose failed\n");
			unlock_BundleLog();
			return ;
		}
		unlock_BundleLog();


		if(bundle->getBundleType()==Bundle::DATA_BUNDLE &&
				bundle->dest().str()==BundleDaemon::GetInstance()->local_eid().str())
		{
			lock_DataBundleLog();
			FILE * fr2;
			errno=0;
			fr2= fopen(DataBundleLogPath.c_str(),"a");
			if (NULL == fr2)
			{
				if (EINVAL == errno)
					printf("err:fopen log file %s failed\n",DataBundleLogPath.c_str());
				else
					printf("err:unknow\n");
			}

			fseek(fr2, 0, SEEK_END);
			int h=fwrite(temp.c_str(),sizeof(char),strlen(temp.c_str()),fr2);
			fflush(fr2);

			if(EOF == fclose(fr2))
			{
				printf("err:fclose failed\n");
				unlock_DataBundleLog();
				return ;
			}
			unlock_DataBundleLog();
		}

	}

	void writeSendBundleLogToFile(string reason,Bundle *bundle)
	{
		lock_BundleLog();
		FILE * fr;
		errno=0;
		fr= fopen(BundleLogPath.c_str(),"a");
		if (NULL == fr)
		{
			if (EINVAL == errno)
			{
				printf("err:fopen log file %s failed\n",BundleLogPath.c_str());
			}
			else
			{
				printf("err:unknow\n");
			}
		}
		string temp;
		string father;

		tm currentTime=CurrentTimeManager::Getinstance()->currentTime;
		if(bundle->getBundleType()!=Bundle::ORIGINAL_BUNDLE &&
			bundle->dest().str()!=BundleDaemon::GetInstance()->local_eid().str())
		{
			temp.append("send a new bundle:\n");
			char time_c[10];
			sprintf(time_c,"%d",currentTime.tm_mon+1);
			temp.append(time_c);
			//temp.append("月,");
			temp.append("month,");
			if(currentTime.tm_wday==0)
				//temp.append("星期天,");
				temp.append("Sunday,");
			else
			{
				sprintf(time_c,"%d",currentTime.tm_wday);
				//temp.append("星期");
				temp.append("week ");
				temp.append(time_c);
				temp.append(",");
			}
			sprintf(time_c,"%d",currentTime.tm_hour);
			temp.append(time_c);
			//temp.append("点");
			temp.append(" hour ");
			sprintf(time_c,"%d",currentTime.tm_min);
			temp.append(time_c);
			//temp.append("分钟\n");
			temp.append("minute\n");
			temp.append("bundle id:");
			sprintf(time_c,"%d",bundle->bundleid());
			temp.append(time_c);
			temp.append("\n");

			temp.append("bundle timestamp: ");
			temp.append("seconds_:");
			sprintf(time_c,"%d",bundle->creation_ts().seconds_);
			temp.append("  seqno_:");
			sprintf(time_c,"%d",bundle->creation_ts().seqno_);
			temp.append(time_c);
			temp.append("\n");

			//temp.append("bundle的类型: ");
			temp.append("bundle's type: ");
			if(bundle->getBundleType()==1)
			{
				temp.append("data_bundle\n");
				//temp.append("bundle的目的区域: ");
				temp.append("bundle's destination area: ");
				sprintf(time_c,"%d",bundle->bottomArea());
				temp.append(time_c);
				temp.append("\n");
			}
			if(bundle->getBundleType()==2)
			{
				temp.append("neighbour_bundle\n");
			}
			//temp.append("bundle的源结点: ");
			temp.append("bundle's source node: ");
			temp.append(bundle->source().str());
			//temp.append("\nbundle的目的结点: ");
			temp.append("\nbundle's destination node: ");
			temp.append(bundle->dest().str());
			//temp.append("\nbundle的负载: ");
			temp.append("\nbundle's payload: ");
			sprintf(time_c,"%d",bundle->payload().length());
			temp.append(time_c);
			temp.append("\n");
			temp.append(reason);
			temp.append("\n");
		}

		fseek(fr, 0, SEEK_END);
		int h=fwrite(temp.c_str(),sizeof(char),strlen(temp.c_str()),fr);
		fflush(fr);

		if(EOF == fclose(fr))
		{
			printf("err:fclose failed\n");
			unlock_BundleLog();
			return;
		}
		unlock_BundleLog();


		if(bundle->getBundleType()==Bundle::DATA_BUNDLE &&
				bundle->dest().str()!=BundleDaemon::GetInstance()->local_eid().str())
		{
			lock_DataBundleLog();
			FILE * fr2;
			errno=0;
			fr2= fopen(DataBundleLogPath.c_str(),"a");
			if (NULL == fr2)
			{
				if (EINVAL == errno)
					printf("err:fopen log file %s failed\n",DataBundleLogPath.c_str());
				else
					printf("err:unknow\n");
			}

			fseek(fr2, 0, SEEK_END);
			int h=fwrite(temp.c_str(),sizeof(char),strlen(temp.c_str()),fr2);
			fflush(fr2);

			if(EOF == fclose(fr2))
			{
				printf("err:fclose failed\n");
				unlock_DataBundleLog();
				return ;
			}
			unlock_DataBundleLog();
		}
	}


	///////////////////////////////////////////处理区域问题

	/////////////////////////////////////////////处理bundle信息
	unsigned int expiration; //=3600
	bool sendMessage2(BundleRef b,string fileroute,string source);
	bool sendMessage3(BundleRef b,size_t payload_len,char *filename,
			 string source_eid);
	void handle_sendBundle(SendBundleMsg *sbm);
	// u_char buffer[4096];
	bool sendMessage(string dest_eid,string fileroute,bool rctp,vector<int> areaid,int bundleType);
	///////////////////////////////////
	void run()
	{
		while(true)
		{
			//if(BundleDaemon::GetInstance()->shutting_down)
			//	break;
			//cout<<"geohistoryrouter"<<endl;
			Object_RouteMessage *o=areaInfoQueue.pop();
			AreaInfo *areaInfo=dynamic_cast<AreaInfo *>(o);
			if(areaInfo!=NULL)
			{
				handle_areamoving(areaInfo);
				continue;
			}
			SendBundleMsg *sbm=dynamic_cast<SendBundleMsg *>(o);
			if(sbm!=NULL)
			{
				handle_sendBundle(sbm);
				continue;
			}
			RouteAllBundleMsg *rab=dynamic_cast<RouteAllBundleMsg *>(o);
			if(rab!=NULL)
			{
				handle_routeAllBundle();
				continue;
			}

		sleep(2);
		}
	}

};

}
