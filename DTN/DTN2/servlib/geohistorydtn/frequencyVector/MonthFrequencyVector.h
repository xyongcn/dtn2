#include"WeekFrequencyVector.h"

namespace dtn{
class MonthFrequencyVector: public FrequencyVector
{
private:
	friend class boost::serialization::access;
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version)
	    {
	    	ar & boost::serialization::base_object<FrequencyVector>(*this);

	    }

public:

	    MonthFrequencyVector(){}
	MonthFrequencyVector(int vectorLevel, int serviceType)
	{
		//FrequencyVector(FrequencyVectorLevel::monthVector, serviceType);
		this->vectorLevel=FrequencyVectorLevel::monthVector;
		this->serviceType=serviceType;
		init();
	}

	void init()
	{
		this->vector_.insert(this->vector_.end(),12,0);
		this->vectorChange.insert(this->vectorChange.end(),12,false);
		this->vectorLength=12;
		this->name="monthFrequencyVector";
	}

	 void changeVector(AreaInfo info) {
		int month=info.getMonth();
		if(month>=0 && month<12)
		{
			if(!vectorChange[month])
			{
				//对该向量加1
				++vector_[month];
				vectorChange[month]=true;
			}
		}
	}

//暂时留下，会改
	 //打算计时器传递时间参数   void changeVector(int month)
	void changeVector(int month) {

		//int month=CurrentTimeManager::Getinstance()->getMonthDayNum();
		if(month>=0 && month<12)
		{
			if(!vectorChange[month])
			{
				//对该向量加1
				++vector_[month];
				vectorChange[month]=true;
			}
		}
	}

};
}
