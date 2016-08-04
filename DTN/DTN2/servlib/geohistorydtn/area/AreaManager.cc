#include "AreaManager.h"

//using namespace dtn;

namespace dtn
{
	const string AreaManager::tag="AreaManager";
	const string AreaManager::historyAreaFilePath="./logDocuments/historyarea.txt";
	const string AreaManager::historyAreaMovingFilePath="./logDocuments/areamoving.txt";

	AreaManager * AreaManager::instance=NULL;
	void AreaManager::init()
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
		    try
		    {
		    	while(!ifs.eof())
		    	{

		    		//将读取到的area保存起来
		    		ia >>area;
		    		char c[20];
		    		string s;
		    		sprintf(c,"%d",area.level);
		    		s.append(c);
		    		s.append("#");
		    		sprintf(c,"%d",area.id);
		    		s.append(c);
		    		areaMap[s]= area;
		    		//将读取到的area的频率向量加入到频率向量管理器中
		    		addAreaFVector2Manager(area);
				}

			}
		   catch(boost::archive::archive_exception &e)
		   {}
		   ifs.close();
		   cout<<"!!!!!!!!!!!!"<<endl;
		   hash_map<string,Area>::iterator it=areaMap.begin();
		   for(;it!=areaMap.end();++it)
		   {
			   Area a=it->second;
			   cout<<a.id<<endl;
		   }
		   cout<<"~~~~~~~~~~"<<endl;


			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:从初始文件中获得本地移动的移动规律，并打印出来：",tag.c_str());
			printAllAreaMoving();
		}

}
