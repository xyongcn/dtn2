class Point
{
public:
	 int id;
	 double longitude;
	 double latitude;

	Point()
	{
		this->id=0;
		this->longitude=0;
		this->latitude=0;

	}

	Point(int id, double longitude,double latitude)
	{
		this->id=id;
		this->longitude=longitude;
		this->latitude=latitude;
	}
	Point(const Point &p)
	{
		this->id=p.id;
		this->longitude=p.longitude;
		this->latitude=p.latitude;
	}
	void copy(Point p)
	{
		this->id=p.id;
		this->longitude=p.longitude;
		this->latitude=p.latitude;
	}

	Point& operator=(const Point &p)
	{
		this->id=p.id;
		this->longitude=p.longitude;
		this->latitude=p.latitude;
		return *this;
	}

};
