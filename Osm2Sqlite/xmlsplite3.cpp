#include"AreaTree.h"
#include<queue>
#include<sqlite3.h>
#include<fstream>

void writeAreaLayertoDb(sqlite3 *pDB)
{


}

int main()
{
	//generate a AreaTree
	AreaTree tree;

	//open sumo.db
	fstream _file;
	char *buffer=new char[100];
	strcpy(buffer,"/home/gaorui/sumo.db");
	_file.open(buffer,ios::in);
	    if(!_file)
	    {
	        cout<<"sumo.db没有被创建"<<endl;
	     }
	     else
	     {
	         cout<<"sumo.db已经存在"<<endl;
	         remove("sumo.db");
	     }
		sqlite3 *db = NULL;
		int rc = sqlite3_open(buffer, &db);
		if(rc)
		{
		cout << " Open the database failed" << endl;
		}

		 char* errMsg;
		 //Relation type: way is 0, subarea is 1, node is 2
		 //idRole:outer  is 0,inner  is 1,subarea is 2,label is 3
		 string strSQL= "CREATE TABLE Relation ( id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,idRelation  INTEGER    NOT NULL,idRef  INTEGER    NOT NULL,Type  INTEGER    NOT NULL,idRole  INTEGER    NOT NULL);";
		 sqlite3_exec(db , strSQL.c_str() ,0 ,0, &errMsg);
		 strSQL= "CREATE TABLE RelationRole ( id   INTEGER PRIMARY KEY NOT NULL UNIQUE,Name  TEXT  NOT NULL);";
		 sqlite3_exec(db , strSQL.c_str() ,0 ,0, &errMsg);
		 strSQL= "CREATE TABLE Node ( id   INTEGER PRIMARY KEY NOT NULL UNIQUE,lat  INTEGER NOT NULL,lon  INTEGER NOT NULL );";
		 sqlite3_exec(db , strSQL.c_str() ,0 ,0, &errMsg);
		 strSQL= "CREATE TABLE KeyValueRelation ( id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,idRelation   INTEGER NOT NULL ,idKey  INTEGER    NOT NULL,idVal  INTEGER    NOT NULL);";
		 sqlite3_exec(db , strSQL.c_str() ,0 ,0, &errMsg);
		 strSQL= "CREATE TABLE KeyNode ( id   INTEGER PRIMARY KEY NOT NULL UNIQUE,Name  TEXT    NOT NULL);";
		 sqlite3_exec(db , strSQL.c_str() ,0 ,0, &errMsg);
		 strSQL= "CREATE TABLE ValueNode ( id   INTEGER PRIMARY KEY NOT NULL UNIQUE,Text  TEXT    NOT NULL);";
		 sqlite3_exec(db , strSQL.c_str() ,0 ,0, &errMsg);
		 strSQL= "CREATE TABLE Way ( id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,idWay  INTEGER    NOT NULL,idNode INTEGER    NOT NULL,Position  INTEGER    NOT NULL);";
		 sqlite3_exec(db , strSQL.c_str() ,0 ,0, &errMsg);

			int outer_id=0;
			int inner_id=1;
			int subarea_id=2;
			int label_id=3;
			char value[500];
			sprintf(value,"insert into RelationRole(id,Name) values('%d','subarea');",subarea_id);
			sqlite3_exec(db,value,0,0,&errMsg);
			sprintf(value,"insert into RelationRole(id,Name) values('%d','inner');",inner_id);
			sqlite3_exec(db,value,0,0,&errMsg);
			sprintf(value,"insert into RelationRole(id,Name) values('%d','outer');",outer_id);
			sqlite3_exec(db,value,0,0,&errMsg);
			sprintf(value,"insert into RelationRole(id,Name) values('%d','lable');",label_id);
			sqlite3_exec(db,value,0,0,&errMsg);

		queue<AreaNode> q;
		AreaNode n;
		q.push(tree.root);
		hash_map<int,int> way_flag;
		hash_map<int,int> point_flag;

			while(!q.empty())
		{
			n=q.front();
			q.pop();
			for(vector<AreaNode>::iterator it=n.ChildList.begin();
					it<n.ChildList.end();++it)
			{
				sprintf(value,"insert into Relation(idRelation,idRef,Type,idRole) values('%d','%d','1','%d');",n.id,it->id,subarea_id);
				sqlite3_exec(db,value,0,0,&errMsg);
				q.push(*it);
			}
			sprintf(value,"insert into Relation(idRelation,idRef,Type,idRole) values('%d','%d','2','%d');",n.id,n.key_point.id,label_id);
			sqlite3_exec(db,value,0,0,&errMsg);


			if(point_flag[n.key_point.id]==1)
				continue;

			double temp=n.key_point.longitude*1000000;
			int lon=(int)temp;
			temp=n.key_point.latitude*1000000;
			int lat=(int)temp;


			//handle with point
			sprintf(value,"insert into Node(id,lat,lon) values('%d','%d','%d')", n.key_point.id,lat,lon);
			sqlite3_exec(db,value,0,0,&errMsg);
			point_flag[n.key_point.id]=1;

			//way
			for(vector<pair<Way,int> >::iterator it=n.WayList.begin();
					it<n.WayList.end();++it)
			{
				if(it->second == outer_id)
					sprintf(value,"insert into Relation(idRelation,idRef,Type,idRole) values('%d','%d','0','%d');",n.id,it->first.id,outer_id);
				else if(it->second == inner_id)
					sprintf(value,"insert into Relation(idRelation,idRef,Type,idRole) values('%d','%d','0','%d');",n.id,it->first.id,inner_id);
				sqlite3_exec(db,value,0,0,&errMsg);

				if(way_flag[it->first.id]==1)
					continue;

				//handle with way
				int i=1;
				for(vector<Point>::iterator p=it->first.PointList.begin();
						p<it->first.PointList.end();++p,++i)
				{
					sprintf(value,"insert into Way(idWay,idNode,Position) values('%d','%d','%d')", it->first.id,p->id,i);
					sqlite3_exec(db,value,0,0,&errMsg);


					if(point_flag[p->id]==1)
						continue;

					double temp=p->longitude*1000000;
					int lon=(int)temp;
					temp=p->latitude*1000000;
					int lat=(int)temp;
					//handle with point
					sprintf(value,"insert into Node(id,lat,lon) values('%d','%d','%d')", p->id,lat,lon);
					sqlite3_exec(db,value,0,0,&errMsg);
					point_flag[p->id]=1;

				}
				way_flag[it->first.id]=1;

			}

		}
	cout<<"sumo.db创建完毕，在路径"<<buffer<<"下"<<endl;
	delete [] buffer;
	return 0;
}
