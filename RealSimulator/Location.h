class Location
{
public:
	double longitude;
	double latitude;

	Location()
	{
		longitude=0;
		latitude=0;
	}

	void setLatitude(double latitude)
	{
		this->latitude=latitude;
	}

	void setLongitude(double longitude)
	{
		this->longitude=longitude;
	}
	double getLatitude()
	{
		return this->latitude;
	}
	double getLongitude()
	{
		return this->longitude;
	}

};
