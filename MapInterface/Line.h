#include"Way.h"
#include<string>
//一个区域可能由多个边界(Line)组成
class Line
{
public:
	vector<Way> waylist;
	static const long serialVersionUID = 8235873185083847058L;
	int idRelation;
	string type;
	Line(int idRelation,string type)
	{
		this->idRelation=idRelation;
		this->type=type;
	}

	// 返回区域多边形中所有结点的序列

	vector<Point> getPolygon()
	{

		vector<Point> polygon;
		Way lastway=this->waylist[this->waylist.size()-1];
		//**改进的判断版本，防止两个点时无法挑准首节点
		Node start=lastway.nodelist[lastway.nodelist.size()-1];//先用最后一个边的最后一个点作为起点
		Way temp;
		bool b=true;
		for(int i=0;i<this->waylist.size();i++)
		{
			temp=this->waylist[i];
			Node t=temp.getOrderPoints(start, &polygon);
			if(t.idNode==0)
			{
	//			Log.i(tag,String.format("way(%s)找不到上一个点作为起点way size:%d,node_id:%s,node_position:%d",temp.idWay,temp.size(),start.idNode,start.position));
				b=false;
				break;
			}
			start=t;
		}
		if(b)
		//此时说明waylist是一个封闭的区域
		{
			return polygon;//polygon中存储了整个区域的所有结点
		}
		b=true;//重置标志位
		polygon.clear();
		start=lastway.nodelist[0];//先用最后一个边的第一个点作为起点
		for(int i=0;i<this->waylist.size();i++)
		{
			temp=this->waylist[i];
			Node t=temp.getOrderPoints(start, &polygon);
			if(t.idNode==0)
			{
	//			Log.i(tag,String.format("way(%s)找不到上一个点作为起点way size:%d,node_id:%s,node_position:%d",temp.idWay,temp.size(),start.idNode,start.position));
				b=false;
				break;
			}
			start=t;
		}
		if(b)
		{
			return polygon;
		}
		else
		{
			printf("way找不到上一个点作为起点");
			vector<Point> p;
			return p;
		}
	}
};
