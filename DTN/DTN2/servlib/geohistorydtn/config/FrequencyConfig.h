#include<sys/time.h>
#include<time.h>
#include <cstddef>
#include <vector>
using namespace std;
/**
 * 说明 ：用来配置和频率向量有关的参数
 */
 class FrequencyConfig {
public:

	//实际时间与已有记录时间的倍率。倍率为大于1的整数，已有的记录的时间差除以倍率就可得到现在比较的时间
	static const int timeZoom=60;

	static const int moning=1;
	static const int afternoon=2;
	static const int evening=3;

	static vector<int> frequcyType;
	tm s;



	// querry current time
	static long long getCurrentTimeInMillis()
	{
	   struct timeval tv;
	   gettimeofday(&tv,NULL);
	   long long t=(long long)tv.tv_sec * 1000+(int)(tv.tv_usec/1000);
	   return t;
	}

	static tm getCurrentTime()
	{
		long long time=getCurrentTimeInMillis();
		long temp=long(time/1000);
		tm *t=localtime(&temp);
		return *t;
	}


};

