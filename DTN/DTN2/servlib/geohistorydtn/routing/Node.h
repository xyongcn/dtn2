#include "ChanceValueCompute.h"

namespace dtn{

class Node
{
public:
	/**
	 * 最接近目的区域的区域，可以获得相应的层次
	 */
	Area *closedArea;    //底层区域

	vector<double> chanceValue;   //某个bundle的生命周期内，area和它的多个父区域的机会值
								//chanceValue[0]是底层区域的机会值，以此类推

	Node(Area *closedArea,vector<double> chanceValue)
	{
		this->closedArea=closedArea;
		this->chanceValue=chanceValue;
	}
};
}
