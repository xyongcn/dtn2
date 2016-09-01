#include<string>
#include<sqlite3.h>
#include<fstream>
#include<vector>
#include<stdlib.h>
#include<stdio.h>
#include"PointInPolygon.h"
#include<limits.h>

#include"Line.h"

using namespace std;
//从地图接口数据库中读取数据库的区域分层
class MapSqllite
{
private:
	static MapSqllite *Instance;
	MapSqllite()
	{
		dbfile.open("./sumo.db",ios::in);
		if(!dbfile)
		{
			cout<<"sumo.db 没有被创建"<<endl;
		}
		else
		{
		    cout<<"sumo.db 已经存在"<<endl;
		}
		int rc = sqlite3_open("./sumo.db", &database);
		if(rc)
		{
			cout << " Open the database failed" << endl;
		}
	}
	static const string TAG;

public:
	sqlite3 *database;
	fstream dbfile;
	static MapSqllite* Getinstance()
	{
	        if(Instance == NULL)  //判断是否第一次调用
	        	Instance = new MapSqllite();


	        return Instance ;
	}


	bool checkDbExist()
	{
		if(database==NULL)
			return false;
		return true;

	}

	void querySubarea(Point p,int idRelation,vector<int> *idRelationList)
	{
		//查询idRelation区域所有的子区域的id
		char t[10];
		sprintf(t, "%d", idRelation);
		string sql="select a.idRef,b.Name from Relation a,RelationRole b ";
		sql.append("where a.idRole=b.id and b.Name='subarea' and a.idRelation=");
		sql.append(t);
		sqlite3_stmt* statement = NULL;
		int idRef;
		sqlite3_prepare(database,sql.c_str(),-1, &statement, NULL);
		while (sqlite3_step(statement) == SQLITE_ROW)
		{
	         idRef = sqlite3_column_int(statement, 0);
	         cout<<"查询区域"<<idRef<<"是否包含点:";
	         printf("%.6lf,%.6lf\n",p.longitude,p.latitude);
	         int k=9;
	         k=isPointInPolygon(p, idRef);
	         if(k==1)
	         {
	        	 idRelationList->push_back(idRef);
	        	 cout<<"节点属于该区域"<<idRef<<endl;
	        	 querySubarea(p, idRef, idRelationList);
	        	 break;//地区的属于都是唯一的，属于某一个子区域后其余的就不用找了

	         }
	     }
	}

	vector<Line> lineList;
	//检查该点是否在该区域内
	int isPointInPolygon(Point p,int idRelation)
	{
		//查询组成该区域的所有路径，以及组成这些路径的结点
		char t2[10];
		sprintf(t2, "%d", idRelation);
		string sql2="select a.idRelation,b.idWay,b.Position,b.idNode,c.Name,d.lat,d.lon ";
		sql2.append("from Relation a,Way b,RelationRole c,Node d ");
		sql2.append("where b.idNode=d.id and a.idRef=b.idWay and a.idRole=c.id and (c.Name='inner' or c.Name='outer')");
		sql2.append("and a.idRelation=");
		sql2.append(t2);
		sqlite3_stmt *statement=NULL;
		int rowNum=0;
		sqlite3_prepare(database,sql2.c_str(),-1, &statement, NULL);
		while (sqlite3_step(statement) == SQLITE_ROW)
		{
			++rowNum;

		}
		if(rowNum==0)
			return 0;
		lineList.clear();
		Line *line=NULL;
		Way *way=NULL;

		int maxLon=INT_MIN;
		int maxLat=INT_MIN;
		int minLon=INT_MAX;
		int minLat=INT_MAX;
		sqlite3_stmt *state=NULL;
		sqlite3_prepare(database,sql2.c_str(),-1, &state, NULL);
		while (sqlite3_step(state) == SQLITE_ROW)
		{
			int temp_idRelation=sqlite3_column_int(state, 0);
			int temp_idWay=sqlite3_column_int(state, 1);
			int temp_position=sqlite3_column_int(state, 2);
			int temp_idNode=sqlite3_column_int(state, 3);
			string temp_type=(char *)sqlite3_column_text(state, 4);
			int lat=sqlite3_column_int(state, 5);
			int lon=sqlite3_column_int(state, 6);
		//	cout<<"----------------------"<<endl;
		//	cout<<temp_idRelation<<" "<< temp_idWay<<endl;

			if(line==NULL || way==NULL)
			{
				//查询所对应的区域第一个边界,即形参idRelation对应的区域
				line=new Line(temp_idRelation,temp_type);
				lineList.push_back(*line);
				//该边界中的第一条路
				way=new Way(temp_idWay, temp_type);
				lineList.back().waylist.push_back(*way);
			}

			else
			{
				//读到一个多边形不同的type，由outer转inner，表示一个完整的线段读出来了，
				//而一个点是否属于这个多边形看包含它最里层的多边形是不是outer
				if(line->type!=temp_type)
				{
					line=new Line(temp_idRelation,temp_type);
					lineList.push_back(*line);
					lineList.back().waylist.back().sortByPosition();
					way=new Way(temp_idWay, temp_type);
					lineList.back().waylist.push_back(*way);
				}
				//读到了不同的线段
				else if(way->idWay!=temp_idWay)
				{
					lineList.back().waylist.back().sortByPosition();
					way=new Way(temp_idWay, temp_type);
					lineList.back().waylist.push_back(*way);
				}

			}
			Node n(temp_idNode, lon, lat, temp_position);
			lineList.back().waylist.back().nodelist.push_back(n);
			//way->nodelist.push_back(n);

			if(lat<minLat)
				minLat=lat;
			if(lat>maxLat)
				maxLat=lat;

			if(lon<minLon)
				minLon=lon;
			if(lon>maxLon)
				maxLon=lon;

		}

		//对最后一个线段排序
		lineList.back().waylist.back().sortByPosition();
	/*	for(vector<Way>::iterator it=lineList[0].waylist.begin();it!=lineList[0].waylist.end();++it)
		{
			cout<<it->idWay<<" : ";
			for(vector<Node>::iterator p=it->nodelist.begin();p!=it->nodelist.end();++p)
			{
				cout<<p->idNode<<"  ";
			}
			cout<<endl;
		}*/
		//记录包含点的最有一个边界类型，是inner还是outer
		string inType;
		for(vector<Line>::iterator it=lineList.begin();it!=lineList.end();++it)
		{
			vector<Point> list=it->getPolygon();
			if(list.empty())
				return -1;

			if(PointInPolygon::pointInPolygon(p, list))
			{
					inType=it->type;
			}
			else
				break;
		}

		if(!inType.empty() && inType=="outer")
		{
				return 1;
		}
		else
		{
				return 0;
		}
	}
};

MapSqllite * MapSqllite::Instance=NULL;


const string MapSqllite::TAG="MapSqllite";
