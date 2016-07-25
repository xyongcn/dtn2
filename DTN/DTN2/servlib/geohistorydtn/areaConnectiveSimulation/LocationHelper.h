//将经纬度由int的表示转为double型，或者有double型转为int
#include<math.h>

class  LocationHelper
{
public:
	static const int accuracy = 6;

	static double gpsint2double(int x){
		return (double)x /pow(10, accuracy);
	}

	static int gpsdouble2int(double x){
		return (int)(x * pow(10, accuracy));
	}
};
