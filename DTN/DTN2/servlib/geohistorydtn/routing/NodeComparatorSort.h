#include "Node.h"
#include <math.h>
/**
 * 说明  :对所有的携带节点进行排序，优先级：有机会值的>无机会值；区域层次低>区域层次高；从低层次到高层，机会值分量越大越优先
 */
namespace dtn
{
class NodeComparatorSort
{

	//要对node节点进行降序排列
	//return为>0时，会交换两个参数

public:
	static bool compare(const Node &node1,const Node &node2)
	{
		//node1为空时，交换
		/*if(node1==NULL)
			return true;

		if(node2==NULL)
			return false;*/

		//node2的区域层次更低，所以交换
		Area *area1 =(node1.closedArea);
		int node1level=area1->getAreaLevel();

		Area *area2 =(node2.closedArea);
		int node2level=area2->getAreaLevel();

		if(node2level<node1level)
			return true;

		if(node2level>node1level)
			return false;

		//逐个比较机会值
		for(int i=0;i<min(node1.chanceValue.size(), node2.chanceValue.size());i++)
		{
			if(node2.chanceValue[i]>node1.chanceValue[i])
			{
				return true;
			}

			if(node2.chanceValue[i]<node1.chanceValue[i])
			{
				return false;
			}
		}


		//默认不交换
		return false;
	}

};
}
