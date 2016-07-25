#include"HourFrequencyVector.h"


namespace dtn{
class MonAftEveFrequencyVector: public FrequencyVector
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

	MonAftEveFrequencyVector(int vectorLevel, int serviceType)
	{
		//FrequencyVector(FrequencyVectorLevel::monAftEveVector, serviceType);
		this->vectorLevel=FrequencyVectorLevel::monAftEveVector;
			this->serviceType=serviceType;
		init();
	}

	void init()
	{
		this->vector_.insert(this->vector_.end(),3,0);
		this->vectorChange.insert(this->vectorChange.end(),3,false);
		this->vectorLength=3;
		this->name="monAftEveFrequencyVector";
	}

	 void changeVector(AreaInfo info) {
		int monAftEve=info.getMonafteve();
		if(monAftEve>=0 && monAftEve<3)
		{
			if(!vectorChange[monAftEve])
			{
				//对该向量加1
				++vector_[monAftEve];
				vectorChange[monAftEve]=true;
			}
		}
	}

//暂时留下，会改
	 //打算计时器传递时间参数   void changeVector(int monAftEve)
	void changeVector(int monAftEve) {

		//int monAftEve=CurrentTimeManager::Getinstance()->getMonafteveNum();
		if(monAftEve>=0 && monAftEve<3)
		{
			if(!vectorChange[monAftEve])
			{
				//对该向量加1
				++vector_[monAftEve];
				vectorChange[monAftEve]=true;
			}
		}
	}

};
}
