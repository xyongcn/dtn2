/*#ifndef AREAMANAGER_H_
#define AREAMANAGER_H_
#include "servlib/geohistorydtn/area/AreaManager.h"
#endif*/

#ifndef NEIGHBOURMANAGER_H_
#define NEIGHBOURMANAGER_H_
#include"servlib/geohistorydtn/neighbour/NeighbourManager.h"
#endif

namespace dtn{
const string TimeManager::tag="TimeManager";
TimeManager * TimeManager::instance=NULL;

//每分钟需要触发的操作
	void TimeManager::minuteTask(int minute)
	{
		//日志输出及提示信息
		//Log.i(tag,getTimeNow()+"\t触发小时操作");
		cout<<"触发分钟任务"<<endl;
		if(minute==0)
			FrequencyVectorManager::Getinstance()->minuteResetFrequency();

		for(list<FrequencyVector *>::iterator it=minFVectorQueue.begin();
						it!=minFVectorQueue.end();++it)
		{
			//(dynamic_cast<MinuteFrequencyVector*>(*it))->changeVector(minute);
			(*it)->changeVector(minute);

		}
		//测试使用,分钟尺度，并不应该进行衰减和写入Area的信息。
		FrequencyVectorManager::Getinstance()->minuteAttenuation();
		//将历史区域向量记录到文件中
		cout<<"writeAreaInfoToFile!"<<endl;
		AreaManager::Getinstance()->wrieteAreaInfoToFile();
		//将历史邻居向量记录到文件中
		NeighbourManager::Getinstance()->saveHistoryNeighbour();
		//测试 将邻居的频率向量和区域的频率向量全部显示出来
		NeighbourManager::Getinstance()->printAllNeighbour();
		AreaManager::Getinstance()->printAllAreaMoving();
	}

	//每小时需要触发的操作
	void TimeManager::hourTask(int hour)
	{
		//日志输出及提示信息
		//Log.i(tag,getTimeNow()+"\t触发小时操作");
		cout<<"触发小时任务"<<endl;
		if(hour==0)
			FrequencyVectorManager::Getinstance()->hourResetFrequency();

		for(list<FrequencyVector *>::iterator it=hourFVectorQueue.begin();
							it!=hourFVectorQueue.end();++it)
		{
			(*it)->changeVector(hour);
		}

		//对所有的需要小时衰减的向量进行
		FrequencyVectorManager::Getinstance()->hourAttenuation();

		//将历史区域向量记录到文件中
		AreaManager::Getinstance()->wrieteAreaInfoToFile();
		//将历史邻居向量记录到文件中
		NeighbourManager::Getinstance()->saveHistoryNeighbour();
		//测试 将邻居的频率向量和区域的频率向量全部显示出来

		//NeighbourManager::Getinstance()->printAllNeighbour();
		//AreaManager::Getinstance()->printAllAreaMoving();
	}

	//每星期需要触发的操作
	void TimeManager::weekTask(int week)
	{
		//日志输出及提示信息
	//	Log.i(tag,getTimeNow()+"\t触发周操作");
	//	AreaManager.writeAreaTimeChange2Log(getSimplifyTime());
		cout<<"触发星期任务"<<endl;
		if(week==0)
			FrequencyVectorManager::Getinstance()->weekResetFrequency();

		for(list<FrequencyVector *>::iterator it=weekFVectorQueue.begin();
							it!=weekFVectorQueue.end();++it)
		{
			(*it)->changeVector(week);
		}

		//对所有的需要小时衰减的向量进行
		FrequencyVectorManager::Getinstance()->weekAttenuation();

	}

	//每月需要触发的操作
	void TimeManager::monthTask(int month)
	{
		//日志输出及提示信息
		//Log.i(tag,getTimeNow()+"\t触发月操作");
		//AreaManager.writeAreaTimeChange2Log(getSimplifyTime());
		cout<<"触发月任务"<<endl;
		if(month==0)
			FrequencyVectorManager::Getinstance()->monthResetFrequency();
		for(list<FrequencyVector *>::iterator it=monFVectorQueue.begin();
							it!=monFVectorQueue.end();++it)
		{
			(*it)->changeVector(month);
		}

		//对所有的需要小时衰减的向量进行
		FrequencyVectorManager::Getinstance()->monthAttenuation();
	}


	//每月需要触发的操作
	void TimeManager::monafteveTask(int monafteve)
	{
		//日志输出及提示信息
		//Log.i(tag,getTimeNow()+"\t触发月操作");
		//AreaManager.writeAreaTimeChange2Log(getSimplifyTime());
		cout<<"触发上午下午晚上任务"<<endl;
		if(monafteve==0)
			FrequencyVectorManager::Getinstance()->monafteveResetFrequency();
		for(list<FrequencyVector *>::iterator it=monafteveFVectorQueue.begin();
							it!=monafteveFVectorQueue.end();++it)
		{
			(*it)->changeVector(monafteve);
		}

		//对所有的需要小时衰减的向量进行
		FrequencyVectorManager::Getinstance()->monafteveAttenuation();
	}

}
