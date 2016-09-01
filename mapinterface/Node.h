class Node
{
public:
	int idNode;
	int lon;
	int lat;
	int position;

	Node(int idNode,int lon,int lat,int position)
	{
		this->idNode=idNode;
		this->lon=lon;
		this->lat=lat;
		this->position=position;
	}

	Point toPoint()
	{
		Point p((double)lon/1000000,(double)lat/1000000);
		return p;
	}

	bool operator==(const Node &n)
	{
		if(this->idNode==n.idNode &&
			this->lon==n.lon &&
			this->lat==n.lat &&
			this->position==n.position )
			return true;
		return false;
	}
};
