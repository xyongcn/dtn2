#include<vector>
#include<iostream>
#include"Point.h"

using namespace std;

//检查待测节点是否在该区域内
class PointInPolygon {
public:

/*	static bool pointInPolygon1(double x,double y,vector<double> polygonX,vector<double> polygonY)
	{
		bool in=false;

		//对多边形周边点的数组的合法做判断
		if(polygonX.size()!=polygonY.size() || polygonX.empty() || polygonY.empty() )
		{
			cout<<"多边形的点数组不合法"<<endl;
		}

		//从待测节点引发的射线与多边形区域相交的个数
		int crossCount=0;
		for(int i=0;i<polygonX.size();i++)
		{
			int p1=i;
			int p2=(i+1)%polygonX.size();

			//如果待测节点是多边形的顶点，认为待测节点是在多边形内
			if((x==polygonX[p1] && y==polygonY[p1])||(x==polygonX[p2] && y==polygonY[p2]))
			{
//				in=true;
				crossCount=1;
				break;
			}

			//特殊情况
			//多边形相邻两节点的y坐标相等
			if(polygonY[p1]==polygonY[p2])
			{
				continue;
			}

			//待测节点不在两节点的y轴之间
			if(y<polygonY[p1] && y<polygonY[p2])
				continue;
			if(y>=polygonY[p2] && y>=polygonY[p2])
				continue;

			//求交点的x坐标�?
			double result=(double)(y-polygonY[p1])*(double)(polygonX[p2]-polygonX[p1])
					/((double)(polygonY[p2]-polygonY[p1])) + polygonX[p1];

			if(result>x)
				++crossCount;
		}

		return crossCount%2==1;
	}


	 static bool pointInPolygon(double x,double y,vector<double> polygonX,vector<double> polygonY)
	{
		bool in=false;

		//对多边形周边点的数组的合法判断
		if(polygonX.size()!=polygonY.size() || polygonX.empty() ||polygonY.empty() )
		{
			cout<<"多边形的点数组不合法"<<endl;
		}


		double minx=1.79769e+308;
		double maxx=2.22507e-308;
		double miny=1.79769e+308;
		double maxy=2.22507e-308;

		vector<double>::iterator iter=polygonX.begin();

		while(iter!=polygonX.end())
		{
			double temp=*iter;
			if(temp>maxx)
				maxx=temp;

			if(temp<minx)
				minx=temp;
			iter++;
		}

		iter=polygonY.begin();
		while(iter!=polygonY.end())
		{
			double temp=*iter;
			if(temp>maxy)
				maxy=temp;

			if(temp<miny)
				miny=temp;
			iter++;
		}

		if(x<minx || x>maxx || y<miny || y>maxy)
		{
			return false;
		}


		int i,j;
		for(i=0,j=polygonX.size()-1;i<polygonX.size();j=i++)
		{
			if(( (polygonY[i]>y) != (polygonY[j]>y))
					&& ( x< (polygonX[j]-polygonX[i]) * (y-polygonY[i]))
					/ (polygonY[j]-polygonY[i]) + polygonX[i])
			{
				in=!in;
			}
		}


		return in;
	}
*/

	 static bool pointInPolygon(Point p,vector<Point> ptPolygon)
	 {
		int nCount=ptPolygon.size();
		int nCross=0;
		for(int i=0;i<nCount;i++)
		{
			Point p1=ptPolygon[i];
			Point p2=ptPolygon[(i+1)%nCount];

			//如果目的点就是多边形顶点，认为目的点在多边形内
			if((p.longitude==p1.longitude && p.latitude==p1.latitude)
					||(p.longitude==p2.longitude && p.latitude==p2.latitude))
			{
				nCross=1;
				break;
			}

			//特殊情况，多边形的该边与射线平行
			if(p1.latitude==p2.latitude)
				continue;
			//射线与该条边不相交
			if(p.latitude<p1.latitude && p.latitude<p2.latitude)
				continue;
			if(p.latitude >= p1.latitude && p.latitude >=p2.latitude)
				continue;

			//计算并统计交点个数
			double longitude=(double)(p.latitude-p1.latitude) * (double)(p2.longitude-p1.longitude)
					/(double)(p2.latitude-p1.latitude)+p1.longitude;
			if(longitude>p.longitude)
				++nCross;
		}

		return (nCross%2==1);
	}

	static Point* generatePoint(double lon,double lat)
	{
		return new Point(lon, lat);
	}


};
