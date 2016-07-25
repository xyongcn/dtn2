#include<string>
#include<iostream>
#include<stdlib.h>
#include<string.h>

using namespace std;

class IpHelper
{
public:
	//将ip地址的字符串转换为Int来表示
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
			    	cout<<" invalid ip";
			}
			int re=((ipIntarr[0] << 24) & 0xff000000 ) | ( (ipIntarr[1] << 16) & 0x00ff0000 )
			    	 | ( (ipIntarr[2] << 8) & 0x0000ff00 ) | ( ipIntarr[3] & 0x000000ff);
			    return re;

		 }

		 else
		 {
			 cout<<"invalid ip"<<endl;
			 return 0;

		 }
	}
};
