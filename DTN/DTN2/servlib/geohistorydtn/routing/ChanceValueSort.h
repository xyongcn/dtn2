#include<string>
#include "NodeComparatorSort.h"

using namespace std;

namespace dtn
{
class ChanceValueSort
{
private:
	const static string tag;
public:
	static list<Area *> getAllAvaliableNodeArea(
			list<Area *> *nowNeighbour,Bundle *bundle,Area *thisnode);
	/**
		 * 在对所有的节点排序的时候，将历史邻居的也加入进来
		 */
	static list<Node> addNeibourAreaNode
		(list<Area *> *nowNeighbour,Bundle *bundle);


};
}
