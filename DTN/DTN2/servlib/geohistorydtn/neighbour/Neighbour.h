#include"NeighbourArea.h"


using namespace std;
/**
 * 说明  :邻居类，用来表示DTN总的邻居
 */
namespace dtn
{
class Neighbour
{

private:
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
       // ar & tag;//序列化或反序列化&操作符比>>和<<更为方便
        ar & vectorlist;
        ar & neighbourArea;
        ar & neighbourEidstr;
    }



private:
	static const string tag;


	//频率向量,与历史邻居相遇的频率向量
	//HourFrequencyVector hourvector;
	//WeekFrequencyVector weekvector;
	//MonthFrequencyVector monthvector;
	list<FrequencyVector *> vectorlist;

	//区域频率向量管理类
	NeighbourArea *neighbourArea;
	//该变量避免序列化是需要侵入类EndpointID
	GeohistoryLog *geohistoryLog;

public:

	//邻居的EID
	EndpointID neighbourEid;
	string neighbourEidstr;


	Neighbour(){}

	Neighbour(EndpointID eid);
	/**
	 * 根据本地文件保存的邻居的区域信息来更改区域记录
	 */
	void init();


	/**
	 * 根据邻居的bundle的payload来生成自己的
	 * @param payloadFile
	 */
	void generateArea();


	NeighbourArea *getNeighbourArea();

	/**
	 * 对向量在该时间段的分量进行处理
	 */
	void checkVectorChange();

	/**
	 * 将频率向量加入到计时器中(没有被调用，应该需要添加)
	 * 这里是让计时器来每段时间都改变一下频率向量，改变小时频率向量；星期频率向量；月频率向量
	 */
	void addTimeCount();

	/**
	 * 将Neighbour中的所有频率向量添加到频率向量管理器中，这样从文件中读取邻居时，频率向量的衰减器也可以对其进行频率的衰减了
	 */
	void addAreaFVector2Manager();

	/**
	 * 将频率向量从计时器中移除
	 */
	void removeTimeCount();

	/**
	 * 获取与历史邻居相遇的各个级别频率向量
	 */
	FrequencyVector *getFrequencyVector(int level);



	/**
	 * 获取该邻居的EID
	 * @return
	 */
	EndpointID getEid();

	/**
	 * 输出保存的区域的星期
	 */
	void printAreaInfo();

	string toString();
};
}
