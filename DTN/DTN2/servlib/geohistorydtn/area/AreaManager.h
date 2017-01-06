#include<string>

#include<vector>
#include <fstream>
#include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <hash_map>


#include "Area.h"


//#include "geohistorydtn/Log/Geohistorog.h"
using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
    template<> struct hash<const string>
    {
        size_t operator()(const string& s) const
        { return hash<const char*>()( s.c_str() ); } //_stl_hash_string
    };
    template<> struct hash<string>
    {
        size_t operator()(const string& s) const
        { return hash<const char*>()( s.c_str() ); }
    };
}


/**
 * 说明 ：对所有的Area进行管理，分配和超出容量删除机制
 */
namespace dtn{


class AreaManager
{
private:

	static const string tag;
public:
	static const string historyAreaFilePath;
	static const string historyAreaMovingFilePath;

	pthread_mutex_t lockHistoryAreaMoving;
	/**
	 * 用来存放所有的Area的hashmap
	 */
	hash_map<string,Area> areaMap;

	/**
	 * 默认构造函数
	 */
	AreaManager()
	{
		geohistoryLog=GeohistoryLog::GetInstance();
		pthread_mutex_init(&lockHistoryAreaMoving,NULL);
		//需要通过每次使用前使用init
	//	init();
	}

public:
	GeohistoryLog *geohistoryLog;
	static AreaManager *instance;

	static AreaManager* Getinstance()
	{
		if(instance == NULL)  //判断是否第一次调用
		     instance = new AreaManager;
		return instance ;
	}

	void init();//在dtnd.cc中初始化，用于将原来的历史移动规律读入该程序






	//锁住自身移动规律的修改，防止在修改移动规律的时候被读取
	void lockHistoryAreaMovingFile(){
		pthread_mutex_lock(&lockHistoryAreaMoving);
	}

	//解锁自身移动规律文件的修改
	void unlockHistoryAreaMovingFile(){
		pthread_mutex_unlock(&lockHistoryAreaMoving);
	}

	/**
	 * 将area中的所有频率向量添加到频率向量管理器中，这样从文件中读取区域时，频率向量的衰减器也可以对其进行频率的衰减了
	 * @param area
	 */
	void addAreaFVector2Manager(Area area){
		for(list<FrequencyVector *>::iterator it=area.vectorlist.begin()
				;it!=area.vectorlist.end();++it)
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
	 * 检查这批新的区域并且修改相应的父类引用
	 * @param areainfo:所在各个层次信息
	 * @return :最底层的区域的引用，如果区域信息不合法会返回Null
	 */
	Area *checkAreaInfo(AreaInfo areainfo)
	{

		vector<int> areaid(areainfo.getAreaId());
		int start=1;
		int end=areaid.size();
	//	vector<Area> arealist;

		bool valid=true;
		int level=1;
		string father;
		Area *baseArea=NULL;
		for(int i=start;i<end;i++)
		{
			int id=areaid[i];
			if(AreaLevel::isLevelValid(level))
			{
				//生成hashmap的标识字符串
				char level_c[10];
				char id_c[10];
				sprintf(level_c,"%d",level);
				sprintf(id_c,"%d",id);
				string s;
				s.append(level_c);
				s.append("#");
				s.append(id_c);
				hash_map<string,Area>::iterator it=areaMap.find(s);
				if(it==areaMap.end())
				{
					Area a(level,id,father);
					areaMap[s]=a;
					hash_map<string,Area>::iterator p=areaMap.find(s);
					//生成每个区域的频率向量
					p->second.init();
					father=s;
					baseArea=&(p->second);
				}
				else
				{
					baseArea=&(it->second);
					father=s;
				}

			}
			//如果存在不合格的level，那么就不需要
			else
			{
				geohistoryLog->LogAppend(geohistoryLog->ERROR_LEVEL,"%s:areainfo的lever不合法",tag.c_str());
				valid=false;
				break;
			}

			//将区域层级+1
			++level;
		}
		return baseArea;

	}
	/*Area *checkAreaInfo_modify(AreaInfo areainfo)
	{
		vector<int> areaid(areainfo.getAreaId());
		int start=1;
		int end=areaid.size();
	//	vector<Area> arealist;

		bool valid=true;
		int level=1;
		string father;
		Area *baseArea=NULL;
		Area *fatherArea=NULL;
		for(int i=start;i<end;i++)
		{
			int id=areaid[i];
			if(AreaLevel::isLevelValid(level))
			{
				//生成hashmap的标识字符串
				char level_c[10];
				char id_c[10];
				sprintf(level_c,"%d",level);
				sprintf(id_c,"%d",id);
				string s;
				s.append(level_c);
				s.append("#");
				s.append(id_c);
				hash_map<string,Area>::iterator it=areaMap.find(s);
				if(it==areaMap.end())
				{
					Area a(level,id,father,fatherArea);
					areaMap[s]=a;
					hash_map<string,Area>::iterator p=areaMap.find(s);
					//生成每个区域的频率向量
					p->second.init();
					father=s;
					fatherArea=&(p->second);
					baseArea=&(p->second);
				}
				else
				{
					fatherArea=&(it->second);
					baseArea=&(it->second);
					father=s;
				}

			}
			//如果存在不合格的level，那么就不需要
			else
			{
				geohistoryLog->LogAppend(geohistoryLog->ERROR_LEVEL,"%s:areainfo的lever不合法",tag.c_str());
				valid=false;
				break;
			}

			//将区域层级+1
			++level;
		}
		return baseArea;
	}*/

	//写入内存中的area相关信息,写入时删除原有的内容
	void wrieteAreaInfoToFile()
	{

		lockHistoryAreaMovingFile();
		//cout<<"minute:wrieteAreaInfoToFile"<<endl;
		fstream ofs;
		ofs.open(historyAreaFilePath.c_str(),ios::trunc|ios::out);
		boost::archive::text_oarchive oa(ofs);
		for(hash_map<string,Area>::iterator it=areaMap.begin();
				it!=areaMap.end();++it)
		{
			Area a=it->second;
			oa << a;
		}
		ofs.close();
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:更新完本地的移动记录文件",tag.c_str());
	//	printAllAreaMoving();

		unlockHistoryAreaMovingFile();
	}


	/**
	 * 将移动轨迹的area保存到log中
	 * @param reason 原因前缀
	 * @param area 新移动的区域
	 */
	void writeAreaLogToFile(string reason,Area *area,AreaInfo areainfo)
	{
		//cout<<"writeAreaLogToFile"<<endl;
		FILE * fr;
		errno=0;
		fr= fopen(historyAreaMovingFilePath.c_str(),"a");
		if (NULL == fr)
		{
			if (EINVAL == errno)
			{
				printf("err:fopen log file %s failed\n",historyAreaMovingFilePath.c_str());
			}
			else
			{
				printf("err:unknow\n");
			}
		}
		string temp;
		string father;

		temp.append(reason);
		temp.append(":\n");
		char time_c[10];
		sprintf(time_c,"%d",areainfo.month+1);
		temp.append(time_c);
		temp.append("月,");
		if(areainfo.week==0)
			temp.append("星期天,");
		else
		{
			sprintf(time_c,"%d",areainfo.week);
			temp.append("星期");
			temp.append(time_c);
			temp.append(",");
		}
		if(areainfo.monafteve==1)
			temp.append("上午,");
		else if(areainfo.monafteve==2)
			temp.append("下午,");
		else
			temp.append("晚上,");

		sprintf(time_c,"%d",areainfo.hour);

		temp.append(time_c);
		temp.append("点");
		sprintf(time_c,"%d",areainfo.minute);
		temp.append(time_c);
		temp.append("分钟\n");

		do{
			temp.append(area->toString());
			temp.append("\n");
			father=area->father;
			if(!father.empty())
				area=AreaManager::Getinstance()->lookforArea(father);
			else
				break;
		}while(true);
		fseek(fr, 0, SEEK_END);
		int h=fwrite(temp.c_str(),sizeof(char),strlen(temp.c_str()),fr);
		fflush(fr);

		if(EOF == fclose(fr))
		{
			printf("err:fclose failed\n");
			return ;
		}

	}
	/**
	 * 计时器时间发生变化将写入日志中
	 * @param time
	 */
	/*static void writeAreaTimeChange2Log(string time)
	{
		FILE * fr;
		errno = 0;
		fr= fopen(historyAreaMovingFilePath.c_str(),"a");
		//name="./logDocuments/GeohistoryLog.txt";

		if (NULL == fr)
		{
			if (EINVAL == errno)
			{
				printf("err:fopen log file %s failed\n",historyAreaMovingFilePath.c_str());
			}
			else
			{
				printf("err:unknow\n");
			}
		}

		fseek(fr, 0, SEEK_END);
		int h=fwrite(time.c_str(),sizeof(char),strlen(time.c_str()),fr);
		fflush(fr);

		if(EOF == fclose(fr))
		{
			printf("err:fclose failed\n");
			return ;
		}

	}*/

	Area *lookforArea(string areakey)
	{
		hash_map<string,Area>::iterator it=AreaManager::Getinstance()->areaMap.find(areakey);
		if(it!=AreaManager::Getinstance()->areaMap.end())
		{
			Area *area=&(it->second);
			return area;
		}
		else
			return NULL;
	}

	/**
	 * 清空已经有的区域信息
	 */
	void shutdown()
	{
		//测试验证结果正确性,在结束的时候将本节点的所有的区域移动规律打印出来
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:退出AreaManager时，打印本节点的移动规律",tag.c_str());
		printAllAreaMoving();
		areaMap.clear();
	}

	void printAllAreaMoving()
	{
		for(hash_map<string,Area>::iterator it=areaMap.begin();
				it!=areaMap.end();++it)
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
			cout<<s<<endl;
		//	geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s",s.c_str());
		}
	}

	//获取位置以及改变位置的流程
	//1.首先由专门获取位置的程序负责获取当前的位置，然后再通过相关程序得到相应的区域编号；一种模拟环境中是直接得到相应的区域编号。
	//2.调用GeoHistoryRouter里面的changeLocation,
	//3.由GeoHistroyRouter来判断是否进行改变区域的事件处理以及调用AreaManager里面对新区域的处理
	//
	//
};
//const string AreaManager::tag="AreaManager";
//const string AreaManager::historyAreaFilePath="/home/gaorui/workspace/DTN/historyarea.obj";
//const string AreaManager::historyAreaMovingFilePath="/home/gaorui/workspace/DTN/areamoving.txt";

//AreaManager * AreaManager::instance=NULL;

}
