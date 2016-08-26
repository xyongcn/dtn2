#include<iostream>
#include<string>
#include<list>
#include <signal.h>
#include<errno.h>

//#pragma once
//#include"geohistorydtn/areaConnectiveSimulation/CurrentLocationFromSimulator.h"
//#include "geohistorydtn/frequencyVector/FrequencyVector.h"
#include "servlib/geohistorydtn/frequencyVector/FrequencyVectorManager.h"
//#include "geohistorydtn/area/AreaManager.h"
//#include "geohistorydtn/frequencyVector/FrequencyVectorLevel.h"
//#include "geohistorydtn/frequencyVector/FrequencyVectorManager.h"
//#include "Time.h"

//#include <oasys/thread/Thread.h>


#define TIMECOUNTINTERVAL 60

using namespace std;
/**
 * 说明 :用来对各种倒数计时的事件管理的类，其实本质上是一个计时和相应事件触发的事件，计时线程和事件触发线程应当分离开
 */

namespace dtn {


class TimeManager:public Thread
{
private:
	static TimeManager *instance;

	static const string tag;

	CurrentTimeManager *currentTime;
	bool timeTaskRun;

	/**
	 * 当前时间流动，
	 */
//	tm currentTime;
//	int currentTimemonafteve;//上午6:00-12:00 为1，下午12:00-18:00为2，晚上18:00-6:00为3
	/**
	 * 下一个计时事件
	 */
//	tm lastTime;
	//int lastTimemonafteve;

	bool shutdown_flag;


	//构造函数
	TimeManager()
	:Thread("TimeManager", CREATE_JOINABLE)
	{

		timeTaskRun=false;
		currentTime=CurrentTimeManager::Getinstance();
		init();

//		hourFVectorQueue=new LinkedBlockingDeque<FrequencyVector>();
//		weekFVectorQueue=new LinkedBlockingDeque<FrequencyVector>();
	//	monFVectorQueue=new LinkedBlockingDeque<FrequencyVector>();
		/*hourFVectorQueue=new LinkedBlockingDeque<FrequencyVector>();
		weekFVectorQueue=new LinkedBlockingDeque<FrequencyVector>();
		monFVectorQueue=new LinkedBlockingDeque<FrequencyVector>();*/

	}

	//每初次运行计时功能时，都要调用的代码
public:

	 void shutdown()
	 {
		 shutdown_flag=true;
	 }


	void init()
	{
		//初次启用计时代码
		/*currentTime=FrequencyConfig::getCurrentTime();
		if(currentTime.tm_hour > 6 && currentTime.tm_hour <12)
			currentTimemonafteve=1;
		else if(currentTime.tm_hour > 12 && currentTime.tm_hour <8)
			currentTimemonafteve=2;
		else
			currentTimemonafteve=3;
		lastTime=FrequencyConfig::getCurrentTime();
		lastTimemonafteve=currentTimemonafteve;*/
		shutdown_flag=false;
	}

    static TimeManager* GetInstance()
    {
        if(instance == NULL)  //判断是否第一次调用
        	instance = new TimeManager();

        return instance ;
    }

	/**
	 * 对照FrequencyConfig里面的时间配置，重置计时器的双时间
	 */
/*	void resetTime()
	{
		currentTime=FrequencyConfig::getCurrentTime();
		if(currentTime.tm_hour > 6 && currentTime.tm_hour <12)
			currentTimemonafteve=1;
		else if(currentTime.tm_hour > 12 && currentTime.tm_hour <8)
			currentTimemonafteve=2;
		else
			currentTimemonafteve=3;
		//minNum=currentTime.tm_min; //时间的范围是0-59
	//	hourNum=currentTime.tm_hour;//时间的范围是0-23
		//weekDayNum=currentTime.tm_wday;//星期在日历中范围是0-6
		//monthDayNum=currentTime.tm_mon;//月的范围是0-11
	}*/



	/**
	 * 小时级别的频率向量的通知
	 * FrequencyVectorLevel::hourVector
	 *
	 */
	list< FrequencyVector*> hourFVectorQueue;

	/**
	 * 星期级别的频率向量
	 * FrequencyVectorLevel::weekVector
	 *
	 */
	list<FrequencyVector*> weekFVectorQueue;

	/**
	 * 月级别的频率向量
	 * FrequencyVectorLevel::monthVector
	 */
	list<FrequencyVector*> monFVectorQueue;



	list<FrequencyVector*> minFVectorQueue;

	list<FrequencyVector*> monafteveFVectorQueue;

	/**
	 * 添加频率向量到计时队列
	 * @param vector:频率向量
	 */
	void addVectorListen(FrequencyVector *vector)
	{
		if(vector==NULL)
			return;

		switch(vector->getFrequencyLevel())
		{
		case FrequencyVectorLevel::hourVector:
			hourFVectorQueue.push_back(dynamic_cast<HourFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::weekVector:
			weekFVectorQueue.push_back(dynamic_cast<WeekFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::monthVector:
			monFVectorQueue.push_back(dynamic_cast<MonthFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::minuteVector:
			minFVectorQueue.push_back(dynamic_cast<MinuteFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::monAftEveVector:
			monafteveFVectorQueue.push_back(dynamic_cast<MonAftEveFrequencyVector*>(vector));
			break;
		}
	}

	/**
	 * 移除计时队列的频率向量
	 * @param vector：频率向量
	 */
	void removeVectorListen(FrequencyVector *vector_)
	{
		if(vector_==NULL)
			return;

		switch(vector_->getFrequencyLevel())
		{
		case FrequencyVectorLevel::hourVector:
			for(list<FrequencyVector *>::iterator it=hourFVectorQueue.begin();
					it!=hourFVectorQueue.end();++it)
			{
				if(*it==vector_)
				{
					hourFVectorQueue.erase(it);
					break;
				}
			}
		break;

		case FrequencyVectorLevel::weekVector:
		for(list<FrequencyVector *>::iterator it=weekFVectorQueue.begin();
				it!=weekFVectorQueue.end();++it)
		{
			if(*it==vector_)
			{
				weekFVectorQueue.erase(it);
				break;
			}
		}
		break;

		case FrequencyVectorLevel::monthVector:
		for(list<FrequencyVector *>::iterator it=monFVectorQueue.begin();
				it!=monFVectorQueue.end();++it)
		{
			if(*it==vector_)
			{
				monFVectorQueue.erase(it);
				break;
			}

		}
		break;

		case FrequencyVectorLevel::minuteVector:
		for(list<FrequencyVector *>::iterator it=minFVectorQueue.begin();
				it!=minFVectorQueue.end();++it)
		{
			if(*it==vector_)
			{
				minFVectorQueue.erase(it);
				break;
			}
		}
		break;

		case FrequencyVectorLevel::monAftEveVector:
		for(list<FrequencyVector *>::iterator it=monafteveFVectorQueue.begin();
				it!=monafteveFVectorQueue.end();++it)
		{
			if(*it==vector_)
			{
				monafteveFVectorQueue.erase(it);
				break;
			}
		}
		break;
		}
	}


	//每分钟需要触发的操作
	void minuteTask(int minute);

	//每小时需要触发的操作
	void hourTask(int hour);

	//每星期需要出发的操作
	void weekTask(int week);

	//每月需要触发的操作
	void monthTask(int month);
	//每月需要触发的操作
	void monafteveTask(int monafteve);


	//每隔1分钟倒数计时一次，这样计时器的最小时间就是一分钟




		//执行相应的任务
	void Task()
	{
		cout<<"time!"<<endl;
		currentTime->resetTime();

		vector<int> frequencyType(FrequencyConfig::frequcyType);
		for(int i=0;i<frequencyType.size();++i)
		{
			if(frequencyType[i]==FrequencyVectorLevel::minuteVector)
			{
				//分钟级触发任务
				if(currentTime->currentTime.tm_min!=currentTime->lastTime.tm_min)
				{
					//执行分钟级触发的任务
					minuteTask(currentTime->currentTime.tm_min);
				}
			}


			if(frequencyType[i]==FrequencyVectorLevel::hourVector)
			{
				//小时级触发任务
				if(currentTime->currentTime.tm_hour!=currentTime->lastTime.tm_hour)
				{
					//执行小时级触发的任务
					hourTask(currentTime->currentTime.tm_hour);
				}
			}

			if(frequencyType[i]==FrequencyVectorLevel::weekVector)
			{
				//星期级触发任务

				if(currentTime->currentTime.tm_wday!=currentTime->lastTime.tm_wday)
				{
					//对应星期级的任务触发
					weekTask(currentTime->currentTime.tm_wday);
				}
			}

			if(frequencyType[i]==FrequencyVectorLevel::monthVector)
			{
				//月级触发任务
				if(currentTime->currentTime.tm_mon!=currentTime->lastTime.tm_mon)
				{
					//对应月级的任务触发
					monthTask(currentTime->currentTime.tm_mon);
				}
			}

			if(frequencyType[i]==FrequencyVectorLevel::monAftEveVector)
			{
				if(currentTime->currentTimemonafteve!=currentTime->lastTimemonafteve)
				{
					//对应月级的任务触发
					monafteveTask(currentTime->currentTimemonafteve);
				}

			}
		}

		currentTime->lastTime=currentTime->currentTime;
		currentTime->lastTimemonafteve=currentTime->currentTimemonafteve;


	}




	void run()
	{

		while(true)
		{
			Task();
			if(shutdown_flag)
				return;
			sleep(30);

		}
		/*signal(SIGALRM,TaskFunction);

		struct itimerval tick;
		tick.it_value.tv_sec = 0;  //十秒钟后将启动定时器
		tick.it_value.tv_usec = 0;
		tick.it_interval.tv_sec =2; //定时器启动后，每隔1秒将执行相应的函数
		tick.it_interval.tv_usec = 0;
		int ret = setitimer(ITIMER_REAL, &tick, NULL);
		if ( ret != 0)
		{
		    printf("Set timer error. %s \n", strerror(errno));
		    return;
		}

		if(shutdown_flag)
			return;*/
	}


};
//const string TimeManager::tag="TimeManager";
//TimeManager * TimeManager::instance=NULL;



}

