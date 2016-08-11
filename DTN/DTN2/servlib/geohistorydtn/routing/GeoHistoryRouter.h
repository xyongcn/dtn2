
#include "servlib/routing/TableBasedRouter.h"

#include<string>
#include "servlib/geohistorydtn/routing/BlockingQueue.h"

#ifndef NEIGHBOURMANAGER_H_
#define NEIGHBOURMANAGER_H_
#include"servlib/geohistorydtn/neighbour/NeighbourManager.h"
#endif
#include "servlib/geohistorydtn/config/BundleConfig.h"
//#include <oasys/thread/Thread.h>
//#include "geohistorydtn/questAreaInfo/QuestAreaInfo.h"

//#ifndef	AREA_H_
//#define AREA_H_
//#include "geohistorydtn/area/Area.h"
//#endif



using namespace std;
using namespace oasys;

namespace dtn {
//class TableBasedRouter;
class GeoHistoryRouter : public TableBasedRouter,public Thread

{
public:


	//pthread_mutex_t areaInfoQueue_Mutex;
	//queue<AreaInfo> areaInfoQueue;
	BlockingQueue <Object_RouteMessage*> areaInfoQueue;
	GeohistoryLog *geohistoryLog;

	GeoHistoryRouter()
	:TableBasedRouter("GeoHistoryRouter", "geohistory"),
	 Thread("GeoHistoryRouter", CREATE_JOINABLE)
	{
		expiration=BundleConfig::EXPIRATION_TIME;
		baseArea=NULL;
		areamanager=AreaManager::Getinstance();
		geohistoryLog=GeohistoryLog::GetInstance();

	}

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

	///////////////////////////////////////////处理区域问题

	/////////////////////////////////////////////处理bundle信息
	unsigned int expiration; //=3600
	bool sendMessage2(BundleRef b,string fileroute,dtn_endpoint_id_t source_eid);
	bool sendMessage3(BundleRef b,size_t payload_len,char *filename,
			 dtn_endpoint_id_t source_eid);
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
			cout<<"geohistoryrouter"<<endl;
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


		sleep(2);
		}
	}

};

}
