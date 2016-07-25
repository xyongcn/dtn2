
#include "servlib/routing/TableBasedRouter.h"
#include<string>
#include "servlib/geohistorydtn/routing/BlockingQueue.h"

#ifndef AREAMANAGER_H_
#define AREAMANAGER_H_
#include "servlib/geohistorydtn/area/AreaManager.h"
#endif

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
	BlockingQueue <AreaInfo> areaInfoQueue;

	GeoHistoryRouter()
	:TableBasedRouter("GeoHistoryRouter", "geohistory"),
	 Thread("GeoHistoryRouter", CREATE_JOINABLE)
	{
		baseArea=NULL;
		areamanager=AreaManager::Getinstance();

	}

	virtual ~GeoHistoryRouter(){}

	///////////////////////////处理区域问题

	Area *baseArea;
	AreaManager *areamanager;

	void movetoArea(AreaInfo areaInfo)
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


	void handle_areamoving(AreaInfo areaInfo)
	{
		if(isChangedArea(areaInfo))
		{
			//生成对应的Area
			Area *newBaseArea=areamanager->checkAreaInfo(areaInfo);
			cout<<newBaseArea->id<<endl;
			if(newBaseArea!=NULL)
			{
				//变动相应的频率向量,因为每次计时到了就开始改变FVector，所以在变更区域的时候不需要进行变动频率向量
				newBaseArea->changeFVector(areaInfo);

				//加入到新的倒数时间列表里面,将原来的从倒数计时里面去掉
				changeAreaTimeCount(baseArea, newBaseArea);

				//更改当前的底层区域向量
				baseArea=newBaseArea;

				//将移动的记录保存到日志中
				//areamanager->writeAreaLogToFile("move to new area, ",baseArea);
			}
		}
	}

	///////////////////////////////////////////处理区域问题
	void run()
	{
		while(true)
		{
			//if(BundleDaemon::GetInstance()->shutting_down)
			//	break;
			cout<<"geohistoryrouter"<<endl;
			AreaInfo a=areaInfoQueue.pop();
			handle_areamoving(a);


		sleep(2);
		}
	}

};

}
