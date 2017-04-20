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
	frre.open("./way.xml",ios::in | ios::out | ios::trunc);
	string s="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<osm version=\"0.6\" generator=\"CGImap 0.4.3 (24414 thorn-04.openstreetmap.org)\" copyright=\"OpenStreetMap and contributors\"\nattribution=\"http://www.openstreetmap.org/copyright\" license=\"http://opendatacommons.org/licenses/odbl/1-0/\">\n";

	fror.open("./test.edg.xml",ios::in);
	if(!fror)
	{
		printf("./test.edg.xml is not exist,error\n");
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
	char *fnode_temp=new char[30];
	char *tnode_temp=new char[30];
	char *id;
	char *fnode;
	char *tnode;
	const char *d = " \t";
	const char *t="=";
	char *strArray;
	while(!fror.eof())
	{
		//处理.nod.xml文件
		fror.getline(line,MAXIMUM);
		strArray=strtok(line,d);
		if(strArray!=NULL && !strcmp(strArray,"<edge"))
		{
			while(strArray!=NULL)
			{
				if(!strncmp(strArray,"id",2))
					strcpy(id_temp,strArray);
				if(!strncmp(strArray,"from",4))
					strcpy(fnode_temp,strArray);
				if(!strncmp(strArray,"to",2))
					strcpy(tnode_temp,strArray);
				strArray=strtok(NULL,d);
			}
			id=strtok(id_temp,t);
			id=strtok(NULL,t);
			fnode=strtok(fnode_temp,t);
			fnode=strtok(NULL,t);
			tnode=strtok(tnode_temp,t);
			tnode=strtok(NULL,t);
			printf("%s,%s,%s\n",id,fnode,tnode);

			////写入node.xml文件
			s.clear();
			s.append(" <way id=");
			s.append(id);
			s.append(" visible=\"true\" version=\"2\" changeset=\"35585225\" timestamp=\"2015-11-26T06:45:20Z\" user=\"katpatuka\" uid=\"17497\">\n");
			s.append("  <nd ref=");
			s.append(fnode);
			s.append("/>\n");
			s.append("  <nd ref=");
			s.append(tnode);
			s.append("/>\n");
			s.append("  <tag k=\"admin_level\" v=\"1\"/>\n  <tag k=\"boundary\" v=\"administrative\"/>\n </way>\n");
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

