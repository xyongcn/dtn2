#ifndef NEIGHBOUR_H_
#define NEIGHBOUR_H_
#include"Neighbour.h"
#endif



//BOOST_CLASS_EXPORT_GUID(dtn::HourFrequencyVector, "HourFrequencyVector")
//BOOST_CLASS_EXPORT_GUID(dtn::MinuteFrequencyVector, "MinuteFrequencyVector")
//BOOST_CLASS_EXPORT_GUID(dtn::WeekFrequencyVector, "WeekFrequencyVector")
//BOOST_CLASS_EXPORT_GUID(dtn::MonthFrequencyVector, "MonthFrequencyVector")
//BOOST_CLASS_EXPORT_GUID(dtn::MonAftEveFrequencyVector, "MonAftEveFrequencyVector")

namespace dtn
{
	const string Neighbour::tag="Neighbour";




	Neighbour::Neighbour(EndpointID eid)
	{
		geohistoryLog=GeohistoryLog::GetInstance();
		//if(eid==NULL)
		//	cout<<"the EndpointID is null while generate a new Neighbour"<<endl;

		this->neighbourEid=eid;
		int serviceType=FrequencyVectorServiceType::NEIGHBOUR;
		vector<int> frequencyType(FrequencyConfig::frequcyType);
		for(int i=0;i<frequencyType.size();++i)
		{
			if(frequencyType[i]==FrequencyVectorLevel::minuteVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::minuteVector,serviceType));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}
			if(frequencyType[i]==FrequencyVectorLevel::hourVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::hourVector,serviceType));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}

			if(frequencyType[i]==FrequencyVectorLevel::weekVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::weekVector,serviceType));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}
			if(frequencyType[i]==FrequencyVectorLevel::monthVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::monthVector,serviceType));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}
			if(frequencyType[i]==FrequencyVectorLevel::monAftEveVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::monAftEveVector,serviceType));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}

		}
		//生成该邻居的历史区域向量记录
		init();
	}

	/**
	 * 根据本地文件保存的邻居的区域信息来更改区域记录
	*/
	void Neighbour::init()
	{
		neighbourArea=new NeighbourArea(neighbourEid);
	}


	/**
	* 根据邻居的bundle的payload来生成自己的
	* @param payloadFile
	*/
	void Neighbour::generateArea()
	{
		//neighbourArea=new NeighbourArea(neighbourEid, payload);
		if(neighbourArea==NULL)
		{
			neighbourArea=new NeighbourArea(neighbourEid);
		}
		else
		{
			string file;
			file.append(NeighbourConfig::NEIGHBOURAREAFILEDIR);
			file.append(neighbourEid.str());

			fstream neighbourAreaFile(file.c_str());
			if(neighbourAreaFile)
			{
				geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"从文件historyarea中读取历史的区域信息");
				neighbourAreaFile.close();
				neighbourArea->updateArea(file);
			}
			else
			{
				geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"没有找到%s的payloadFile来更新邻居区域移动频率",neighbourEid.str().c_str());
				return;
			}

		}
	}

	/**
	* 根据邻居的payload的buffer来更新邻居的移动规律
	* @param buffer
	*/
	/*void generateArea(u_char buffer)
	{
		if(neighbourArea==null)
		{
			neighbourArea=new NeighbourArea(neighbourEid);
		}

		neighbourArea.updateArea(buffer);
	}*/


	NeighbourArea * Neighbour::getNeighbourArea()
	{
		return neighbourArea;
	}

	/**
	* 对向量在该时间段的分量进行处理
	*/
	void Neighbour::checkVectorChange()
	{
		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
			it!=vectorlist.end();++it)
		{
			switch((*it)->vectorLevel)
			{
			case FrequencyVectorLevel::minuteVector:
			{
				(*it)->changeVector(CurrentTimeManager::Getinstance()->getMinuteNum());
				break;
			}
			case FrequencyVectorLevel::hourVector:
			{
				(*it)->changeVector(CurrentTimeManager::Getinstance()->getHourNum());
				break;
			}
			case FrequencyVectorLevel::monAftEveVector:
			{
				(*it)->changeVector(CurrentTimeManager::Getinstance()-> getMonafteveNum());
				break;
			}
			case FrequencyVectorLevel::weekVector:
			{
				(*it)->changeVector(CurrentTimeManager::Getinstance()->getWeekDayNum());
				break;
			}
			case FrequencyVectorLevel::monthVector:
			{
				(*it)->changeVector(CurrentTimeManager::Getinstance()->getMonthDayNum());
				break;
			}
			}
		}
	}

	/**
	* 将频率向量加入到计时器中(没有被调用，应该需要添加)
	* 这里是让计时器来每段时间都改变一下频率向量，改变小时频率向量；星期频率向量；月频率向量
	*/
	void Neighbour::addTimeCount()
	{
		int i=1;
		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
			it!=vectorlist.end();++it)
		{
			FrequencyVector *v=*it;
			TimeManager::GetInstance()->addVectorListen(v);
		}
	}

	/**
	* 将Neighbour中的所有频率向量添加到频率向量管理器中，这样从文件中读取邻居时，频率向量的衰减器也可以对其进行频率的衰减了
	*/
	void Neighbour::addAreaFVector2Manager()
	{
		for(list<FrequencyVector *>::iterator it=vectorlist.begin()
			;it!=vectorlist.end();++it)
		{
			switch((*it)->vectorLevel)
			{
			case FrequencyVectorLevel::minuteVector:
			{
				MinuteFrequencyVector *v=dynamic_cast<MinuteFrequencyVector *>(*it);
				FrequencyVectorManager::Getinstance()->addFVector(v);
				break;
			}
			case FrequencyVectorLevel::hourVector:
			{
				HourFrequencyVector *v2=dynamic_cast<HourFrequencyVector *>(*it);
				FrequencyVectorManager::Getinstance()->addFVector(v2);
				break;
			}
			case FrequencyVectorLevel::monAftEveVector:
			{
				MonAftEveFrequencyVector *v3=dynamic_cast<MonAftEveFrequencyVector *>(*it);
				FrequencyVectorManager::Getinstance()->addFVector(v3);
				break;
			}
			case FrequencyVectorLevel::weekVector:
			{
				WeekFrequencyVector *v4=dynamic_cast<WeekFrequencyVector *>(*it);
				FrequencyVectorManager::Getinstance()->addFVector(v4);
				break;
			}
			case FrequencyVectorLevel::monthVector:
			{
				MonthFrequencyVector *v5=dynamic_cast<MonthFrequencyVector *>(*it);
				FrequencyVectorManager::Getinstance()->addFVector(v5);
				break;
			}
			default:
				break;
			}
		}
	}

	/**
	* 将频率向量从计时器中移除
	*/
	void Neighbour::removeTimeCount()
	{
		cout<<"remove Frequency"<<endl;
		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
			it!=vectorlist.end();++it)
		{
			FrequencyVector *v=*it;
			TimeManager::GetInstance()->removeVectorListen(v);
			(*it)->resetChangeFVectorSign();//更新频率向量的修改标志位
		}
	}

	/**
	 * 获取与历史邻居相遇的各个级别频率向量
	*/
	FrequencyVector * Neighbour::getFrequencyVector(int level)
	{
		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
			it!=vectorlist.end();++it)
		{
			if(level == (*it)->vectorLevel)
				return (*it);
		}

	}



	/**
	 * 获取该邻居的EID
	 * @return
	 */
	EndpointID Neighbour::getEid()
	{
		return neighbourEid;
	}

	/**
	 * 输出保存的区域的星期
	 */
	void Neighbour::printAreaInfo()
	{
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"打印邻居（%s）的区域移动规律：",neighbourEid.str().c_str());
		for(map<string,Area>::iterator it=neighbourArea->areaMap.begin();
				it!=neighbourArea->areaMap.end();++it)
		{
			Area a=it->second;
			string s;
			char c[20];
			s.append(tag.c_str());
			s.append(":层次为");
			sprintf(c,"%d",a.level);
			s.append(c);
			s.append("的区域");
			sprintf(c,"%d",a.id);
			s.append(c);
			s.append("的移动规律:\n");
			s.append(a.toString());
			//cout<<s<<endl;
			//geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s",s.c_str());
		}
	}

	string Neighbour::toString()
	{
		//if(neighbourEid!=null)
		//{
		string s;
		s.append("neighbour_eid:");
		s.append(neighbourEid.str());
		s.append(",\n");
		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
				it!=vectorlist.end();++it)
		{
			switch((*it)->vectorLevel)
			{
			case FrequencyVectorLevel::minuteVector:
				s.append("minuteVector:");
				break;
			case FrequencyVectorLevel::hourVector:
				s.append("hourVector:");
				break;
			case FrequencyVectorLevel::monAftEveVector:
				s.append("monafteveVector:");
				break;
			case FrequencyVectorLevel::weekVector:
				s.append("weekVector:");
				break;
			case FrequencyVectorLevel::monthVector:
				s.append("monthVector:");
				break;
			}
			s.append((*it)->toString());
			s.append(",\n");
		}
		return s;
		//}
		//else
		//return "neighbour_eid:null";
	}
}
