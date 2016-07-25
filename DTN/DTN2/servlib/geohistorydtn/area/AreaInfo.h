//#include"geohistorydtn/areaConnectiveSimulation/CurrentLocationFromSimulator.h"
#include "AreaLevel.h"
#include <vector>
#include "servlib/geohistorydtn/timeManager/CurrentTimeManager.h"

using namespace std;

/**
 * 作用：用来表示一个区域变化的必要信息
 *
 * areaId,从0到3，0就是最底层区域的id，从1到3已经是从顶层到底层的id
 */

namespace dtn
{

//class TimeManeger;
class AreaInfo {
public:
	/**
	 * 各个区域层次的id，从底层到到高层，范围依次扩大
	 */
	vector<int> areaId;
	int minute;
	int hour;
	int month;
	int week;
	int monafteve;

	/**
	 * 底层区域的数组位置,在目前情况下，区域的向量是四维的，因此底层区域存储在下标为3处
	 * 最底层是区域标识id，和最底层区域的唯一id一样，一旦区域标识id不同，
	 * 则表示一定移动了区域
	 */
	int baseAreaNum;

	/**
	 * 底层区域的对应的level
	 */
	int baseAreaLevel;
	AreaInfo()
	{

	}

	AreaInfo(vector<int> areaid)
	{
		this->areaId.push_back(areaid[areaid.size()-1]);
		for(int i=0;i<areaid.size();++i)
			this->areaId.push_back(areaid[i]);
		baseAreaNum=areaid.size();
		baseAreaLevel=areaid.size();
		//获取当前时间
		CurrentTimeManager *t=CurrentTimeManager:: Getinstance();
		this->minute=t->getMinuteNum();
		this->hour=t->getHourNum();
		this->week=t->getWeekDayNum();
		this->month=t->getMonthDayNum();
		this->monafteve=t->getMonafteveNum();
	}

	AreaInfo(vector<int> areaid,int minute,int hour,int week,int month,int monafteve)
	{
		this->areaId.push_back(areaid[areaid.size()-1]);
		for(int i=0;i<areaid.size();++i)
			this->areaId.push_back(areaid[i]);
		baseAreaNum=areaid.size();
		baseAreaLevel=areaid.size();

		this->minute=minute;
		this->hour=hour;
		this->week=week;
		this->month=month;
		this->monafteve=monafteve;
	}

	/**
	 * 由于程序中的分层信息有是否三层的错误校验，所以这里的分层信息只设置三层
	 */
	AreaInfo(AreaLayerInfo layerinfo)
	{
		//由于layerinfo的区域分层信息是从上层到底层排列,为0,1,2,3...
		//而AreaInfo中，0存储的是最底层，1,2,3...再依次存储各层

		this->areaId.push_back(layerinfo.arealist[layerinfo.arealist.size()-1]);
		for(int i=0;i<layerinfo.arealist.size();++i)
			this->areaId.push_back(layerinfo.arealist[i]);

		baseAreaNum=layerinfo.arealist.size();
		baseAreaLevel=layerinfo.arealist.size();

		//获取当前时间
		CurrentTimeManager *t=CurrentTimeManager:: Getinstance();
		this->minute=t->getMinuteNum();
		this->hour=t->getHourNum();
		this->week=t->getWeekDayNum();
		this->month=t->getMonthDayNum();
		this->monafteve=t->getMonafteveNum();
	}

	/**
	 * 说明：根据另一个AreaInfo来更新当前类的时间
	 */
	void updataTime(AreaInfo another)
	{
		this->minute=another.minute;
		this->hour=another.hour;
		this->week=another.week;
		this->month=another.month;
		this->monafteve=another.monafteve;
	}

	vector<int> getAreaId()
	{
		return areaId;
	}

	int getMinute()
	{
		return minute;
	}


	int getHour()
	{
		return hour;
	}

	int getMonth()
	{
		return month;
	}

	int getWeek()
	{
		return week;
	}

	int getMonafteve()
	{
		return monafteve;
	}

	//返回底层的区域的id
	int getBaseAreaId()
	{
		return areaId[baseAreaNum];
	}

	//获取底层区域id所在数组的位置
	int getBaseAreaNum()
	{
		return baseAreaNum;
	}

	//获取底层区域的level等级
	int getBaseAreaLevel()
	{
		return baseAreaLevel;
	}

	bool equals(AreaInfo another)
	{
		bool isEqual=true;

		isEqual=baseAreaLevel==another.baseAreaLevel && baseAreaNum==another.baseAreaNum;

		if(!isEqual)
			return false;

		for(int i=0;i<areaId.size();i++)
		{
			if(areaId[i]!=another.areaId[i]){
					return false;
			}
		}

		return true;
	}
	/**
	 * 返回默认的区域分层信息，由于是默认的，所以一般作为邻居间交换区域信息时，填充使用
	 * @return
	 */
	static vector<int> defaultAreaId()
	{
		vector<int> v(AreaLevel::MAXLEVEL,-1);
		return v;

	}
};
}
