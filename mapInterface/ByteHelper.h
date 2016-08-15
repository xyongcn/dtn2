#include<string>
#include<iostream>
#include<stdlib.h>
#include"stdlib.h"
#include<stdio.h>
#include<string.h>

using namespace std;
class ByteHelper
{
public:
	static const bool endian_test()
	{
		int x = 1;
		char p = (char)x;
		if(p == 1)
			return false;//little endian
		else
			return true;//big endian

	}

	static const int byte_array_to_int(char* b)
	{
		int a= b[3] & 0xFF;

		a |= ((b[2] << 8) & 0xFF00);

		a|= ((b[1] << 16) & 0xFF0000);

		a |= ((b[0] << 24) & 0xFF000000);

		return a;

	}

	static const void int_to_byte_array(int value,char s[])
	{
		s[0]=value >> 24;
		s[1]=value >>16;
		s[2]=value >>8;
		s[3]=value;
	}

	static int ipstr2int(string ip)
	{
		char ip_temp[15];

		const char *d = ".";

		strcpy(ip_temp,ip.c_str());
		char *numArray=strtok(ip_temp,d);

		int num=0;
		while(numArray)
		{

			numArray=strtok(NULL,d);
			num++;
		}

		strcpy(ip_temp,ip.c_str());
		char *strArray=strtok(ip_temp,d);

		if(num == 4)
		{
			 int ipIntarr[4];
			 for(int i=0;i<4;++i)
			 {
			    int temp=atoi(strArray);
			    strArray=strtok(NULL,d);
			    ipIntarr[i]=temp;
			    if(temp<0 || temp >255)
			    	cout<<"Invalid ip!"<<endl;
			  }
			  int re=((ipIntarr[0] << 24) & 0xff000000 ) | ( (ipIntarr[1] << 16) & 0x00ff0000 )
			    					 | ( (ipIntarr[2] << 8) & 0x0000ff00 ) | ( ipIntarr[3] & 0x000000ff);
			    return re;

		 }

		else
		{
			 cout<<"Invalid ip!"<<endl;
			 return 0;

		}
	}
	static string int2ipstr(int ip)
	{
		int iparr[4];
		iparr[0]=(ip >> 24) & 0x000000ff;
		iparr[1]=(ip >> 16) & 0x000000ff;
		iparr[2]=(ip >> 8) & 0x000000ff;
		iparr[3]=ip & 0x000000ff;
		char c[4];
		string s;
		for(int i=0;i<4;++i)
		{
			sprintf(c, "%d", iparr[i]);
			s.append(c);
		}
		return s;

	}

};
