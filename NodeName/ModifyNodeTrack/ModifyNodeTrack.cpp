#include<fstream>
#include<string.h>
#include<iostream>

using namespace std;

#define MAXIMUM 256
#define MAX 5000
#define OFFSET 16

int main(int argc,char *argv[])
{
//修改/RealSimulator/nodeInfo.txt中的nodeNo
	printf("修改/RealSimulator/nodeInfo.txt中的nodeNo\n");
	char *part3=new char[MAX];
	char *part4=new char[MAX];
	char *buffer2=new char[MAXIMUM];
	fstream fn;
	fn.open("../../RealSimulator/nodeInfo.txt",ios::in | ios::out);
	if(!fn)
	{
		printf("../../RealSimulator/nodeInfo.txt is not exist,error\n");
	    delete [] part3;
	    delete [] part4;
		delete [] buffer2;
		return 0;
	}

	int location2=0;
	fn.getline (buffer2,MAXIMUM);
	printf("old track: %s\n",buffer2);
	location2=fn.tellg();
    fn.seekg(0,ios::end);
    int end2=fn.tellg();
    location2--;
    fn.seekg(location2,ios::beg);
    fn.read(part4,end2-location2);
    fn.close();
    string s2="nodeNo:";
    s2+=*(++argv);
	printf("new track: nodeNo:%s\n",*argv);
    for(int i=0;i<s2.size();++i)
    	part3[i]=s2[i];
	fn.open("../../RealSimulator/nodeInfo.txt",ios::out | ios::trunc);
	fn.write(part3,s2.size());
	fn.close();

	fn.open("../../RealSimulator/nodeInfo.txt",ios::out | ios::app);
	fn.write(part4,end2-location2);
    fn.close();
    delete [] part3;
    delete [] part4;
    delete [] buffer2;
	return 0;
}
