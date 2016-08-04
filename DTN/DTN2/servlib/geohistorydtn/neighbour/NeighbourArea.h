#include "servlib/geohistorydtn/config/NeighbourConfig.h"
#include<fstream>
#include<sys/stat.h>
#include <boost/serialization/map.hpp>
#include<map>
//#include <boost/serialization/tracking.hpp>

//#include "servlib/geohistorydtn/Log/GeohistoryLog.h"


#ifndef AREAMANAGER_H_
#define AREAMANAGER_H_
#include "servlib/geohistorydtn/area/AreaManager.h"
#endif




/**
 * 说明  : 记录一个邻居所到过的所有区域的信息，区域的频率信息
 */

namespace dtn
{

#define Areamap map<string,Area>

//BOOST_CLASS_TRACKING(Area,boost::serialization::track_never)

//BOOST_CLASS_TRACKING(Areamap,boost::serialization::track_never)

class BundlePayload;
class EndpointID;
class Bundle;

class NeighbourArea
{
private:
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        //ar & tag;//序列化或反序列化&操作符比>>和<<更为方便
        ar & neighbourEIDstr;
        ar & areaMap;
    }

private:
	static const string tag;
	GeohistoryLog *geohistoryLog;
	NeighbourArea(){}
	/**
	 * 利用本地文件来更新，读取eid所对应的区域文件，然后添加到对应的Area列表中
	 */
	void init();

	/**
	 * 利用收到邻居的区域信息的bundle来初始化本地的邻居历史区域信息
	 * @param eid
	 * @param payload
	 */
	void init(string eid,BundlePayload payload);
public:
	/**
	 * 该邻居的区域向量记录
	 */
	EndpointID neighbourEID;
	string neighbourEIDstr;

	/**
	 * 用来存放所有的Area的hashmap
	 */
	map<string,Area> areaMap;

	 NeighbourArea(EndpointID eid);

	/**
	 * 利用邻居区域移动频率的文件来更新频率文件
	 * @param payloadFile
	 * @throws IOException
	 * @throws FileNotFoundException
	 * @throws StreamCorruptedException
	 * @throws ClassNotFoundException
	 */
	void updateArea(fstream &payloadFile);



	/**
	 * 作用 ：根据bundle的目的节点，查询当前邻居节点中离目的地最近的区域（主要是尽可能低的区域层次）
	 * @param bundle :需要对比目的节点的bundle
	 * @return :如果找到了同一层次的区域则返回该层次区域；如果该邻居区域信息为空，则为null；一般情况下只要有区域信息就能返回区域信息的。
	 */
	Area *checkBundleDestArea(Bundle bundle);

	/**
	 * 根据area查询当前
	 * @param area
	 * @return
	 */
	Area *checkArea(Area area);
};
}
