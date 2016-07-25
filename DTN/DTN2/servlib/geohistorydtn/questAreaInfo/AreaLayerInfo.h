/**
 * 用来记录这个GPS所在的各个行政区分层的信息
 * 作为收到区域分层信息后返回分层的信息
 *
 */
#include<vector>
#include<string>
#include<stdio.h>


using namespace std;
namespace dtn{
class AreaLayerInfo
{
public:
	vector<int> arealist;

	string toString()
	{
		string s;
		char c[20];
		//s.append("areainfo（从顶层到底层）:");
		sprintf(c,"%d",arealist[0]);
		s.append(c);
		for(int i=1;i<arealist.size();i++)
		{
			sprintf(c,"%d",arealist[i]);
			s.append(" , ");
			s.append(c);
		}

			return s;
	}
		/*bool equals(Object o) {
		if(this == o)
			return true;

		if(! (o instanceof AreaLayerInfo)){
			return false;
		}

		AreaLayerInfo other=(AreaLayerInfo)o;
		if(this.size()!=other.size()){
			return false;
		}

		for(int i=0;i<this.size();i++){
			if(this.get(i)!=other.get(i))
				return false;
		}
		return true;
	}*/
};
}
