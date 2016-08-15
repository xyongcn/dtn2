#include<vector>
using namespace std;

class AreaLayerInfo
{
public:
	vector<int> arealist;
	bool equals(int o) {
		/*if(this == o)
			return true;


		AreaLayerInfo other=(AreaLayerInfo)o;
		if(this.size()!=other.size()){
			return false;
		}

		for(int i=0;i<this.size();i++){
			if(this.get(i)!=other.get(i))
				return false;
		}*/
		return true;
	}
};

