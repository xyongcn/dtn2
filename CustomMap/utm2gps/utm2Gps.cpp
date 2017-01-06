#include <iostream> 
#include "math.h"
#include "stdio.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define P  206264.806247096355
#define PI 3.141592653589793 

void GaosF_fun(double x,double y,double A[]) 
{      
	double B,Bf,Nf,b,b2,b3,b4,b5,Z,l,L0;  
//	x=3380330.875;
//	y=320089.976;
	double default_longitude=116.30941;//default longitude
	// double default_latitude=39.95789;
	double default_latitude=0;

	b=x/6367558.4969;

	Bf=b+(50221746+    (     293622+   (2350+22*cos(b)*cos(b))*cos(b)*cos(b)    )  *cos(b)*cos(b))  *sin(b)*cos(b)*1e-10;
	Nf=6399698.902-(21562.267-(108.973-0.612*cos(Bf)*cos(Bf))*cos(Bf)*cos(Bf))*cos( Bf)*cos(Bf);

	Z=y/Nf/cos(Bf);
	b2=(0.5+0.003369*cos(Bf)*cos(Bf))*sin(Bf)*cos(Bf);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	b3=0.333333-(0.166667-0.001123*cos(Bf)*cos(Bf))*cos(Bf)*cos(Bf);
	b4=0.25+(0.16161+0.00562*cos(Bf)*cos(Bf))*cos(Bf)*cos(Bf); 
	b5=0.2-(0.167-0.0088*cos(Bf)*cos(Bf))*cos(Bf)*cos(Bf); 

	B=Bf-(1-(b4-0.12*Z*Z)*Z*Z)*Z*Z*b2; 
	l=(1-(b3-b5*Z*Z)*Z*Z)*Z;      
	printf("B=%fL=%f\n",B*180/PI,(L0+l*180/PI));
	A[1]=B*180/PI;//latitude
	A[0]=(L0+l*180/PI);//longitude
	A[1]+=default_latitude;
	A[0]+=default_longitude;
//	cout<<endl;
	cout<<setprecision(10)<<A[0]<<"\t"<<setprecision(10)<<A[1]<<"\n";
	cout<<fixed<<A[0]<<"\t"<<A[1]<<endl;
	// printf("a[0]=%f,a[1]=%f\n",A[0],A[1]);
}



int main()
{
	ifstream fin("./input.nod.xml", ios::in);
		ofstream fout("./output.nod.xml");
	ofstream* fp = &fout;
	
	if (fin == NULL)
	{
		cout<<"open failed!"<<endl;
	}
	string line;
	double *A=new double[2];

	while(getline(fin,line,'\n'))
	{
		string wline=line.data();
		char *part=strstr (const_cast<char*>(line.data()), "x=");
		char *part_y=strstr (const_cast<char*>(line.data()), "y=");
		if ( part== NULL || part_y==NULL)
		{
			(*fp)<<line.data()<<endl;
			continue;
		}
		char* part1 =strtok(part," ");
		char* part2 =strtok(part," ");

		char* part_y1 =strtok(part_y," ");
		char* part_y2 =strtok(part_y," ");

		int i=3,j=0;
		while(part1[i]!='"')
		{
			part2[j]=part1[i];
			++i;
			++j;
		}
		part2[j]='\0';
		double x= atof(part2);


		int p=3,q=0;
		while(part_y1[p]!='"')
		{
			part_y2[q]=part_y1[p];
			++p;
			++q;
		}
		part_y2[q]='\0';
	    double y= atof(part_y2);
	//	cout<<x<<"   "<<y<<endl;

	    //x means longtitude,y means latitude
	    //GaosF_fun(latitude,longitude,A)
		GaosF_fun(y,x,A); 




		string p1 = strtok(const_cast<char*>(wline.data())," ");
		string p2=  strtok(NULL," ");
		string p3=  strtok(NULL," ");
		string p4=  strtok(NULL," ");
		string p5=  strtok(NULL," ");


		
		(*fp)<<fixed<<setprecision(8)<<p1<<" "<<p2<<" "<<"x=\""<<A[0]<<"\""<<" "<<"y=\""<<A[1]<<"\""<<" "<<p5<<endl;
	//	(*fp)<<wline.data()<<endl;

	}

	system("pause");
	return 0;
}