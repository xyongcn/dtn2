#include"Point.h"
#include<vector>

using namespace std;

class Way
{
public:
	vector<Point> PointList;
	int id;

	Way()
	{
		this->id=0;
	}

	Way& operator=(const Way &w)
	{
		this->id=w.id;
		this->PointList=w.PointList;
		return *this;

	}


};
