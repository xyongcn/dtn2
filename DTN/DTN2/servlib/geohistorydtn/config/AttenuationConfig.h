#include"servlib/geohistorydtn/frequencyVector/FrequencyVectorLevel.h"
#include"servlib/geohistorydtn/frequencyVector/FrequencyVectorServiceType.h"
/**
 * 说明 :用来维护相应的衰减系数
 */
class AttenuationConfig {
public:

	/**
	 * 返回需要的衰减参数
	 * @param serviceType：向量服务于哪一种类别
	 * @param vectorLevel：向量属于哪一种时间尺度
	 * @return
	 */
	static double getAttenuation(int vectorLever,int serviceType)
	{
		if(serviceType==FrequencyVectorServiceType::AREA)
		{
			switch(vectorLever)
			{

			case FrequencyVectorLevel::minuteVector:
				return 1;

			case FrequencyVectorLevel::hourVector:
				return 0.99;

			case FrequencyVectorLevel::monAftEveVector:
				return 0.97;

			case FrequencyVectorLevel::weekVector:
				return 0.9;

			case FrequencyVectorLevel::monthVector:
				return 0.95;
			}
		}
		else if(serviceType==FrequencyVectorServiceType::NEIGHBOUR)
		{
			switch(vectorLever)
			{

				case FrequencyVectorLevel::minuteVector:
					return 1;

				case FrequencyVectorLevel::hourVector:
					return 0.99;

				case FrequencyVectorLevel::monAftEveVector:
					return 0.97;

				case FrequencyVectorLevel::weekVector:
					return 0.9;

				case FrequencyVectorLevel::monthVector:
					return 0.95;
				}
		}
		return 0.9;
	}

};
