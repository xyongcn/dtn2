#include<fstream>
#include<string.h>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

#define MAXIMUM 256
#define MAX 5000
#define OFFSET 16

int main(int argc,char *argv[])
{
	char *part1=new char[MAX];
	char *part2=new char[MAX];
	char *buffer=new char[MAXIMUM];

	vector<string> partnewname;
	++argv;
	++argv;
	for(int i=2;i<argc;++i)
	{
		if(!strcmp(*argv,"minute") | !strcmp(*argv,"hour") | !strcmp(*argv,"monafteve") 
			|!strcmp(*argv,"month") | !strcmp(*argv,"week") )
	   	{
			partnewname.push_back(*argv);
	   		argv++;
		}
		else
		{
			printf("parameter error!\n");
			return 0;
		}
		
	}

	const char *d = " \t";
	char *strArray;
	int location_last=0;
	int location=0;
	fstream fr;
	fr.open("../DTN/DTN2/mydtn.conf",ios::in | ios::out);
	if(!fr)
	{
		printf("../DTN/DTN2/mydtn.conf is not exist,error\n");
	    delete [] buffer;
		return 0;
	}
    while(!fr.eof() )
    {
    	fr.getline (buffer,MAXIMUM);
	    location=fr.tellg();
    	strArray=strtok(buffer,d);

    	if(strArray!=NULL && !strcmp(strArray,"frequency"))
		{
		    printf("old vector : ");
		    strArray=strtok(NULL,d);
		    strArray=strtok(NULL,d);
		    strArray=strtok(NULL,d);
		    strArray=strtok(NULL,d);
		    printf("%s",strArray);
		    strArray=strtok(NULL,d);
		    while(strArray!=NULL && !strcmp(strArray,"&"))
		    {
			    strArray=strtok(NULL,d);
			    printf(",%s",strArray);
			    strArray=strtok(NULL,d);
		    }

			break;
		}
    	location_last=location;
    }
    fr.seekg(0,ios::beg);
    fr.read(part1,location_last);
    fr.seekg(0,ios::end);
    int end=fr.tellg();
    location--;
    fr.seekg(location,ios::beg);
    fr.read(part2,end-location);
    fr.close();
    string s="frequency vector set type ";

	vector<string>::iterator it=partnewname.begin();
	s+=*it;
	for(++it;it!=partnewname.end();++it)
	{
		s+=" & ";
		s+=*it;
	}
    for(int i=0;i<s.size();++i)
    	part1[location_last+i]=s[i];
	fr.open("../DTN/DTN2/mydtn.conf",ios::out | ios::trunc);
	fr.write(part1,location_last+s.size());
	fr.close();

	fr.open("../DTN/DTN2/mydtn.conf",ios::out | ios::app);
	fr.write(part2,end-location);
    fr.close();
    delete [] part1;
    delete [] part2;
    delete [] buffer;
    return 0;
}
