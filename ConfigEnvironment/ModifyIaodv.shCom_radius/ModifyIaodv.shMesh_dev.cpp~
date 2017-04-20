#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

#define MAXIMUM 256
#define MAX 5000
#define OFFSET 16

int main(int argc,char *argv[])
{
	printf("../../iaodv/aodv.sh关于MESH_DEV参数的修改\n");
	char *part1=new char[MAX];
	char *part2=new char[MAX];
	char *buffer=new char[MAXIMUM];
	fstream fa;
	fa.open("../../iaodv/aodv.sh",ios::in | ios::out);
	if(!fa)
	{
		printf("../../iaodv/aodv.sh is not exist,error\n");
	    delete [] part1;
	    delete [] part2;
	    delete [] buffer;
	    return 0;
	}
	//const char *d = " \t";
	//char *strArray;
	int location_last=0;
	int location=0;
	string ori;

	while(!fa.eof() )
	{
	    fa.getline (buffer,MAXIMUM);
		location=fa.tellg();
	    if(buffer!=NULL && !strncmp(buffer,"MESH_DEV",8))
	    {
	    	ori=buffer;
	    	break;
	    }
	    location_last=location;
	}

    printf("Original version: %s\n",ori.c_str());
    fa.seekg(0,ios::beg);
    fa.read(part1,location_last);
    fa.seekg(0,ios::end);
    int end=fa.tellg();
    location--;
    fa.seekg(location,ios::beg);
    fa.read(part2,end-location);
    fa.close();
    string s;

    s+="MESH_DEV=\"mesh_dev=";

    if(argc!=1)
    {
	s+=*(++argv);
	for(int i=2;i<argc;++i)
	{
		s+="/"; 
		s+=*(++argv);
	}
     }
    s+="\"";

    printf("Modified version: %s\n",s.c_str());
    for(int i=0;i<s.size();++i)
     	part1[location_last+i]=s[i];
 	fa.open("../../iaodv/aodv.sh",ios::out | ios::trunc);
 	fa.write(part1,location_last+s.size());
 	fa.close();

 	fa.open("../../iaodv/aodv.sh",ios::out | ios::app);
 	fa.write(part2,end-location);
    fa.close();
    delete [] part1;
    delete [] part2;
    delete [] buffer;
    return 0;
}

