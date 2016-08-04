#include<string>
/**
 * 对邻居相关的配置：系统内保存邻居的数目，系统内保留邻居区域的个数，需要保留区域信息的历史邻居个数
 *
 */

namespace dtn
{
class NeighbourConfig
{
	/**
	 * 保留的历史邻居个数
	 */
public:
	//NeighborManager中存储在hash表的neighbour个数
	const static int NEIGHOURNUM=100;
	/**
	 * 保留的具有区域信息的历史邻居个数，该数目不能超过NEIGHBOURNUM
	 */
	const static int NEIGHBOURAREANUM=10;

	/**
	 * 保留的区域信息的邻居所保留的区域个数（也是邻居之间要交换的区域的个数）
	 */
	const static int NEIGHBOURKEEPAREANUM=200;

	/**
	 * 存放邻居的历史区域信息的path
	 */
	static std::string NEIGHBOURAREAFILEDIR;
};
}
