#ifndef FCONFIG_H_
#define FCONFIG_H_
#include "servlib/geohistorydtn/config/FrequencyConfig.h"
#endif

namespace dtn
{
class CurrentTimeManager
{

private:
	CurrentTimeManager()
	{
		init();
	}
	static CurrentTimeManager *instance;

public:
	tm currentTime;
	//时间的范围是0-59
	//时间的范围是0-23
	//星期在日历中范围是0-6,其中0代表星期天，1代表星期一,
	//月的范围是0-11,从一月开始，0代表一月
	int currentTimemonafteve;//上午6:00-12:00 为0，下午12:00-18:00为1，晚上18:00-6:00为2
	/**
	 * 下一个计时事件
	 */
	tm lastTime;
	int lastTimemonafteve;

	static CurrentTimeManager* Getinstance()
	{
	        if(instance == NULL)  //判断是否第一次调用
	        	instance = new CurrentTimeManager;


	        return instance ;
	}
	void init()
	{

		//初次启用计时代码
		currentTime=FrequencyConfig::getCurrentTime();
		if(currentTime.tm_hour >=6 && currentTime.tm_hour <12)
			currentTimemonafteve=1;
		else if(currentTime.tm_hour >=12 && currentTime.tm_hour <20)
			currentTimemonafteve=2;
		else
			currentTimemonafteve=3;
		lastTime=FrequencyConfig::getCurrentTime();
		lastTimemonafteve=currentTimemonafteve;

	}

	void resetTime()
	{
		currentTime=FrequencyConfig::getCurrentTime();
		if(currentTime.tm_hour >= 6 && currentTime.tm_hour <12)
			currentTimemonafteve=1;
		else if(currentTime.tm_hour >= 12 && currentTime.tm_hour <20)
			currentTimemonafteve=2;
		else
			currentTimemonafteve=3;
	}

	 int getMinuteNum()
		{
			return currentTime.tm_min;
		}


		int getHourNum()
		{
			return currentTime.tm_hour;
		}

		int getWeekDayNum()
		{
			return currentTime.tm_wday;
		}

		int getMonthDayNum()
		{
			return currentTime.tm_mon;
		}

		int getMonafteveNum()
		{
			return currentTimemonafteve;
		}


};
}
//CurrentTimeManager *CurrentTimeManager::instance=NULL;



