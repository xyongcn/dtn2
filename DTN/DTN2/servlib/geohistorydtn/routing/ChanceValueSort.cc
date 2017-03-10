#include "ChanceValueSort.h"
#include<algorithm>

namespace dtn
{
	const string ChanceValueSort::tag="ChanceValueSort";

	//nowNeighbour内的Area存储的是当前邻居节点与bundle的目的地址最接近的区域，
	//和本节点与bundle的目的地址最接近的区域
	//thisnode内的Area是本节点与bundle的目的地址最接近的区域,
	//它与nowNeighbour中存储的最后一个Area是相同的
	list<Area *> ChanceValueSort::getAllAvaliableNodeArea(
			list<Area *> *nowNeighbour,Bundle *bundle,Area *thisnode,
			string *record,map<Area *,string> neiIdRecord)
	{
		list<Area *>::iterator it=(*nowNeighbour).begin();
		for(;it!=(*nowNeighbour).end();it++)
		{
			if((*it)->id == thisnode->id)
				break;
		}
		if(it == (*nowNeighbour).end() )
		{
			cout<<"计算比本节点更优节点时，本节点不再列表中"<<endl;
			(*nowNeighbour).push_back(thisnode);

		}

		//allNodeList 存储本节点，当前邻居，以及多个临时邻居到达目的地最近的Area和机会值
		list<Node> allNodeList=addNeibourAreaNode(nowNeighbour, bundle);
		vector<Node> nodelist_temp;
		for(list<Node>::iterator it=allNodeList.begin();it!=allNodeList.end();++it)
		{
			nodelist_temp.push_back(*it);
		}
		/////////////////////////////////////////////////////
		for(vector<Node>::iterator it=nodelist_temp.begin();it!=nodelist_temp.end();++it)
		{
			if((*it).nei==NULL)
			{
				map<Area *,string>::iterator itn;
				for(itn=neiIdRecord.begin();itn!=neiIdRecord.end();++itn)
				{
					if(itn->first==it->closedArea)
						break;
				}
				if(itn==neiIdRecord.end())
					record->append("this node:");
				else
				{
					record->append("current neighbour id:");
					record->append(itn->second);
				}
				record->append("\ndestination id:");
				char tp[10];
				sprintf(tp,"%d",(*it).closedArea->id);
				record->append(tp);
				record->append(" opportunity value:");
				for(int i=0;i<(*it).chanceValue.size();++i)
				{
					sprintf(tp,"%f",(*it).chanceValue[i]);
					record->append(tp);
					record->append(" ");
				}
				record->append("\n");
			}
			else
			{
				record->append("history neighbour id:");
				char tp[10];
				record->append((*it).nei->neighbourEidstr);
				record->append("\ndestination id:");
				sprintf(tp,"%d",(*it).closedArea->id);
				record->append(tp);
				record->append(" opportunity value:");
				for(int i=0;i<(*it).chanceValue.size();++i)
				{
					sprintf(tp,"%f",(*it).chanceValue[i]);
					record->append(tp);
					record->append(" ");
				}
				record->append("\n");
			}
		}
		/////////////////////////////////////////////////////
		sort(nodelist_temp.begin(),nodelist_temp.end(),NodeComparatorSort::compare);

		vector<Node>::iterator iter;
		int count=0;
		/////////////////////////////////////////////////
		//printf("\n当前邻居，历史邻居，本节点到达目的地的机会值\n");
		printf("\ncurrent neighbour,history neighbour,this node's value\n");
		for(iter=nodelist_temp.begin();iter!=nodelist_temp.end();++iter)
		{
			/*if((*iter).nei!=NULL)
			{
				printf("历史邻居(%s)的机会值为:",(*iter).nei->neighbourEid.str().c_str());

			}
			else
			{
				if(count==0)
				{
					printf("当前邻居的机会值为:");
					count++;
				}
				else
				{
					printf("本节点的机会值为:");
				}
			}*/
			for(int i=0;i<(*iter).chanceValue.size();++i)
				cout<<(*iter).chanceValue[i]<<" ";
			printf("\n");
		}

		//////////////////////////////////////////

		list<Area *> avaliable;

		for(vector<Node>::iterator it=nodelist_temp.begin();it!=nodelist_temp.end();++it)
		{
			if((*it).closedArea!=thisnode)
			{
				avaliable.push_back((*it).closedArea);
			}
			else
				break;
		}
		return avaliable;
	}


	list<Node> ChanceValueSort::addNeibourAreaNode
			(list<Area *> *nowNeighbour,Bundle *bundle)
	{
		list<Node> nodelist;

		//加入当前邻居表的node,这个表里面包含有本几点的node
		//cout<<endl<<"当前邻居到各个区域的机会值,和本节点到达各个区域的机会值:"<<endl;
		cout<<endl<<"current neighbour and this node's opportunity value:"<<endl;
		for(list<Area *>::iterator it=(*nowNeighbour).begin();
				it!=(*nowNeighbour).end();++it)
		{
			Area *area=*it;
			vector<double> v=ChanceValueCompute::carryChance(bundle, area);
			Node *tempNode=new Node((*it),NULL,v);
			nodelist.push_back(*tempNode);
		}

		//cout<<endl<<"历史邻居到各个区域的机会值:"<<endl;
		cout<<endl<<"history neighbour's opportunity value:"<<endl;
		list<Neighbour *> historyNeighbour=NeighbourManager::Getinstance()->getAllNeighbour();
		for(list<Neighbour *>::iterator it=historyNeighbour.begin();
				it!=historyNeighbour.end();++it)
		{
			//这个邻居没有它的历史区域的记录
			NeighbourArea *neiArea=(*it)->getNeighbourArea();
			if(neiArea==NULL)
				continue;
			//这个邻居的历史区域没有接近目的区域的
			Area *area=neiArea->checkBundleDestArea(bundle);
			if(area==NULL)
				continue;
			//判断是否这个历史邻居中点在当前邻居中存在,存在，则不需要
			list<Area *>::iterator p=nowNeighbour->begin();
			for(;p!=nowNeighbour->end();p++)
			{
				if(*p==area)
					break;
			}
			if(p!= nowNeighbour->end() )
				continue;
			vector<double> v=ChanceValueCompute::histroyNeighbourCarryChance(bundle, area, *it);
			Node *tempNode=new Node(area,*it,v);
			nodelist.push_back(*tempNode);
		}

		return nodelist;
	}


}
