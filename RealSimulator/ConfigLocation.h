#include<string>
#include"ParseConfigFile.h"
#include"NodeInfo.h"
#include<sys/time.h>

using namespace std;
#define MAXLINE 256

class ConfigLocation {


private:
	static const string configPath;
	static ConfigLocation *instance;
	static const string TAG;
	ConfigLocation()
	{
		eof = false;
		lastLoc = new ParseConfigFile();
		curLoc = new ParseConfigFile();
		fr.open(ConfigLocation::configPath.c_str(),ios::in);
	}

public:
	ParseConfigFile *lastLoc;
	ParseConfigFile *curLoc;
	bool eof;

	///*特殊处理时间以匹配配置文件的时间
	long long sub ;

	//要读取的配置文件及读取数据的缓存区
	fstream fr;
	char buffer[MAXLINE];

	//单例模式

    static ConfigLocation* GetInstance()
    {
        if(instance == NULL)  //判断是否第一次调用
        	instance = new ConfigLocation();

        return instance ;
    }

	//切记代码结束时要调用关闭连接代码，在哪调用呢？确定一下
	void delConfigLocation()
	{
		//设置instance为null，以便在此调用时可以重新new一耳光
		instance = NULL;
		//要不要先判断下是否已到文件末被关闭了？
		fr.close();

	}
	// querry current time
	long long getCurrentTime()
	{
	   struct timeval tv;
	   gettimeofday(&tv,NULL);
	   long long t=(long long)tv.tv_sec * 1000+(int)(tv.tv_usec/1000);
	   return t;
	}

	ParseConfigFile *getFirstLocation()
	{


		string curLine;

		//节点编号
		NodeInfo *node = NodeInfo::GetInstance();

		//配置文件的第一行，为模拟器的第一个时间
		// 若请求时间恰好为模拟器开始时间或者小于模拟器的开始时间
		 // 设置节点位置为此位置
		 //

		//按行读取文件内容


		long long time=getCurrentTime();
		//cout<<"query time = "<<time<<endl;
		//构造一个FileReader和BUfferedReader的对象
	    if (! fr.is_open())
	    {
	   	 	cout<< "Error opening file";
	    }

		///*
		//最好能使用同步算法将其同步为配置文件的开始时间，否则需要特殊处理配置文件的时间或系统时间

		//*/
	    while(!fr.eof() )//当curLine==null时，说明已到文件末，设置节点位置为配置文件里相关的最后一个位置。
	    {
	    	fr.getline (buffer,MAXLINE);
	    	//解析配置文件的一行数据
	    	curLine=buffer;
			ParseConfigFile *pcf=new ParseConfigFile(curLine);
				//配置文件某行出错
				if(pcf->gotTime == -1){
				cout<<"Wrong Config Line";
					continue;
				}

				//寻找本节点的第一个位置
				if(pcf->nodeNo == node->nodeNo)
				{
					//上次位置设置为当前询问时间及第一次询问到的位置
					lastLoc->copy(pcf);
					lastLoc->gotTime = time;
					curLoc->copy(pcf);
				//	cout<<curLine;
					///*
					this->sub =time- pcf->gotTime;
					cout<<"getFirstConfigLocation"<<endl;
					cout<<"当前请求时间："<<time<<
							"  轨迹文件时间："<<pcf->gotTime
							<<"  时间间隔："<<this->sub<<endl;

					return curLoc;
				}
	    }

		return NULL;
	}

	ParseConfigFile *getConfigLocation()
	{
		//请求时间
		long long queryTime=getCurrentTime();
		cout<<"当前系统时间： "<<queryTime<<endl;

		///*特殊处理配置时间
		queryTime = queryTime - this->sub;
		cout<<"轨迹文件中对应的时间: "<<queryTime<<endl;
		//*/
		if(this->eof)//文件已到文件末
		{
			this->curLoc->gotTime = queryTime;
			//cout<<TAG<<"经纬度："<<curLoc->latitude << " " <<curLoc->longitude<<endl;
			return curLoc;
		}


		string curLine;

		//节点编号
		NodeInfo *node = NodeInfo::GetInstance();

		//从上次读取到的部分继续读取下一次的位置
		//先判断前两次位置的时间与当前请求时间的关系
		ParseConfigFile *curPcf = new ParseConfigFile();

		if(queryTime < curLoc->gotTime)//节点未运动到上次所读取到的时间点，匀速计算位置
		{
			curPcf = calLocation(queryTime);
			cout<<"queryTime < curLoc.gotTime";
		}
		else if(queryTime == curLoc->gotTime)//节点恰好运动到上次所读位置
		{
			curPcf->copy(curLoc);
			cout<<"queryTime == curLoc.gotTime";
		}
		else//节点已过上次所读位置，继续读取文件以获得新位置
		{
			//curLine = buf.readLine();
			lastLoc->copy(curLoc);
		//	cout<<"queryTime > curLoc.gotTime";
			//按行读取文件内容
		    	fr.getline (buffer,MAXLINE);
		    	curLine=buffer;

				while(!fr.eof()) //当curLine==null时，说明已到文件末，设置节点位置为配置文件里相关的最后一个位置。
				{
				      ParseConfigFile *pcf = new ParseConfigFile(curLine);
				      //非本节点数据直接跳过，读取下一行
				      if(pcf->nodeNo != node->nodeNo)
				      {
				    	  fr.getline (buffer,MAXLINE);
				    	  curLine=buffer;
				    	  continue;
				      }

				      if(queryTime == pcf->gotTime)
				      {
				    	  curLoc->copy(pcf);
				    	//  cout<<TAG<<"经纬度："<< curLoc->latitude << " " << curLoc->longitude<<endl;
				    	//  cout<<"in cl  " << curLoc->gotTime << "  " <<curLoc->latitude << "  " << curLoc->longitude<<endl;
				    	  return curLoc;
				      }
				      else if(queryTime < pcf->gotTime)//计算位置
				      {
				    	  curLoc->copy(pcf);
				    	  curPcf = calLocation(queryTime);
				    	  //跳出读取信息循环
				    	  break;
				      }
				      else if(queryTime > pcf->gotTime)
				      {
				    	  lastLoc->copy(pcf);
				      }
				      fr.getline (buffer,MAXLINE);
				      curLine=buffer;
				}
				if(fr.eof())
				{
					//读取到文件末，设置节点位置为配置文件里相关的最后一个位置
					this->eof = true;
					curLoc->gotTime = queryTime;
					//curPcf->copy(curLoc);
					//关闭连接
						fr.close();
					return curLoc;
				}


			//关闭对象连接
			/*
			 finally{

			    try {
			      buf.close();
			      fr.close();
			    }
			    catch (IOException ex2) {
			      ex2.printStackTrace();
			    }
			}
			*/

		}

		cout<<TAG<<": 请求时间/经纬度："<<curPcf->gotTime <<" "<<curPcf->latitude << " " << curPcf->longitude<<endl;
		return curPcf;
	}

	ParseConfigFile *calLocation(long long queryTime)
	{
		double ratio = (queryTime - this->lastLoc->gotTime)*1.0/(this->curLoc->gotTime - this->lastLoc->gotTime) ;
		//按比例计算新的纬度
		double latitude = lastLoc->latitude + (curLoc->latitude - lastLoc->latitude)*ratio;
		//按比例计算新的经度
		double longitude = lastLoc->longitude + (curLoc->longitude - lastLoc->longitude)*ratio;

		ParseConfigFile *pcf = new ParseConfigFile(queryTime,latitude,longitude,curLoc->nodeNo);

		return pcf;
	}

};

const string ConfigLocation::configPath="./track.txt";
const string ConfigLocation::TAG="ConfigLocation";
ConfigLocation *ConfigLocation::instance=NULL;

