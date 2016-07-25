//由aodv.nod.xml得到结点信息，再依次生成
//line,boundry,areaNode，AreaTree, AreaTree 为地图的区域数

#include <hash_map>
#include<string>
#include<string.h>
#include <libxml/parser.h>
#include<iostream>
#include"structure/AreaNode.h"
#include <map>
#include<stdio.h>
#include<stdlib.h>


using namespace std;
using namespace __gnu_cxx;

#define POINT_MAX 50
#define LEVEL_HIGH 1

class AreaTree
{
public:
	AreaNode root;

	hash_map <int,Point> pointmap;
	hash_map <int,Way> waymap;
	hash_map <int,AreaNode> areanodemap;

	int getNodes()
	{
		char buffer[50];
		getcwd(buffer,50);
		strcat(buffer,"/assets/node.xml");

		xmlDocPtr doc;           //定义解析文档指针
		xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
		xmlChar *szKey;          //临时字符串变量

		doc = xmlReadFile(buffer,"UTF-8",XML_PARSE_RECOVER); //解析文件
		//检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。
		 //一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。
		//如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.
		if (NULL == doc)
		{
			//文档打开错误
			return -1;
		}

		curNode = xmlDocGetRootElement(doc); //确定文档根元素
		//检查确认当前文档中包含内容
		if (NULL == curNode)
		{
			//空得xml文件
			xmlFreeDoc(doc);
			return -2;
		 }
		 //在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。
		if (xmlStrcmp(curNode->name, BAD_CAST "osm"))
		{
			//分析根元素失败
			xmlFreeDoc(doc);
			return -3;
		 }
	    curNode = curNode->xmlChildrenNode;
	    xmlNodePtr propNodePtr = curNode;
	    xmlChar* id;
	    xmlChar* lon;
	    xmlChar* lat;
	    double lon_double;
	    double lat_double;
	    int id_int;
		 while(curNode != NULL)
		 	    {

		 	       //查找带有属性attribute的节点
		 	       if (xmlHasProp(curNode,BAD_CAST "id"))
		 	       {

		 	           propNodePtr = curNode;
		 	           xmlAttrPtr attrPtr = propNodePtr->properties;
		 		       id = xmlGetProp(propNodePtr,BAD_CAST "id");
		 		       char *id_temp=(char *)id;
		 		       id_int=0;
		 		       for(int i=0;id_temp[i]!='\0';++i)
		 		       {
		 		    	   id_int=(id_int*10)+id_temp[i]-48;
		 		       }
		 		   //    cout<<id_int<<endl;
		 		       xmlFree(id);

		 	       }
		 	       if (xmlHasProp(curNode,BAD_CAST "lon"))
		 	       {
		 	    	   propNodePtr = curNode;
		 	           xmlAttrPtr attrPtr = propNodePtr->properties;
		 	           lon = xmlGetProp(propNodePtr,BAD_CAST "lon");
		 	           lon_double=atof((char *)lon);
		 	  //         printf("%.8lf\n",lon_double); //经度，longitude

		 		       xmlFree(lon);

		 	       }
		 	       if (xmlHasProp(curNode,BAD_CAST "lat"))
		 	       {
		 	    	   propNodePtr = curNode;
		 	           xmlAttrPtr attrPtr = propNodePtr->properties;
		 	           lat = xmlGetProp(propNodePtr,BAD_CAST "lat");
		 	           lat_double=atof((char *)lat);
		 	 //          printf("%.9lf\n",lat_double); //纬度，latitude
		 		       xmlFree(lat);
		 	       }

			       Point point(id_int,lon_double,lat_double);
			  //     printf("%d,%.8lf,%.8lf\n",point.id,point.latitude,point.longitude);
			       pointmap[id_int]=point;
		 	       curNode = curNode->next;
		 }
		 xmlFreeDoc(doc);
		 return 0;
	}


	 	int getWays()
	{

		char buffer[50];
		getcwd(buffer,50);
		strcat(buffer,"/assets/way.xml");

		xmlDocPtr doc;           //定义解析文档指针
		xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
		xmlChar *szKey;          //临时字符串变量

		doc = xmlReadFile(buffer,"UTF-8",XML_PARSE_RECOVER); //解析文件
		//检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。
		 //一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。
		//如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.
		if (NULL == doc)
		{
			//文档打开错误
			return -1;
		}

		curNode = xmlDocGetRootElement(doc); //确定文档根元素
		//检查确认当前文档中包含内容
		if (NULL == curNode)
		{
			//空得xml文件
			xmlFreeDoc(doc);
			return -2;
		 }
		 //在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。
		if (xmlStrcmp(curNode->name, BAD_CAST "osm"))
		{
			//分析根元素失败
			xmlFreeDoc(doc);
			return -3;
		 }
		curNode = curNode->xmlChildrenNode;
	    xmlNodePtr propNodePtr = curNode;
		xmlNodePtr curNode_child;
	    xmlNodePtr propNodePtr_child;
	    xmlChar* id;
	    xmlChar* nod_ref;
	    int id_int;
	    int nod_ref_int[POINT_MAX];
	    memset(nod_ref_int,0,POINT_MAX);

	 	 while(curNode != NULL)
		 {
			    memset(nod_ref_int,0,POINT_MAX);

		 	       //查找带有属性attribute的节点
			 	 	if (xmlHasProp(curNode,BAD_CAST "id"))
		 	       {

		 	           propNodePtr = curNode;
		 	           xmlAttrPtr attrPtr = propNodePtr->properties;
		 		       id = xmlGetProp(propNodePtr,BAD_CAST "id");
		 		       char *id_temp=(char *)id;
		 		       id_int=0;
		 		       for(int i=0;id_temp[i]!='\0';++i)
		 		       {
		 		    	   id_int=(id_int*10)+id_temp[i]-48;
		 		       }
		 		       xmlFree(id);

		 	       }
			 	 	curNode_child=curNode->xmlChildrenNode;
			 	 	propNodePtr_child=curNode_child;
			 	 	int num=0;
					 while(curNode_child != NULL)
					 {
						if(xmlStrcmp(curNode->name, BAD_CAST "nd"))
						{
					 	if (xmlHasProp(curNode_child,BAD_CAST "ref"))
					 	{
					 		propNodePtr_child = curNode_child;
					 		xmlAttrPtr attrPtr = propNodePtr_child->properties;
					 		nod_ref = xmlGetProp(propNodePtr_child,BAD_CAST "ref");
					 		char *temp=(char *)nod_ref;
					 		for(int i=0;temp[i]!='\0';++i)
					 			nod_ref_int[num]=(nod_ref_int[num]*10)+temp[i]-48;
					 		xmlFree(nod_ref);
					 		num++;
					 	}

						}
						 curNode_child = curNode_child->next;
					 }
					 if(num!=0)
					 {
						 Point p;
						 Way w;
						 w.id=id_int;
						 for(int i=0;i<num;++i)
						 {
							 p.copy(pointmap[nod_ref_int[i]]);
							 w.PointList.push_back(p);
						 }
						 waymap[id_int]=w;

					 }

					curNode = curNode->next;
		 }

		 xmlFreeDoc(doc);
		 return 0;
	}

	 	int getAreas()
	{

		char buffer[50];
		getcwd(buffer,50);
		strcat(buffer,"/assets/area.xml");

		xmlDocPtr doc;           //定义解析文档指针
		xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
		xmlChar *szKey;          //临时字符串变量

		doc = xmlReadFile(buffer,"UTF-8",XML_PARSE_RECOVER); //解析文件
		//检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。
		 //一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。
		//如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.
		if (NULL == doc)
		{
			//文档打开错误
			return -1;
		}

		curNode = xmlDocGetRootElement(doc); //确定文档根元素
		//检查确认当前文档中包含内容
		if (NULL == curNode)
		{
			//空得xml文件
			xmlFreeDoc(doc);
			return -2;
		 }
		 //在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。
		if (xmlStrcmp(curNode->name, BAD_CAST "osm"))
		{
			//分析根元素失败
			xmlFreeDoc(doc);
			return -3;
		 }
		curNode = curNode->xmlChildrenNode;
	    xmlNodePtr propNodePtr = curNode;
		xmlNodePtr curNode_child;
	    xmlNodePtr propNodePtr_child;
	    xmlChar* id;
	    int id_int;
	    xmlChar* ref;
	    int ref_int;
	    xmlChar* role;
	    int role_int;   //outer is 0,inner is 1,subarea is 2,label is 3
	//    char *type_way="way";
	//    char *type_relation="relation";
	    char *role_inner="inner";
	    char *role_outer="outer";
	    char *role_subarea="subarea";
	    char *role_label="label";
	    int low_area_level=0;

	    char *k_level="admin_level";
	    xmlChar* k;
	    xmlChar* v;
	    int v_int;
		vector<pair<int,int> > way_list;
		vector<int> child_list;
		int node_ref;

	    curNode = curNode->next;
	 	 while(curNode != NULL)
		 {

		 	       //查找带有属性attribute的节点
			 	 	if (xmlHasProp(curNode,BAD_CAST "id"))
		 	       {

		 	           propNodePtr = curNode;
		 	           xmlAttrPtr attrPtr = propNodePtr->properties;
		 		       id = xmlGetProp(propNodePtr,BAD_CAST "id");
		 		       char *id_temp=(char *)id;
		 		       id_int=0;
		 		       for(int i=0;id_temp[i]!='\0';++i)
		 		       {
		 		    	   id_int=(id_int*10)+id_temp[i]-48;
		 		       }
		 		//       cout<<id_int<<endl;
		 		       xmlFree(id);

		 	       }
			  	    curNode_child=curNode->xmlChildrenNode;
			 	 	propNodePtr_child=curNode_child;
					way_list.clear();
					child_list.clear();
					 while(curNode_child != NULL)
					 {
						if(!xmlStrcmp(curNode_child->name, BAD_CAST "member"))
						{

							propNodePtr_child = curNode_child;
							xmlAttrPtr attrPtr = propNodePtr_child->properties;

							ref = xmlGetProp(propNodePtr_child,BAD_CAST "ref");
							role = xmlGetProp(propNodePtr_child,BAD_CAST "role");
						 	ref_int=0;
						 	char *ref_temp=(char *)ref;
						 	for(int i=0;ref_temp[i]!='\0';++i)
						 		ref_int=(ref_int*10)+ref_temp[i]-48;
						 	role_int=-1;
						 	if(!strcmp((char *)role,role_outer))
						 		role_int=0;
						 	else if(!strcmp((char *)role,role_inner))
						 		role_int=1;
						 	else if(!strcmp((char *)role,role_subarea))
						 		role_int=2;
						 	else if(!strcmp((char *)role,role_label))
						 		role_int=3;
						 	xmlFree(ref);
						 	xmlFree(role);
						 	//处理way/subarea/node项
						 	if(role_int == 0 || role_int == 1)
						 	{
						 		pair<int,int> pa(ref_int,role_int);
						 		way_list.push_back(pa);
						 	}
						    else if(role_int == 2)
						 		child_list.push_back(ref_int);
						    else if(role_int == 3)
						 		node_ref=ref_int;
						 	else
						 		cout<<"error!"<<endl;

						}
						 if(!xmlStrcmp(curNode_child->name, BAD_CAST "tag"))
						{
							propNodePtr_child = curNode_child;
							k = xmlGetProp(propNodePtr_child,BAD_CAST "k");
							if(!strcmp((char*)k,k_level))
							{
								v = xmlGetProp(propNodePtr_child,BAD_CAST "v");
						 		v_int=0;
						 		char *v_temp=(char *)v;
						 		for(int i=0;v_temp[i]!='\0';++i)
						 		    v_int=(v_int*10)+v_temp[i]-48;
						 		if(v_int>low_area_level)
						 			low_area_level=v_int;
						 		 xmlFree(v);
							}
							xmlFree(k);
						}

						 curNode_child = curNode_child->next;
					 }
					 AreaNode n;
					 Way w;
					 Point p;

					 n.id=id_int;
					 n.level=v_int;
					 p=pointmap[node_ref];
					 n.key_point=p;
					 for(vector<pair<int,int> >::iterator it=way_list.begin();
							 it<way_list.end();++it)
					 {
						 w=waymap[(*it).first];
						 pair<Way,int> pwi(w,(*it).second);
						 n.WayList.push_back(pwi);
					 }

			/*		 for(vector<pair<Way,int> >::iterator it=n.WayList.begin();
							 it<n.WayList.end();++it)
						 cout<<it->first.id<<"    ";
					 cout<<endl;*/


					 for(vector<int>::iterator it=child_list.begin();
							 it<child_list.end();++it)
					 {
						 n.ChildList_ref.push_back(*it);
					 }

					 if(!areanodemap.count(id_int))
						 areanodemap[id_int]=n;
					 curNode = curNode->next;
		 }
	 	 for(int i=low_area_level-1;i>0;--i)
	 	 {
	 		 for(hash_map<int,AreaNode>::iterator it=areanodemap.begin();
	 			it!=areanodemap.end();++it)
	 		 {
	 			 if(it->second.level==i)
	 			 {
	 				 for(vector<int>::iterator p=it->second.ChildList_ref.begin();
	 					 p<it->second.ChildList_ref.end();++p)
	 					 it->second.ChildList.push_back(areanodemap[*p]);
	 			 }
	 		 }
	 	 }

	 	 xmlFreeDoc(doc);
		 return 0;
	}

	void testnode()
	{
		hash_map <int,Point>::iterator it=pointmap.begin();
		 for(; it!=pointmap.end();it++)
		 {
			 printf("%d,%lf,%lf\n",it->second.id,it->second.latitude,it->second.longitude);
		 }
	}
	void testway()
	{
		hash_map <int,Way>::iterator it=waymap.begin();
		 for(; it!=waymap.end();it++)
		 {

			// cout<<it->first<<"   "<<it->second.id<<"   ";
			 for(vector<Point>::iterator p=it->second.PointList.begin();p<it->second.PointList.end();++p)
			 {
				 cout<<p->id<<"   ";
			 }
			 cout<<endl;
		 }
	}
	void testarea()
	{
		hash_map <int,AreaNode>::iterator it=areanodemap.begin();
		 for(; it!=areanodemap.end();it++)
		 {
			 AreaNode n=it->second;
			 cout<<n.id<<"   "<<n.level<<"   "
					 <<n.key_point.id<<endl;
			 for(vector<pair<Way,int> >::iterator p=n.WayList.begin();
			 							 p<n.WayList.end();++p)
			 						 cout<<p->first.id<<"    ";
			 					 cout<<endl;
				for(vector<int>::iterator p=n.ChildList_ref.begin();
			 		 p<n.ChildList_ref.end();++p)
			 			 cout<<*p<<"    ";
				for(vector<AreaNode>::iterator p=n.ChildList.begin();
			 		 p<n.ChildList.end();++p)
			 			 cout<<p->id<<"    ";
			  cout<<endl;
		 }
	}

	AreaTree()
	{
		getNodes();
		getWays();
		getAreas();
		for(hash_map<int,AreaNode>::iterator it=areanodemap.begin();
			 it!=areanodemap.end();++it)
		{
				if(it->second.level==LEVEL_HIGH)
						root=it->second;

		}
	}

};



