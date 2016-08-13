#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#define MAXLINE 256
//解析配置文件，存到NodeInfo类中
class NodeInfo {
		//节点属性文件
private:
		static const string nodeInfoPath;
		static const string TAG;
		static NodeInfo *Instance;

		//单例模式

		NodeInfo()   //构造函数是私有的
		{
			ifstream fr(nodeInfoPath.c_str());
			char buffer[MAXLINE];
		    if (! fr.is_open())
		    {
		   	 	cout<< "Error opening file";
		    }
		    while(!fr.eof() )
		    {
		    	fr.getline (buffer,MAXLINE);
		    	char *curLine=buffer;
		    	char *attrArray=strtok(curLine,":");

		    	while (attrArray != NULL)
		    	{
		    		if(!strcmp(attrArray,"nodeNo"))
		    		{
		    			attrArray= strtok (NULL, ":");
		    			this->nodeNo=atoi(attrArray);
		    		}
		    		else if(!strcmp(attrArray,"com_Dis"))
		    		{
		    			attrArray= strtok (NULL, ":");
		    			this->com_Dis=atof(attrArray);
		    		}
		    		else if(!strcmp(attrArray,"Mode"))
		    		{
		    			attrArray= strtok (NULL, ":");
		    			char temp[10];
		    			strncpy(temp,attrArray,3);
		    			temp[3]='\0';
		    			if(!strcmp(temp,"Con"))
		    				this->Mode=NodeInfo::Config_Mode;
		    			else if(!strcmp(temp,"GPS"))
		    				this->Mode=NodeInfo::GPS_Mode;
		    			else
		    				this->Mode=0;
		    		}
		    		else if(!strcmp(attrArray,"defualtLatitude"))
		    		{
		    			attrArray= strtok (NULL, ":");
		    			this->defualtLatitude=atof(attrArray);
		    		}
		    		else if(!strcmp(attrArray,"defualtLongitude"))
		    		{
		    			attrArray= strtok (NULL, ":");
		    			this->defualtLongitude=atof(attrArray);
		    		}
		    		else if(!strcmp(attrArray,"startTime"))
		    		{
		    			attrArray= strtok (NULL, ":");
			 		    this->startTime=atoll(attrArray);
		    		}
		    		else if(!strcmp(attrArray,"endTime"))
		    		{
		    			attrArray= strtok (NULL, ":");
		    			this->endTime=atoll(attrArray);
		    		}

		    		attrArray= strtok (NULL, ":");
		    	}
		    }
		  /*  cout<<nodeNo<<endl;
		    cout<<com_Dis<<endl;
		    cout<<Mode<<endl;
		    cout<<defualtLatitude<<endl;
		    cout<<defualtLongitude<<endl;
		    cout<<startTime<<endl;
		    cout<<endTime<<endl;*/

		}






public:
		static const int GPS_Mode = 1000;
		static const int Config_Mode = 1001;

		//节点属性
		int Mode;
		int nodeNo;//节点编号
		double defualtLatitude;//默认纬度
		double defualtLongitude;//默认纬度
		long long startTime;//以msec为单位。system.currentTimeMillis
		long long endTime;//结束时间
		double com_Dis;//单位m


		/**
		 * 单例
		 */
	    static NodeInfo* GetInstance()
	    {
	        if(Instance == NULL)  //判断是否第一次调用
	        	Instance = new NodeInfo();


	        return Instance ;
	    }

		void setMode(int Mode)
		{
			this->Mode = Mode;
		}

		void setNode_No(int node_No)
		{
			this->nodeNo = node_No;
		}

		void setStartTime(long long startTime)
		{
			this->startTime = startTime;
		}

		void setendTime(long long endTime)
		{
			this->endTime=endTime;
		}

		void setCom_Dis(double dis)
		{
			this->com_Dis = dis;
		}





};
NodeInfo * NodeInfo::Instance=NULL;
const string NodeInfo::nodeInfoPath="./nodeInfo.txt";
const string NodeInfo::TAG="NodeInfo";

