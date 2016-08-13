#include<math.h>
#include<iostream>

using namespace std;

class Distance
{
private:
	static const double EARTH_RADIUS = 6378137.0;//单位m

	static double rad(double d)
	{
		return d * M_PI / 180.0;
	}

public:
	// 返回单位是米
	static double getDistance(double latitude1, double longitude1, double latitude2, double longitude2)
	{
		cout<<"当前结点经纬度："<<latitude1<<"  ";
		printf("%.6lf\n",longitude1);
		cout<<"邻居结点经纬度："<<latitude2<<"  ";
		printf("%.6lf\n",longitude2);
		double Lat1 = rad(latitude1);
		double Lat2 = rad(latitude2);
		double a = Lat1 - Lat2;
		double b = rad(longitude1) - rad(longitude2);
		double s = 2 * asin(sqrt(pow(sin(a / 2), 2)
					+ cos(Lat1) * cos(Lat2) * pow(sin(b / 2), 2)));
		s = s * EARTH_RADIUS;
		s = round(s * 10000) / 10000;
		//单位为米，不需要再进行进行处理
		return s;
	}

	static bool canConnected(double distance)
	{
		NodeInfo *node = NodeInfo::GetInstance();
		/*
		double random_dis = Math.random() * node.com_Dis/2.5 - node.com_Dis/5;//暂定波动范围为通信范围的1/5，正负
		//做实验的时候可以不要随机数，否则难控制

		if( (distance + random_dis) < node.com_Dis)
		*/
		if(distance < node->com_Dis)
			return true;
		return false;
	}

};
