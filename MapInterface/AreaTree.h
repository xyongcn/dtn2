#include<list>
using namespace std;

struct AreaTreeNode
{
	AreaTreeNode *father;
	int level;
	int id;
};

struct BottomtoTopAreaTree
{
	list<AreaTreeNode *> buttonarea;
};

struct ToptoBottomAreaTree
{
	list<ToptoBottomAreaTree *>child;
	int level;
	int id;
};
