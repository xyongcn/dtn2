#include<fstream>
#include<string.h>
#include<iostream>

using namespace std;

//#define MAXIMUM 16384
#define MAXIMUM 1024

int main()
{
	char *buffer=new char[MAXIMUM];
	char *line=new char[MAXIMUM];
	fstream frre;
	fstream fror;
	int location=0;
	frre.open("./node.xml",ios::in | ios::out | ios::trunc);
	string s="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<osm version=\"0.6\" generator=\"CGImap 0.4.3 (24429 thorn-04.openstreetmap.org)\" copyright=\"OpenStreetMap and contributors\" attribution=\"http://www.openstreetmap.org/copyright\" license=\"http://opendatacommons.org/licenses/odbl/1-0/\">\n";

	fror.open("./test.nod.xml",ios::in);
	if(!fror)
	{
		printf("./test.nod.xml is not exist,error\n");
		frre.close();
		fror.close();
	    delete [] buffer;
	    delete [] line;
		return 0;
	}
	strcpy(buffer,s.c_str());
	frre.write(buffer,s.size());
	location=s.size();
	frre.seekg(location,ios::beg);
	char *id_temp=new char[30];
	char *lat_temp=new char[30];
	char *lon_temp=new char[30];
	char *id;
	char *lat;
	char *lon;
	const char *d = " \t";
	const char *t="=";
	char *strArray;
	while(!fror.eof())
	{
		//处理.nod.xml文件
		fror.getline(line,MAXIMUM);
		strArray=strtok(line,d);
		if(strArray!=NULL && !strcmp(strArray,"<node"))
		{
			while(strArray!=NULL)
			{
				if(!strncmp(strArray,"id",2))
					strcpy(id_temp,strArray);
				if(!strncmp(strArray,"x",1))
					strcpy(lon_temp,strArray);
				if(!strncmp(strArray,"y",1))
					strcpy(lat_temp,strArray);
				strArray=strtok(NULL,d);
			}
			id=strtok(id_temp,t);
			id=strtok(NULL,t);
			lat=strtok(lat_temp,t);
			lat=strtok(NULL,t);
			lon=strtok(lon_temp,t);
			lon=strtok(NULL,t);
			printf("%s,%s,%s\n",id,lat,lon);

			////写入node.xml文件
			s.clear();
			s.append(" <node id=");
			s.append(id);
			s.append(" visible=\"true\" version=\"3\" changeset=\"35986026\" timestamp=\"2015-12-16T11:29:05Z\" user=\"FreedSky\" uid=\"1236135\" lat=");
			s.append(lat);
			s.append(" lon=");
			s.append(lon);
			s.append("/>\n");
			strcpy(buffer,s.c_str());
			frre.write(buffer,s.size());
			location+=s.size();
			frre.seekg(location,ios::beg);
		}
	}
	s.clear();
	s.append("</osm>");
	strcpy(buffer,s.c_str());
	frre.write(buffer,s.size());

	frre.close();
	fror.close();
	delete [] buffer;
	delete [] line;
    return 0;
}

