#include<fstream>
#include<string.h>
#include<iostream>
#include<vector>

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
	frre.open("./area.xml",ios::in | ios::out | ios::trunc);
	string s="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<osm version=\"0.6\" generator=\"CGImap 0.4.3 (6174 thorn-05.openstreetmap.org)\" copyright=\"OpenStreetMap and contributors\"\nattribution=\"http://www.openstreetmap.org/copyright\" license=\"http://opendatacommons.org/licenses/odbl/1-0/\">\n";

	fror.open("./test.area.xml",ios::in);
	if(!fror)
	{
		printf("./test.area.xml is not exist,error\n");
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
	char id_temp[30];
	char node_temp[30];
	char level_temp[30];

	char *id;
	vector<string> way;
	vector<string> relation;
	char *node;
	char *level;

	const char *d = " \t";
	const char *t="=";
	char *strArray;
	while(!fror.eof())
	{
		//处理.nod.xml文件
		fror.getline(line,MAXIMUM);
		strArray=strtok(line,d);
		way.clear();
		relation.clear();
		if(strArray!=NULL && !strcmp(strArray,"<area"))
		{
			while(strArray!=NULL)
			{
				if(!strncmp(strArray,"id",2))
					strcpy(id_temp,strArray);
				if(!strncmp(strArray,"way",3))
				{
					char c[30];
					strcpy(c,strArray);
					cout<<c<<" ";
					way.push_back(c);
				}
				if(!strncmp(strArray,"relation",8))
				{
					char c[30];
					strcpy(c,strArray);
					relation.push_back(c);
				}
				if(!strncmp(strArray,"node",4))
					strcpy(node_temp,strArray);
				if(!strncmp(strArray,"level",5))
					strcpy(level_temp,strArray);
				strArray=strtok(NULL,d);
			}
			id=strtok(id_temp,t);
			id=strtok(NULL,t);
			node=strtok(node_temp,t);
			node=strtok(NULL,t);
			level=strtok(level_temp,t);
			level=strtok(NULL,t);

			printf("%s,%s,%s,%d,%d\n",id,node,level,way.size(),relation.size());

			////写入node.xml文件
			s.clear();
			s.append(" <relation id=");
			s.append(id);
			s.append(" visible=\"true\" version=\"7\" changeset=\"37949537\" timestamp=\"2016-03-20T04:15:21Z\" user=\"FreedSky\" uid=\"1236135\">\n");
			for(vector<string>::iterator it=way.begin();it!=way.end();++it)
			{
				s.append("  <member type=\"way\" ref=");
				const char *c="\"";
				const char *cc="/";

				int len = it->length();
				char *str=new char[len+1];
				it->copy(str,len+1,0);
				char *way1;
				way1=strtok(str,t);
				way1=strtok(NULL,t);
				char *way2;
				way2=strtok(way1,c);
				char *way3;
				way3=strtok(way2,cc);
				s.append("\"");
				s.append(way2);
				s.append("\" role=\"");
				way3=strtok(NULL,cc);
				if(!strncmp(way3,"o",1))
					s.append("outer");
				if(!strncmp(way3,"i",1))
					s.append("inner");
				s.append("\"/>\n");
				delete [] str;
			}
			for(vector<string>::iterator it=relation.begin();it!=relation.end();++it)
			{
				s.append("  <member type=\"relation\" ref=");
				int len = it->length();
				cout<<len<<"  ";
				char *str=new char[len+1];
				it->copy(str,len,0);
				str[len]='\0';
				cout<<str<<" ";
				char *relation1;
				relation1=strtok(str,t);
				relation1=strtok(NULL,t);
				s.append(relation1);
				s.append(" role=\"subarea\"/>\n");
				delete [] str;
			}
			cout<<endl;
			s.append("  <member type=\"node\" ref=");
			s.append(node);
			s.append(" role=\"label\"/>\n");
			s.append("  <tag k=\"admin_level\" v=");
			s.append(level);
			s.append("\n  <tag k=\"alt_name\" v=\"海淀;Haidian\"/>\n  <tag k=\"boundary\" v=\"administrative\"/>\n  <tag k=\"name\" v=\"海淀区\"/>\n  <tag k=\"name:en\" v=\"Haidian District\"/>\n  <tag k=\"type\" v=\"boundary\"/>\n  <tag k=\"wikipedia\" v=\"zh:海淀区\"/>\n </relation>\n");
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

	//delete [] buffer;
	//delete [] line;

    return 0;
}

