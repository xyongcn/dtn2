#include"Way.h"
#include<vector>



class AreaNode
{
public:
	int id;
	int level;
	Point key_point;
	vector<pair<Way,int> > WayList;     //outer 0,inner 1

	vector<int> ChildList_ref;
	vector<AreaNode> ChildList;

	AreaNode()
	{
		this->id=0;
		this->level=0;
		 WayList.clear();
		 ChildList_ref.clear();
		 ChildList.clear();
	}

	AreaNode& operator=(const AreaNode &n)
	{
		id=n.id;
		level=n.level;
		key_point=n.key_point;
		WayList=n.WayList;
		ChildList_ref=n.ChildList_ref;
		ChildList=n.ChildList;
		return *this;
	}
};
