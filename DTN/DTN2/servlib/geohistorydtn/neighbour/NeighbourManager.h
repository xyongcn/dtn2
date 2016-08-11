/**
 * 说明  ：作为邻居类的管理器
 *
 * 尚未完成：没有将当前邻居的频率向量加入到计时器中
 */
#include<string>
#include<list>

#ifndef NEIGHBOUR_H_
#define NEIGHBOUR_H_
#include"Neighbour.h"
#endif

namespace dtn{
class NeighbourManager {

	private:
	static NeighbourManager *instance;
	static const string tag;

	/**
	 * 历史邻居的表
	 */
	hash_map<string, Neighbour> neighbourlist;

	/**
	 * 保存历史邻居对象的文件名称
	 * @author wwtao
	 */
	static string historyNeighbourFileName;
	static string historyNeighbourFileDirectory;

	NeighbourManager();

	/**
	 * 从文件中获取历史的邻居的记录，包括历史邻居的
	 */
public:
	GeohistoryLog *geohistoryLog;

	static NeighbourManager *Getinstance()
	{
		if(instance==NULL)
			instance=new NeighbourManager();
		return instance;
	}

	void init();
	/**
	 * 将历史的邻居记录保存到文件中，以便下一次访问
	 */
	void saveHistoryNeighbour();



	/**
	 * 关闭邻居管理器所要进行的必要步骤和操作，包括清空已有的邻居表，保存历史的邻居信息
	 */
	void shutdown();
	/**
	 * 遇到新邻居后进行check，如果没有该邻居将其建立
	 */
	Neighbour *checkNeighbour(EndpointID eid);

	/**
	 * 找到该eid的Neichbour
	 */
	Neighbour *getNeighbour(EndpointID eid);

	list<Neighbour *> getAllNeighbour();

	void printAllNeighbour();

	/**
	 * 将邻居发来的区域频率信息的bundle的payload内容保存下来
	 * @param payload：bundle的内容
	 * @param eid：发来bundle的邻居的eid
	 */
	void saveNeighbourAreaPayload(BundlePayload payload,string eid);

};
}
