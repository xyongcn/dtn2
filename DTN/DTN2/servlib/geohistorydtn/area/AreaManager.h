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
		//geohistoryLog=GeohistoryLog::GetInstance();
		pthread_mutex_init(&lockHistoryAreaMoving,NULL);
		//需要通过每次使用前使用init
//		init();
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

	void init()
	{
	    std::fstream ifs;
	    ifs.open(historyAreaFilePath.c_str(),ios::in);
		//从文件中读取历史的区域信息
		if (!ifs.is_open())
	    {
	   	 	cout<< "Error opening file";
	   	 	return;
	    }
	    boost::archive::text_iarchive ia(ifs);

		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:从文件historyarea中读取历史的区域信息",tag.c_str());
		Area area;
	  //  try
	  //  {
	    	while(!ifs.eof())
	    	{

	    		//将读取到的area保存起来
	    		ia >>area;
	    		string s=area.level+"#"+area.id;
	    		areaMap[s]= area;
	    		//将读取到的area的频率向量加入到频率向量管理器中
	    		addAreaFVector2Manager(area);
			}

		//}
	  // catch(boost::archive::archive_exception &e)
	  // {}
		ifs.close();
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:从初始文件中获得本地移动的移动规律，并打印出来：",tag.c_str());
		printAllAreaMoving();
	}






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
				;it!=area.vectorlist.end();++it){
			FrequencyVector *v=*it;
			FrequencyVectorManager::Getinstance()->addFVector(v);
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

	//将内存中的area相关信息写到sdcard中
//	FileOutputStream geohistoryarea=null;
	void wrieteAreaInfoToFile()
	{

		lockHistoryAreaMovingFile();
		fstream ofs;
		ofs.open(historyAreaFilePath.c_str(),ios::out);
		boost::archive::text_oarchive oa(ofs);
		for(hash_map<string,Area>::iterator it=areaMap.begin();
				it!=areaMap.end();++it)
		{
			Area a=it->second;
			oa << a;
		}
		ofs.close();
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:更新完本地的移动记录文件",tag.c_str());
		printAllAreaMoving();


	}


	/**
	 * 将移动轨迹的area保存到log中
	 * @param reason 原因前缀
	 * @param area 新移动的区域
	 */
	void writeAreaLogToFile(string reason,Area area)
	{
		FILE * fr;
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
		string fatherid=0;

		temp.append(reason);
		temp.append(":\n");
		do{
			temp.append(area.toString());
			temp.append("\n");
			fatherid=area.father;
			if(fatherid.empty())
				break;
			else
			{
				hash_map<string,Area>::iterator it=areaMap.find(fatherid);
				area=it->second;
			}
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
	static void writeAreaTimeChange2Log(string time)
	{
		FILE * fr;
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

		fseek(fr, 0, SEEK_END);
		int h=fwrite(time.c_str(),sizeof(char),strlen(time.c_str()),fr);
		fflush(fr);

		if(EOF == fclose(fr))
		{
			printf("err:fclose failed\n");
			return ;
		}

	}

	Area *lookforArea(string areakey)
	{
		hash_map<string,Area>::iterator it=AreaManager::Getinstance()->areaMap.find(areakey);
		Area *area=&(it->second);
		return area;
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
		int i=1;
		for(hash_map<string,Area>::iterator it=areaMap.begin();
				it!=areaMap.end();++it)
		{
			Area a=it->second;
			char i_temp[5];
			sprintf(i_temp,"%d",i);
			++i;
			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:第%d个区域的移动规律：\n%s",tag.c_str(),i_temp,a.toString().c_str());
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
