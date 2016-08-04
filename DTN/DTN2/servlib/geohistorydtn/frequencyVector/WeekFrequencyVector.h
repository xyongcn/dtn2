#include"MonAftEveFrequencyVector.h"

namespace dtn{
class WeekFrequencyVector: public FrequencyVector
{
private:
	friend class boost::serialization::access;
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version)
	    {
	    	ar & boost::serialization::base_object<FrequencyVector>(*this);

	    }
public:
	    WeekFrequencyVector(){}
	WeekFrequencyVector(int vectorLevel, int serviceType)
	{
		//FrequencyVector(FrequencyVectorLevel::weekVector, serviceType);
		this->vectorLevel=FrequencyVectorLevel::weekVector;
		this->serviceType=serviceType;
		init();
	}

	void init()
	{
		this->vector_.insert(this->vector_.end(),7,0);
		this->vectorChange.insert(this->vectorChange.end(),7,false);
		this->vectorLength=7;
		this->name="weekFrequencyVector";
	}

	 void changeVector(AreaInfo info) {
		int week=info.getWeek();
		if(week>=0 && week<7)
		{
			if(!vectorChange[week])
			{
				//对该向量加1
				++vector_[week];
				vectorChange[week]=true;
			}
		}
	}

//暂时留下，会改
	 //打算计时器传递时间参数   void changeVector(int week)
	void changeVector(int week) {

		//int week=CurrentTimeManager::Getinstance()->getWeekDayNum();
		if(week>=0 && week<7)
		{
			if(!vectorChange[week])
			{
				//对该向量加1
				++vector_[week];
				vectorChange[week]=true;
			}
		}
	}

};
}
