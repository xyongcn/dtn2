 class Point
{

 public:
	 double longitude;
	 double latitude;

	 Point()
	 {
		this->longitude=0;
		this->latitude=0;
	 }

	 Point(double lon,double lat)
	 {
		this->longitude=lon;
		this->latitude=lat;
	 }


	 Point& operator=(const Point &p)
	 {
			this->longitude=p.longitude;
			this->latitude=p.latitude;
	 }
};
