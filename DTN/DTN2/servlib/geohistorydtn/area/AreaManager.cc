#include "AreaManager.h"
#include<errno.h>

//using namespace dtn;

namespace dtn
{
	const string AreaManager::tag="AreaManager";
	//const string AreaManager::historyAreaFilePath="/home/gaorui/workspace/DTN/DTN2/logDocuments/historyarea.txt";
	//const string AreaManager::historyAreaMovingFilePath="/home/gaorui/workspace/DTN/DTN2/logDocuments/areamoving.txt";

	const string AreaManager::historyAreaFilePath="logDocuments/historyarea.txt";
	const string AreaManager::historyAreaMovingFilePath="logDocuments/areamoving.txt";

	AreaManager * AreaManager::instance=NULL;
	void AreaManager::init()
	{
		    std::fstream ifs;
		    char *buffer;
		    //也可以将buffer作为输出参数
		   /* if((buffer = getcwd(NULL, 0)) == NULL)
		    {
		        perror("getcwd error");
		    }
		    else
		    {
		        printf("%s\n", buffer);
		        free(buffer);
		    }*/

		    ifs.open(historyAreaFilePath.c_str(),ios::in);
			//从文件中读取历史的区域信息
			if (!ifs.is_open())
		    {
				//printf("errno=%d\n",errno);
				//char *message=strerror(errno);
				//printf("Mesg:%s\n",message);
		   	 	cout<< "historyarea.txt does not exist\n";
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
		   cout<<"__________________________________________"<<endl;
		   hash_map<string,Area>::iterator it=areaMap.begin();
		   cout<<"从historyarea.txt所读取的区域信息的区域id:"<<endl;
		   for(;it!=areaMap.end();++it)
		   {
			   Area a=it->second;
			   cout<<a.id<<" ";
		   }
		   cout<<endl<<"__________________________________________"<<endl;


			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s:从初始文件中获得本地移动的移动规律，并打印出来：",tag.c_str());
			//printAllAreaMoving();
		}

}
