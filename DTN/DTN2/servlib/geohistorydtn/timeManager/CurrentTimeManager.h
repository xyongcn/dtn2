#include "servlib/geohistorydtn/config/FrequencyConfig.h"


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
	int currentTimemonafteve;//上午6:00-12:00 为1，下午12:00-18:00为2，晚上18:00-6:00为3
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
		if(currentTime.tm_hour > 6 && currentTime.tm_hour <12)
			currentTimemonafteve=1;
		else if(currentTime.tm_hour > 12 && currentTime.tm_hour <8)
			currentTimemonafteve=2;
		else
			currentTimemonafteve=3;
		lastTime=FrequencyConfig::getCurrentTime();
		lastTimemonafteve=currentTimemonafteve;

	}

	void resetTime()
	{
		currentTime=FrequencyConfig::getCurrentTime();
		if(currentTime.tm_hour > 6 && currentTime.tm_hour <12)
			currentTimemonafteve=1;
		else if(currentTime.tm_hour > 12 && currentTime.tm_hour <8)
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



