#include"FrequencyVector.h"

namespace dtn{


class MinuteFrequencyVector: public FrequencyVector
{
	private:
	friend class boost::serialization::access;
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version)
	    {
	    	ar & boost::serialization::base_object<FrequencyVector>(*this);

	    }

public:
	    MinuteFrequencyVector(){}

	MinuteFrequencyVector(int vectorLevel, int serviceType)
	{
		//FrequencyVector(FrequencyVectorLevel::minuteVector, serviceType);
		this->vectorLevel=FrequencyVectorLevel::minuteVector;
		this->serviceType=serviceType;
		init();
	}

	void init()
	{
		this->vector_.insert(this->vector_.end(),60,0);
		this->vectorChange.insert(this->vectorChange.end(),60,false);
		this->vectorLength=60;
		this->name="minuteFrequencyVector";
	}

	 void changeVector(AreaInfo info) {
		int minute=info.getMinute();
		if(minute>=0 && minute<60)
		{
			if(!vectorChange[minute])
			{
				//对该向量加1
				++vector_[minute];
				vectorChange[minute]=true;
			}
		}
	}

//暂时留下，会改
	 //打算计时器传递时间参数   void changeVector(int minute)
	void changeVector(int minute) {

		//int minute=CurrentTimeManager::Getinstance()->getMinuteNum();
		if(minute>=0 && minute<60)
		{
			if(!vectorChange[minute])
			{
				//对该向量加1
				++vector_[minute];
				vectorChange[minute]=true;
			}
		}
	}

};

}

