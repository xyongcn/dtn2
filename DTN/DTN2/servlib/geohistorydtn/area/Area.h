#include<list>
//#include "geohistorydtn/frequencyVector/FrequencyVectorManager.h"
//#include "geohistorydtn/frequencyVector/FrequencyVectorServiceType.h"
//#include "AreaLevel.h"
#include "servlib/geohistorydtn/timeManager/TimeManager.h"
using namespace std;



/**
 * 作用:用来描述区域的对象
 *
 */
namespace dtn{

class Area
{
private:
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & isCurrent;//序列化或反序列化&操作符比>>和<<更为方便
        ar & level;
        ar & id;
        ar & vectorlist;
        ar & father;
    }


public:
	/**
	 * 该区域是否是是当前所在区域
	 */
	bool isCurrent;

	/**
	 * 区域的层次
	 */
	int level;

	/**
	 * 区域所在层次的id
	 */
	int id;

	/**
	 * 频率向量列表表
	 */
	list<FrequencyVector *> vectorlist;
	/*FrequencyVector hourvector;
	FrequencyVector monaftevevector;
	FrequencyVector weekvector;
	FrequencyVector monthvector;*/


	/**
	 * 父区域
	 */
	string father;

	Area()
	{}

	Area(int level,int id,string fatherArea)
	{
		this->level=level;
		this->id=id;
		this->isCurrent=false;
		this->father=fatherArea;

	//	init(FrequencyConfig::frequcyType);
	}

	/**
	 * 需要重新实现它的father
	 * @param level
	 * @param id
	 */
	//Area(int level,int id,vector<int> frequencyType)
	Area(int level,int id)
	{
		this->level=level;
		this->id=id;
		this->isCurrent=false;
	//	init(FrequencyConfig::frequcyType);
	}

	void init()
	{
		/**
		 * 加入频率向量，根据区域的层次建立相应的频率向量
		 */

		vector<int> frequencyType(FrequencyConfig::frequcyType);
		int areatype=FrequencyVectorServiceType::AREA;
		for(int i=0;i<frequencyType.size();++i)
		{
			if(frequencyType[i]==FrequencyVectorLevel::minuteVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::minuteVector,areatype));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}
			if(frequencyType[i]==FrequencyVectorLevel::hourVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::hourVector,areatype));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}

			if(frequencyType[i]==FrequencyVectorLevel::weekVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::weekVector,areatype));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}
			if(frequencyType[i]==FrequencyVectorLevel::monthVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::monthVector,areatype));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}
			if(frequencyType[i]==FrequencyVectorLevel::monAftEveVector)
			{
				vectorlist.push_back(FrequencyVectorManager::Getinstance()->newFVector(FrequencyVectorLevel::monAftEveVector,areatype));
				FrequencyVectorManager::Getinstance()-> addFVector((vectorlist.back()));
			}

		}
	}



	/**
	 * 判断是不是同一个area，不只是通过对象引用比较，因为在不同区域间也是要进行比较的，所以应该有每个区域的标识
	 *
	 */
	bool equals(Area *other)
	{

		if(other->level==this->level && other->id==this->id)
			return true;
		else
				return false;
	}

	int getAreaId()
	{
		return id;
	}

	int getAreaLevel()
	{
		return this->level;
	}

	Area *getFatherArea();

	void setFatherArea(string areaid)
	{
		father=areaid;
	}

	/**
	 * 设置区域为当前所在区域
	 */
	void setCurrent()
	{
		this->isCurrent=true;
	}

	/**
	 * 获取该区域是否是当前所在区域
	 * @return
	 */
	bool isCurrentArea()
	{
		return isCurrent;
	}

	/**
	 * 将该区域设置为非当前区域
	 */
	void cancleCurrent()
	{
		isCurrent=false;
	}

	/**
	 * 将本区域的频率向量加入到计时器队列
	 */
	void addTimeCount()
	{
		setCurrent();
		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
				it!=vectorlist.end();++it)
		{
			FrequencyVector *v=*it;
			TimeManager::GetInstance()->addVectorListen(v);
		}
	}

	/**
	 * 将本区域的频率向量移除计时器队列
	 * 将本区域的频率向量的修改标识重置，这样即使这个区域再次移动进入了也不会出现不更新频率向量
	 */
	void removeTimeCount()
	{
		cancleCurrent();
		cout<<"remove"<<endl;
		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
				it!=vectorlist.end();++it)
		{
			FrequencyVector *v=*it;
			TimeManager::GetInstance()->removeVectorListen(v);
			(*it)->resetChangeFVectorSign();//更新频率向量的修改标志位
		}
	}

	/**
	 * 对区域有关的频率向量改变相应的频率
	 * @param info:主要是利用AreaInfo里面的时间来改变频率相当当前时间段的频率
	 */
	void changeFVector(AreaInfo info);

	/**
	 * 作用 ：根据bundle的目的节点，查询当前区域及其父类区域中离目的地最近的区域（主要是尽可能低的区域层次）
	 * @param bundle :需要对比目的节点的bundle
	 * @return :如果父类区域中有和bundle的目的地同一层的区域;则返回该区域；如果没有则返回null;
	 */
	 Area *checkBundleDestArea(Bundle bundle)
	{
		/*switch(this->getAreaLevel())
		{
		case AreaLevel::TWELVELEVEL:
			if(this->getAreaId()==bundle.twelveArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}

		case AreaLevel::ELEVENLEVEL:
			if(this->getAreaId()==bundle.elevenArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::TENLEVEL:
			if(this->getAreaId()==bundle.tenArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::NINELEVEL:
			if(this->getAreaId()==bundle.nineArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}

		case AreaLevel::EIGHTLEVEL:
			if(this->getAreaId()==bundle.eightArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::SEVENLEVEL:
			if(this->getAreaId()==bundle.sevenArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::SIXLEVEL:
			if(this->getAreaId()==bundle.sixArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::FIVELEVEL:
			if(this->getAreaId()==bundle.fiveArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::FOURLEVEL:
			if(this->getAreaId()==bundle.fourArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::THIRDLEVEL:
			if(this->getAreaId()==bundle.thirdArea())
				return this;
			else
			{
				if(this->getFatherArea().id==0)
					return NULL;
				else
					return this->getFatherArea().checkBundleDestArea(bundle);
			}
		case AreaLevel::SECONDLEVEL:
				if(this->getAreaId()==bundle.secondArea())
					return this;
				else
				{
					if(this->getFatherArea().id==0)
						return NULL;
					else
						return this->getFatherArea().checkBundleDestArea(bundle);
				}
		case AreaLevel::FIRSTLEVEL:
				if(this->getAreaId()==bundle.firstArea())
					return this;
				else
				{
					if(this->getFatherArea().id==0)
						return NULL;
					else
						return this->getFatherArea().checkBundleDestArea(bundle);
				}

		default:
			return NULL;
		}
*/
	}

	/**
	 * @return : 获取该区域的频率向量列表
	 */
	list<FrequencyVector *> getFrequencyVectorList()
	{
		return vectorlist;
	}

	 string toString() {
		string s="";
		s.append("level and id:%s#%d,", level,id);

		for(list<FrequencyVector *>::iterator it=vectorlist.begin();
				it!=vectorlist.end();++it)
		{
			switch((*it)->getFrequencyLevel())
			{

			case FrequencyVectorLevel::minuteVector:
				s.append("minuteVector:(%s),", (*it)->toString().c_str());
				break;

			case FrequencyVectorLevel::hourVector:
				s.append("hourVector:(%s),",(*it)->toString().c_str());
				break;

			case FrequencyVectorLevel::weekVector:
				s.append("\n");
				s.append("weekVector:(%s),", (*it)->toString().c_str());
				break;

			case FrequencyVectorLevel::monthVector:
				s.append("\n");
				s.append("monthVector:(%s)", (*it)->toString().c_str());
				break;

			case FrequencyVectorLevel::monAftEveVector:
				s.append("\n");
				s.append("monafteveVector:(%s)",(*it)->toString().c_str());
				break;
			}
		}
		s.append("\n");
		return s;
//		return super.toString();
	}
};
}
