
/**
 *
 *
 * 此类用于帮助解析配置文件的各行信息，各列信息
 * 主要为了与工具生成的配置文件信息兼容
 * 工具生成的配置文件包含信息：时间，纬度，经度，速度，角度（当节点无位移时，速度和角度均为nam）
 * 在本项目中添加了节点编号信息，为第六位
 * 在本项目中速度和角度没有用处，直接设置为0
 *
 */
#include<string>
#include<string.h>
#include <iostream>
#include<stdlib.h>

using namespace std;

class ParseConfigFile {
public:
	long long gotTime;
	double latitude;
	double longitude;
	double speed;
	double angle;
	int nodeNo;

	ParseConfigFile()
	{
		 gotTime = -1;
		 latitude = 0;
		 longitude = 0;
		 speed = 0;
		 angle = 0;
	}

	ParseConfigFile(long long gotTime,double latitude,double longitude,int nodeNo)
	{
		this->gotTime = gotTime;
		this->latitude = latitude;
		this->longitude = longitude;
		this->nodeNo = nodeNo;
		this->speed = 0;
		this->angle = 0;
	}

	ParseConfigFile(ParseConfigFile &pcf)
	{
		this->gotTime = pcf.gotTime;
		this->latitude = pcf.latitude;
		this->longitude = pcf.longitude;
		this->nodeNo = pcf.nodeNo;
		this->speed = pcf.speed;
		this->angle = pcf.angle;
	}

	ParseConfigFile(string line)
	{
			char line_temp[100];

			const char *d = " \t";

			strcpy(line_temp,line.c_str());
		    char *numArray=strtok(line_temp,d);

			int num=0;
		    while(numArray)
		    {

		    	numArray=strtok(NULL,d);
		        num++;
		    }

			strcpy(line_temp,line.c_str());
		    char *strArray=strtok(line_temp,d);

		    if(num == 6)//config中的每一个行可被分割为六个元素，否则出错
			{
		        this->nodeNo = atoi(strArray);
		        strArray=strtok(NULL,d);

				this->gotTime = atoll(strArray);
				strArray=strtok(NULL,d);
				this->longitude =atof(strArray);
				strArray=strtok(NULL,d);
				this->latitude= atof(strArray);

				strArray=strtok(NULL,d);
				char temp1[10];
				strncpy(temp1,strArray,3);
				temp1[3]='\0';
				if(!strcmp(temp1,"nan"))//表示无速度
					this->speed = 0;
				else
					this->speed = atof(strArray);

				strArray=strtok(NULL,d);
				char temp2[10];
				strncpy(temp2,strArray,3);
				temp2[3]='\0';
				if(!strcmp(temp2,"nan"))//表示无速度
					this->angle = 0;
				else
					this->angle = atof(strArray);

			}
			else
			{
				cout<<"RS Error"<<"ParseConfigFile Error,End of File";
			}
		}

		 void copy(ParseConfigFile *pcf)
		{
			this->gotTime = pcf->gotTime;
			this->latitude = pcf->latitude;
			this->longitude = pcf->longitude;
			this->nodeNo = pcf->nodeNo;
			this->speed = pcf->speed;
			this->angle = pcf->angle;
		}

};
