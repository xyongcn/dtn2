#include<string>
#include<list>
//#include"FrequencyVector.h"
#include"MonthFrequencyVector.h"
//#include "geohistorydtn/Log/GeohistoryLog.h"

namespace dtn{
using namespace std;

class FrequencyVectorManager{
private :
	static const string tag;
	static FrequencyVectorManager *Instance;

	FrequencyVectorManager()
	{
		geohistoryLog=GeohistoryLog::GetInstance();
		init();
	}

	void init()
	{
	}

public:
	GeohistoryLog *geohistoryLog;
	/**
	 * 存储管理时间向量的列表
	 */
	list<FrequencyVector *> hourvectorlist;
	list<FrequencyVector *> weekvectorlist;
	list<FrequencyVector *> monthvectorlist;
	list<FrequencyVector *> minutevectorlist;
	list<FrequencyVector *> monaftevevectorlist;


	static FrequencyVectorManager* Getinstance()
	{
	        if(Instance == NULL)  //判断是否第一次调用
	        	Instance = new FrequencyVectorManager();


	        return Instance ;
	}

	/**
	 * 对向量进行衰减
	 */


	void minuteResetFrequency()
	{
		list<FrequencyVector *>::iterator iter=minutevectorlist.begin();
		while(iter!=minutevectorlist.end())
		{
			FrequencyVector *minutevector=*iter;
			cout<<"reset all miute frequency:"<<endl;
			minutevector->resetChangeFVectorSign();
			iter++;
		}

	}

	void hourResetFrequency()
	{
		list<FrequencyVector *>::iterator iter=hourvectorlist.begin();
		while(iter!=hourvectorlist.end())
		{
			FrequencyVector *hourvector=*iter;
			cout<<"reset all hour frequency:"<<endl;
			hourvector->resetChangeFVectorSign();
			iter++;
		}

	}

	void monafteveResetFrequency()
	{
		list<FrequencyVector *>::iterator iter=monaftevevectorlist.begin();
		while(iter!=monaftevevectorlist.end())
		{
			FrequencyVector *monaftevevector=*iter;
			cout<<"reset all monafteve frequency:"<<endl;
			monaftevevector->resetChangeFVectorSign();
			iter++;
		}

	}

	void weekResetFrequency()
	{
		list<FrequencyVector *>::iterator iter=weekvectorlist.begin();
		while(iter!=weekvectorlist.end())
		{
			FrequencyVector *weekvector=*iter;
			cout<<"reset all week frequency:"<<endl;
			weekvector->resetChangeFVectorSign();
			iter++;
		}

	}

	void monthResetFrequency()
	{
		list<FrequencyVector *>::iterator iter=monthvectorlist.begin();
		while(iter!=monthvectorlist.end())
		{
			FrequencyVector *monthvector=*iter;
			cout<<"reset all month frequency:"<<endl;
			monthvector->resetChangeFVectorSign();
			iter++;
		}

	}



	void hourAttenuation()
	{
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,
								"All hour Frequency Vector Attenuation");
		list<FrequencyVector *>::iterator iter=hourvectorlist.begin();
		while(iter!=hourvectorlist.end())
		{
			FrequencyVector *hourvector=*iter;
			hourvector->attenuationVector();
			iter++;
		}
	}
	void weekAttenuation()
	{
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,
								"All week Frequency Vector Attenuations");
		list<FrequencyVector *>::iterator iter=weekvectorlist.begin();
		while(iter!=weekvectorlist.end())
		{
			FrequencyVector *weekvector=*iter;
			weekvector->attenuationVector();
			iter++;
		}
	}
	void monthAttenuation()
	{
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,
								"All month Frequency Vector Attenuations");
		list<FrequencyVector *>::iterator iter=monthvectorlist.begin();
		while(iter!=monthvectorlist.end())
		{
			FrequencyVector *monthvector=*iter;
			monthvector->attenuationVector();
			iter++;
		}
	}

	//测试使用
	void minuteAttenuation()
	{
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,
								"All minute Frequency Vector Attenuation");
		list<FrequencyVector *>::iterator iter=minutevectorlist.begin();
		while(iter!=minutevectorlist.end())
		{
			FrequencyVector *minutevector=*iter;
			minutevector->attenuationVector();
			//cout<<endl;
			iter++;
		}
	}


	void monafteveAttenuation()
	{
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,
								"All monafteve Frequency Vector Attenuation");
		list<FrequencyVector *>::iterator iter=monaftevevectorlist.begin();
		while(iter!=monaftevevectorlist.end())
		{
			FrequencyVector *monaftevevector=*iter;
			monaftevevector->attenuationVector();
			iter++;
		}
	}




	//对外的公共方法
	/**
	 * 作用：由频率向量管理器来生成相关的向量
	 * @param vectorLevel
	 * @param serviceType
	 * @return
	 */
	FrequencyVector *newFVector(int vectorLevel,int serviceType)
	{


		if(vectorLevel==FrequencyVectorLevel::hourVector)
		{
			FrequencyVector* vector= new HourFrequencyVector(vectorLevel, serviceType);
			//hourvectorlist.push_back(*vector);
			return vector;
		}
		else if(vectorLevel==FrequencyVectorLevel::weekVector)
		{
			FrequencyVector* vector= new WeekFrequencyVector(vectorLevel, serviceType);
			//weekvectorlist.push_back(*vector);
			return vector;
		}
		else if(vectorLevel==FrequencyVectorLevel::monthVector)
		{
			FrequencyVector* vector= new MonthFrequencyVector(vectorLevel, serviceType);
			//monthvectorlist.push_back(*vector);
			return vector;
		}
		else if(vectorLevel==FrequencyVectorLevel::minuteVector)
		{
			FrequencyVector* vector= new MinuteFrequencyVector(vectorLevel, serviceType);
			//minutevectorlist.push_back(*vector);
			return vector;
		}
		else if(vectorLevel==FrequencyVectorLevel::monAftEveVector)
		{
			FrequencyVector* vector= new MonAftEveFrequencyVector(vectorLevel, serviceType);
			//monaftevevectorlist.push_back(*vector);
			return vector;
		}

	}

	/**
	 * 针对从文件中读取到的区域的频率向量或者邻居频率向量，将其添加到向量列表中
	 */
	void addFVector(FrequencyVector *vector){
		//根据要添加的频率向量的级别来添加
		switch(vector->vectorLevel){
		case FrequencyVectorLevel::hourVector:
			hourvectorlist.push_back(dynamic_cast<HourFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::weekVector:
			weekvectorlist.push_back(dynamic_cast<WeekFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::monthVector:
			monthvectorlist.push_back(dynamic_cast<MonthFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::minuteVector:
			minutevectorlist.push_back(dynamic_cast<MinuteFrequencyVector*>(vector));
			break;

		case FrequencyVectorLevel::monAftEveVector:
			monaftevevectorlist.push_back(dynamic_cast<MonAftEveFrequencyVector*>(vector));
			break;
		}
	}

	/**
	 * 对频率向量管理器所存储的频率向量引用清空
	 */
	 void shutdown(){

		hourvectorlist.clear();
		weekvectorlist.clear();
		monthvectorlist.clear();
	}
};
//const string FrequencyVectorManager::tag="FrequencyVectorManager";
//FrequencyVectorManager *FrequencyVectorManager::Instance=NULL;
}
