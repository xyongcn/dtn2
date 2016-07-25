#include"MinuteFrequencyVector.h"

namespace dtn{
class HourFrequencyVector: public FrequencyVector
{
	private:
	friend class boost::serialization::access;
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version)
	    {
	        ar & vector_;//序列化或反序列化&操作符比>>和<<更为方便
	        ar & vectorLevel;
	        ar & serviceType;
	        ar & vectorLength;
	        ar & vectorChange;

	    }



public:

	HourFrequencyVector(int vectorLevel, int serviceType)
	{
		//FrequencyVector(FrequencyVectorLevel::hourVector, serviceType);
		this->vectorLevel=FrequencyVectorLevel::hourVector;
		this->serviceType=serviceType;
		init();
	}

	void init()
	{
		this->vector_.insert(this->vector_.end(),24,0);
		this->vectorChange.insert(this->vectorChange.end(),24,false);
		this->vectorLength=24;
		this->name="hourFrequencyVector";
	}

	 void changeVector(AreaInfo info) {
		int hour=info.getHour();
		if(hour>=0 && hour<24)
		{
			if(!vectorChange[hour])
			{
				//对该向量加1
				++vector_[hour];
				vectorChange[hour]=true;
			}
		}
	}

//暂时留下，会改
	 //打算计时器传递时间参数   void changeVector(int hour)
	void changeVector(int hour) {

	//	int hour=CurrentTimeManager::Getinstance()->getHourNum();
		if(hour>=0 && hour<24)
		{
			if(!vectorChange[hour])
			{
				//对该向量加1
				++vector_[hour];
				vectorChange[hour]=true;
			}
		}
	}

};
}
