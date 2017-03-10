#include<fstream>
#include<string.h>
#include<iostream>
#include<sstream>
using namespace std;

#define MAXIMUM 256
#define MAX 5000
#define OFFSET 16

int main(int argc,char *argv[])
{
	//string ip1="192.168.11.122";
	//string ip2="192.168.1.122";
	//修改/etc/dtn.conf
	printf("/etc/dtn.conf关于discovery announce参数cl_addr的修改\n");
	char *part1=new char[MAX];
	char *part2=new char[MAX];
	char *buffer=new char[MAXIMUM];
	string disAnoun_name;
	string disAnoun_rest;
	string disAnoun_all;
	fstream fr;
	fr.open("/etc/dtn.conf",ios::in | ios::out);
	if(!fr)
	{
		printf("/etc/dtn.conf is not exist,error\n");
	    delete [] part1;
	    delete [] part2;
	    delete [] buffer;
		return 0;
	}
	const char *d = " \t";
	char *strArray;
	int location_last=0;
	int location=0;
	int length;
	int f=0;
    while(!fr.eof() )
    {
    	fr.getline (buffer,MAXIMUM);
	    location=fr.tellg();
    	strArray=strtok(buffer,d);
    	if(f!=0 && (strArray ==NULL || strcmp(strArray,"discovery")))
    			break;
    	if(strArray!=NULL && !strcmp(strArray,"discovery"))
		{
		    strArray=strtok(NULL,d);
			if(!strcmp(strArray,"announce"))
			{
			    disAnoun_all+="discovery announce";
			    strArray=strtok(NULL,d);
			    int flag=0;
			    int flag2=0;

			    while(strArray)
			    {
			    	if(flag==0 && f==0)
			    	{
			    		disAnoun_name=strArray;
			    		flag++;
			    	}
			    	else
			    	{
			    		if(!strncmp(strArray,"cl_addr",7))
			    			flag2++;
			    		if(flag2==0 && f==0)
			    		{
			    			disAnoun_rest+=" ";
			    			disAnoun_rest+=strArray;
			    		}

			    	}
				disAnoun_all+=" ";
			    	disAnoun_all+=strArray;
			    	strArray=strtok(NULL,d);

			    }
			    disAnoun_all+="\n";
			    f=1;
			}
		}
    	if(f==0)
    		location_last=location;
    }
    printf("Original version: %s\n",disAnoun_all.c_str());
    fr.seekg(0,ios::beg);
    fr.read(part1,location_last);
    fr.seekg(0,ios::end);
    int end=fr.tellg();
    location--;
    fr.seekg(location,ios::beg);
    fr.read(part2,end-location);
    fr.close();
    int num=0;
    string s;

    int z=0;
    for(;disAnoun_name[z]!='/0';++z)
    {
    	if(disAnoun_name[z]<='9' && disAnoun_name[z] >= '0')
    		break;
    }
    string real_name;
    real_name.assign(disAnoun_name,0,z);

    argv++;
	if(argc==1)
	{
   	s+="discovery announce ";
    	s+=real_name;
    	stringstream ss;
    	string temp;
    	ss<<num;
    	ss>>temp;
    	s+=temp;
    	s+=disAnoun_rest;
	}
else
{
    for(num=0;num<argc-1;++num)
    {
    	s+="discovery announce ";
    	s+=real_name;
    	stringstream ss;
    	string temp;
    	ss<<num;
    	ss>>temp;
    	s+=temp;
    	s+=disAnoun_rest;
    	s+=" cl_addr=";
    	s+=*argv;
    	s+="\n";
    	argv++;
    }
}
    printf("Modified version: %s\n",s.c_str());
   for(int i=0;i<s.size();++i)
    	part1[location_last+i]=s[i];
	fr.open("/etc/dtn.conf",ios::out | ios::trunc);
	fr.write(part1,location_last+s.size());
	fr.close();

	fr.open("/etc/dtn.conf",ios::out | ios::app);
	fr.write(part2,end-location);
    fr.close();
    delete [] part1;
    delete [] part2;
    delete [] buffer;



    return 0;
}

