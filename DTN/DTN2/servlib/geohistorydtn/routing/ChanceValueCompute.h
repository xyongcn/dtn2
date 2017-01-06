#include<string>
#include "ValidVector.h"
#include "servlib/geohistorydtn/config/ChanceComputeConfig.h"

#ifndef NEIGHBOURMANAGER_H_
#define NEIGHBOURMANAGER_H_
#include"servlib/geohistorydtn/neighbour/NeighbourManager.h"
#endif

using namespace std;
/**

 * 说明  ：用来作为路由选择时计算机会值的类
 * 机会值计算公式： 	1.根据bundle的失效时间和当前时间计算出三个不同尺度的有效时间向量，如果某一时间段bundle有效则对应分量为1，如果无效则为0
 * 				2.将不同尺度的有效时间向量乘以不同尺度的频率向量（邻居或者区域），得到的不同尺度的机会值，
 * 					当所需要的尺度的机会值大于相应的阈值的时候，把各个尺度机会值相加，然后返回.如果期间的有效时间向量超时或者其他的错误，则返回-1;
 */
namespace dtn
{
class ChanceValueCompute
{
private:
		/**
	 * 节点携带到达目的地的机会值
	 * @param bundle :为需要发送的bundle
	 * @param area :计算携带到给区域(该区域一般是两个节点)的机会值，包括本区域以及上层区域的机会值。
	 * @return double[] :携带到本层及其父类的区域的机会值；如果存在且合法返回向量；如果不合法返回null
	 * 在每一层区域的机会值计算中，如果计算出来的机会值向量所需要的分量大于阈值，则返回机会值；否则返回-1；
	 */
public:
	static const string tag;

	static vector<double> histroyNeighbourCarryChance(Bundle *bundle,Area *area,Neighbour *neighbour);
	static double historyNeighbourcarryChance(ValidVector *validVector,Area *area,Neighbour *neighbour);
	static vector<double> carryChance(Bundle *bundle,Area *area);
	/**
	 * 作用：	计算携带到Area区域的机会值
	 * @param validVector :bundle的时效向量
	 * @param area
	 * @return
	 */
	static double carryChance(ValidVector *validVector,Area *area);

	/**
	 * 计算向量之间相乘
	 * @return :如果向量为空或者两者长度不等，返回null；否则返回
	 */
    static vector<double> vectorMultiplyvector(vector<double> a,vector<double> b);

	//根据bundle的生命周期，生成每种尺度的频率向量，且将这些频率向量在有效bundle生命周期中为1
	static ValidVector *getValidVector(Bundle *bundle);

	static double vectorSum(vector<double> a);


};
}
