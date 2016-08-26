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
			list<Area *> *nowNeighbour,Bundle *bundle,Area *thisnode)
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

		list<Node> allNodeList=addNeibourAreaNode(nowNeighbour, bundle);
		vector<Node> nodelist_temp;
		for(list<Node>::iterator it=allNodeList.begin();it!=allNodeList.end();++it)
		{
			nodelist_temp.push_back(*it);
		}
		sort(nodelist_temp.begin(),nodelist_temp.end(),NodeComparatorSort::compare);

		list<Area *> avaliable;

		for(vector<Node>::iterator it=nodelist_temp.begin();it!=nodelist_temp.end();++it)
		{
			if((*it).closedArea->id!=thisnode->id)
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
		for(list<Area *>::iterator it=(*nowNeighbour).begin();
				it!=(*nowNeighbour).end();++it)
		{
			Area *area=*it;
			Node *tempNode=new Node((*it),ChanceValueCompute::carryChance(bundle, area));
			nodelist.push_back(*tempNode);
		}

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

			//这个历史邻居中点在当前邻居中存在
			list<Area *>::iterator p=(*nowNeighbour).begin();
			for(;p!=(*nowNeighbour).end();p++)
			{
				if((*p)->id ==area->id)
					break;
			}
			if(p!= (*nowNeighbour).end() )
				continue;

			Node *tempNode=new Node(area, ChanceValueCompute::histroyNeighbourCarryChance(bundle, area, *it));
			nodelist.push_back(*tempNode);
		}

		return nodelist;
	}


}
