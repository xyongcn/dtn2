//#include<vector>
//#include<string>
#include"servlib/geohistorydtn/config/AttenuationConfig.h"
//#include "geohistorydtn/area/AreaInfo.h"
#include "servlib/geohistorydtn/areaConnectiveSimulation/CurrentLocationFromSimulator.h"

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>

/**
 * 说明 ：时间区域频率向量的基类
 * 将向量的增加标志位的清除放在了衰减函数attenuationVector里面
 */
namespace dtn{
class FrequencyVector{


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
	     ar & name;

	}

public:
	virtual ~FrequencyVector(){}
	FrequencyVector(){}

	/**
	 * 时间向量的级别
	 */
	int vectorLevel;

	/**
	 * 该事件向量服务的类型，服务类型和向量级别共同决定衰减类型：比如服务于邻居的，或者服务于区域的
	 */
	int serviceType;


	/**
	 * 时间向量的值
	 */
	std::vector<double> vector_;

	/**
	 * 向量长度
	 */
	int vectorLength;

	/**
	 * 向量变动标志位
	 */
	std::vector<bool> vectorChange;

	std::string name;



	/*FrequencyVector(int vectorLevel ,int serviceType)
	{
		this->vectorLevel=vectorLevel;
		this->serviceType=serviceType;
	}*/

	/**
	 * 获取向量的值
	 */
	std::vector<double> getVector()
	{
		return vector_;
	}

	/**
	 * 获取向量等级
	 */
	int getFrequencyLevel()
	{
		return vectorLevel;
	}

	/**
	 * 获取服务的对象
	 */
	int getServiceType()
	{
		return serviceType;
	}

	/**
	 * 对更换区域时，处理当前时间段的向量进行处理；因为Info里面存放的包括改变区域时的时间，所以要根据这个info里面的时间来比较需要修改哪一维的频率向量
	 */
	virtual void changeVector(AreaInfo info){}

	/**
	 * 对到达计时时间后，处理当前时间段的向量
	 */
	virtual void changeVector(int time){}

	/**
	 * 衰减函数
	 */
	void attenuationVector()
	{
		//衰减
		double parameter=AttenuationConfig::getAttenuation(this->vectorLevel,this->serviceType);
		for(int i=0;i<vector_.size();i++)
		{
			vector_[i]=vector_[i]*parameter;
			cout<<i<<":"<<vector_[i]<<" ";
		}
	}

	/**
	 * 重置修改频率向量的修改标志位，这个在区域移动，重置衰减时间
	 */
	void resetChangeFVectorSign(){

		//清楚修改标志位
		for(int i=0;i<vectorChange.size();i++)
		{
			vectorChange[i]=false;
		}
	}

	std::string toString() {
		char c[20];
		sprintf(c,"%.4lf",vector_[0]);
		std::string s;
		s.append(c);

		for(int i=1;i<vector_.size();i++)
		{
			sprintf(c,"%.4lf",vector_[i]);
			s.append(",");
			s.append(c);
		}

		return s;
//		return super.toString();
	}
};
}
