#include<vector>
#include"algorithm"
#include"Node.h"

class Way{
	public:
	vector<Node> nodelist;
	static const long long serialVersionUID = -7116607006510557164L;

	int idWay;
	string typeWay; //inner  or outer

	Way()
	{
		this->idWay=0;
		this->typeWay="";
	}

	Way(int idWay,string typeWay)
	{
		this->idWay=idWay;
		this->typeWay=typeWay;
	}

	static bool SortByPositon(const Node &v1,const Node &v2)
	{
		return v1.position < v2.position;//升序排列
	}

	void sortByPosition()//将路径内的结点该position升序排序
	{
		 sort(nodelist.begin(),nodelist.end(),Way::SortByPositon);
	}


	//如果形参node在成员nodelist的第一位，
	//则将nodelist按顺序依次加入到形参list中,返回最后一个node
	//如果形参node在成员nodelist的最后一位，
	//则将nodelist按倒序依次加入到形参list中,返回第一个node
	// 否则不进行任何处理
	Node getOrderPoints(Node node,vector<Point> *list)
	{
		int index=-1;
		int i=0;
		for(vector<Node>::iterator it=this->nodelist.begin();
						it!=this->nodelist.end();++it,++i)
		{
			if(it->idNode==node.idNode)
			{
				index=i;
				break;
			}

		}

		if(index!=-1)
		{
			if(index==0)
			{
				for(int j=index+1;j<this->nodelist.size();j++)
				{
					list->push_back(this->nodelist[j].toPoint());
				}
				return (this->nodelist[this->nodelist.size()-1]);
			}

			else if(index==(this->nodelist.size()-1))
			{
				for(int j=index-1;j>=0;j--)
				{
					list->push_back(this->nodelist[j].toPoint());
				}
				return (this->nodelist[0]);
			}
			else
			{
				Node n(0,0,0,0);
				return n;
			}


		}
		Node n(0,0,0,0);
		return n;
	//			return c;
	}


};
