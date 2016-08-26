#include "ChanceValueCompute.h"

namespace dtn
{
	const string ChanceValueCompute::tag="ChanceValueCompute";

	/**
	* 节点通过历史邻居携带到达目的地的机会值
	* @param bundle :为需要发送的bundle
	* @param area :计算携带到给区域(该区域一般是两个节点)的机会值，包括本区域以及上层区域的机会值。
	* @param neighbour :历史邻居的neighbour，用来提供与历史邻居相遇的频率向量
	* @return double[] :携带到本层及其父类的区域的机会值；如果存在且合法返回向量；如果不合法返回null
	* 在每一层区域的机会值计算中，如果计算出来的机会值向量所需要的分量大于阈值，则返回机会值；否则返回-1；
	 */

	vector<double> ChanceValueCompute::histroyNeighbourCarryChance(Bundle *bundle,Area *area,Neighbour *neighbour)
	{
		ValidVector *validVector=ChanceValueCompute::getValidVector(bundle);
		//存储地图每一层的机会值
		vector<double> chanceValue;
		vector<double> error_chanceValue;
		while(area!=NULL)
		{
			double temp=ChanceValueCompute::historyNeighbourcarryChance(validVector, area, neighbour);
			//出现不合法的机会值
			if(temp==-1)
				return error_chanceValue;
			chanceValue.push_back(temp);
			area=area->getFatherArea();
		}
		return chanceValue;
	}

	double ChanceValueCompute::historyNeighbourcarryChance(ValidVector *validVector,Area *area,Neighbour *neighbour)
	{
		list<FrequencyVector *> vectorList=area->getFrequencyVectorList();

		//到达area区域的机会值总和
		double chanceValue=0;

		//到达area区域，不同尺度机会值向量
		vector<double> temp;

		for(list<FrequencyVector *>::iterator it=vectorList.begin();
				it!=vectorList.end();++it)
		{

			switch((*it)->getFrequencyLevel())
			{
				case FrequencyVectorLevel::minuteVector:
					//到达Area区域，分钟级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getMinuteVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::MINUTE_LEVEL_AREA_THRESHOLD)
						{
							//乘以与历史邻居相遇的衰减
							double minuteDenominator=ChanceComputeConfig::MINUTE_LEVEL_NEIGHBOUR_DENOMINATOR;
							if(validVector->getMinuteVector().size()!=0)
							{
								minuteDenominator=minuteDenominator*validVector->getMinuteVector().size();
							}
							double minuteTemp=vectorSum(vectorMultiplyvector(validVector->getMinuteVector(),neighbour->getFrequencyVector(FrequencyVectorLevel::minuteVector)->getVector()))/minuteDenominator;
							sum*=minuteTemp;

							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;

				case FrequencyVectorLevel::hourVector:
					//到达Area区域，分钟级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getHourVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::HOUR_LEVEL_AREA_THRESHOLD)
						{
							//乘以与历史邻居相遇的衰减
							double hourDenominator=ChanceComputeConfig::HOUR_LEVEL_NEIGHBOUR_DENOMINATOR;
							if(validVector->getHourVector().size()!=0)
							{
								hourDenominator=hourDenominator*validVector->getHourVector().size();
							}
							double hourTemp=vectorSum(vectorMultiplyvector(validVector->getHourVector(),neighbour->getFrequencyVector(FrequencyVectorLevel::hourVector)->getVector()))/hourDenominator;
							sum*=hourTemp;

							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;

				case FrequencyVectorLevel::weekVector:
					//到达Area区域，星期级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getWeekVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::WEEK_LEVEL_AREA_THREASHOLD)
						{
							//乘以与历史邻居相遇的衰减
							double weekDenominator=ChanceComputeConfig::WEEK_LEVEL_NEIGHBOUR_DENOMINATOR;
							if(validVector->getWeekVector().size()!=0)
							{
								weekDenominator=weekDenominator*validVector->getWeekVector().size();
							}
							double weekTemp=vectorSum(vectorMultiplyvector(validVector->getWeekVector(),neighbour->getFrequencyVector(FrequencyVectorLevel::weekVector)->getVector()))/weekDenominator;
							sum*=weekTemp;

							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;
				case FrequencyVectorLevel::monthVector:
					//到达Area区域，月级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getMonthVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::MONTH_LEVEL_AREA_THRESHOLD)
						{
							//乘以与历史邻居相遇的衰减
							double monthDenominator=ChanceComputeConfig::MONTH_LEVEL_NEIGHBOUR_DENOMINATOR;
							if(validVector->getMonthVector().size()!=0)
							{
								monthDenominator=monthDenominator*validVector->getMonthVector().size();
							}
							double monthTemp=vectorSum(vectorMultiplyvector(validVector->getMonthVector(),neighbour->getFrequencyVector(FrequencyVectorLevel::monthVector)->getVector()))/monthDenominator;
							sum*=monthTemp;

							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;
				case FrequencyVectorLevel::monAftEveVector:
					//到达Area区域，上午下午晚上级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getMonAftEveVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::MONAFTEVE_LEVEL_AREA_THRESHOLD)
						{
							double monafteveDenominator=ChanceComputeConfig::MONAFTEVE_LEVEL_NEIGHBOUR_DENOMINATOR;
							if(validVector->getMonAftEveVector().size()!=0)
							{
								monafteveDenominator=monafteveDenominator*validVector->getMonAftEveVector().size();
							}
							double monafteveTemp=vectorSum(vectorMultiplyvector(validVector->getMonAftEveVector(),neighbour->getFrequencyVector(FrequencyVectorLevel::monAftEveVector)->getVector()))/monafteveDenominator;
							sum*=monafteveTemp;

							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;

				default:
					//不合法的频率向量
					return -1;
				}
		}
		return chanceValue;

	}

	//在bundle的生命周期内，到达Area及其父区域的机会值
	vector<double> ChanceValueCompute::carryChance(Bundle *bundle,Area *area)
	{
		ValidVector *validVector=getValidVector(bundle);
		//存储地图每一层的机会值
		vector<double> chanceValue;
		vector<double> error_chanceValue;
		while(area!=NULL)
		{
			double temp=carryChance(validVector, area);
			//出现不合法的机会值
			if(temp==-1)
				return error_chanceValue;
			chanceValue.push_back(temp);
			area=area->getFatherArea();
		}

		return chanceValue;



	}


	/**
		 * 作用：	计算携带到Area区域的机会值
		 * @param validVector :bundle的时效向量
		 * @param area
		 * @return
		 */
	double ChanceValueCompute::carryChance(ValidVector *validVector,Area *area)
	{
		list<FrequencyVector *> vectorList=area->getFrequencyVectorList();

		//到达area区域的机会值总和
		double chanceValue=0;

		//到达area区域，不同尺度机会值向量
		vector<double> temp;

		for(list<FrequencyVector *>::iterator it=vectorList.begin();
				it!=vectorList.end();++it)
		{

			switch((*it)->getFrequencyLevel())
			{
				case FrequencyVectorLevel::minuteVector:
					//到达Area区域，分钟级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getMinuteVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::MINUTE_LEVEL_AREA_THRESHOLD)
						{
							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;

				case FrequencyVectorLevel::hourVector:
					//到达Area区域，分钟级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getHourVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::HOUR_LEVEL_AREA_THRESHOLD)
						{
							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
						break;

				case FrequencyVectorLevel::weekVector:
					//到达Area区域，星期级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getWeekVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::WEEK_LEVEL_AREA_THREASHOLD)
						{
							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;
				case FrequencyVectorLevel::monthVector:
					//到达Area区域，月级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getMonthVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::MONTH_LEVEL_AREA_THRESHOLD)
						{
							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;
				case FrequencyVectorLevel::monAftEveVector:
					//到达Area区域，上午下午晚上级别的机会值
					temp=ChanceValueCompute::vectorMultiplyvector(validVector->getMonAftEveVector(),(*it)->getVector());
					if(temp.empty())
						return -1;
					else
					{
						double sum=ChanceValueCompute::vectorSum(temp);
						if(sum>ChanceComputeConfig::MONAFTEVE_LEVEL_AREA_THRESHOLD)
						{
							chanceValue+=sum;
						}
						else
						{
							//该尺度机会值不满足阈值要求
							return -1;
						}
					}
					break;

				default:
					//不合法的频率向量
					return -1;
				}
			}
			return chanceValue;
		}

	/**
	 * 计算向量之间相乘
	 * @return :如果向量为空或者两者长度不等，返回null；否则返回
	 */
	vector<double> ChanceValueCompute::vectorMultiplyvector(vector<double> a,vector<double> b)
	{
		vector<double> result;
		if(a.empty() || b.empty())
		{
			return result;
		}

		//两向量不合法
		if(a.size()!=b.size())
			return result;


		for(int i=0;i<a.size();i++)
		{
			result.push_back(a[i]*b[i]);
		}

		return result;

	}

    //根据bundle的生命周期，生成每种尺度的频率向量，且将这些频率向量在有效bundle生命周期中为1
	ValidVector *ChanceValueCompute::getValidVector(Bundle *bundle)
	{
		long expirationSeconds=bundle->creation_ts().seconds_+bundle->expiration()+BundleTimestamp::TIMEVAL_CONVERSION;

		long nowSeconds=BundleTimestamp::get_current_time()+BundleTimestamp::TIMEVAL_CONVERSION;

		if(nowSeconds>=expirationSeconds)
		{
			GeohistoryLog::GetInstance()->LogAppend(GeohistoryLog::GetInstance()->INFO_LEVEL,
					"该bundle%d,已经超时却进入了小时机会值计算",bundle->bundleid());
			return NULL;
		}
		else
		{
			vector<double> ValidMinuteVector;
			vector<double> ValidHourVector;
			vector<double> ValidMonafteveVector;
			vector<double> ValidWeekVector;
			vector<double> ValidMonthVector;
			vector<int> frequencyType(FrequencyConfig::frequcyType);
			for(int i=0;i<frequencyType.size();++i)
			{
				if(frequencyType[i]==FrequencyVectorLevel::minuteVector)
				{
					ValidMinuteVector.insert(ValidMinuteVector.end(),60,0);
				}
				if(frequencyType[i]==FrequencyVectorLevel::hourVector)
				{
					ValidHourVector.insert(ValidHourVector.end(),24,0);
				}

				if(frequencyType[i]==FrequencyVectorLevel::weekVector)
				{
					ValidWeekVector.insert(ValidWeekVector.end(),7,0);
				}
				if(frequencyType[i]==FrequencyVectorLevel::monthVector)
				{
					ValidMonthVector.insert(ValidMonthVector.end(),12,0);
				}
				if(frequencyType[i]==FrequencyVectorLevel::monAftEveVector)
				{
					ValidMonafteveVector.insert(ValidMonafteveVector.end(),3,0);
				}
			}

			time_t ttime_now =nowSeconds;
			struct tm* tt_now;
			tt_now=localtime(&ttime_now);

			time_t ttime_exp =expirationSeconds;
			struct tm* tt_exp;
			tt_exp=localtime(&ttime_exp);

			//将当前时间和有效时间段内的值赋为1
			for(int i=0;i<frequencyType.size();++i)
			{
				if(frequencyType[i]==FrequencyVectorLevel::minuteVector)
				{

					if((tt_now->tm_hour+1)%24 < tt_exp->tm_hour)
					{
						for(int i=0;i<60;++i)
							ValidMinuteVector[i]=1;
					}
					else if((tt_now->tm_hour+1)%24 ==tt_exp->tm_hour && tt_now->tm_min<=tt_exp->tm_min)
					{
						for(int i=0;i<60;++i)
							ValidMinuteVector[i]=1;
					}
					else if((tt_now->tm_hour+1)%24 ==tt_exp->tm_hour && tt_now->tm_min>tt_exp->tm_min)
					{
						for(int i=tt_now->tm_min;i<60;++i)
							ValidMinuteVector[i]=1;
						for(int i=0;i<=tt_exp->tm_min;++i)
							ValidMinuteVector[i]=1;
					}
					else if(tt_now->tm_hour==tt_exp->tm_hour)
					{
						for(int i=tt_now->tm_min;i<=tt_exp->tm_min;++i)
							ValidMinuteVector[i]=1;
					}

				}
				if(frequencyType[i]==FrequencyVectorLevel::hourVector)
				{
					if(tt_now->tm_hour > tt_exp->tm_hour)
					{
						for(int i=tt_now->tm_hour;i<24;++i)
							ValidHourVector[i]=1;
						for(int i=0;i<=tt_exp->tm_hour;++i)
							ValidHourVector[i]=1;
					}
					else
						for(int i=tt_now->tm_hour;i<=tt_exp->tm_hour;++i)
							ValidHourVector[i]=1;
				}

				if(frequencyType[i]==FrequencyVectorLevel::weekVector)
				{
					if(tt_now->tm_wday > tt_exp->tm_wday)
					{
						for(int i=tt_now->tm_wday;i<7;++i)
							ValidWeekVector[i]=1;
						for(int i=0;i<=tt_exp->tm_wday;++i)
							ValidWeekVector[i]=1;
					}
					else
						for(int i=tt_now->tm_wday;i<=tt_exp->tm_wday;++i)
							ValidWeekVector[i]=1;
				}
				if(frequencyType[i]==FrequencyVectorLevel::monthVector)
				{
					if(tt_now->tm_mon > tt_exp->tm_mon)
					{
						for(int i=tt_now->tm_mon;i<12;++i)
							ValidMonthVector[i]=1;
						for(int i=0;i<=tt_exp->tm_mon;++i)
							ValidMonthVector[i]=1;
					}
					else
						for(int i=tt_now->tm_mon;i<=tt_exp->tm_mon;++i)
							ValidMonthVector[i]=1;
				}
				if(frequencyType[i]==FrequencyVectorLevel::monAftEveVector)
				{
					int monafteve_now;
					int monafteve_exp;

					if(tt_now->tm_hour >=6 && tt_now->tm_hour <12)
						monafteve_now=1;
					else if(tt_now->tm_hour >=12 && tt_now->tm_hour <20)
						monafteve_now=2;
					else
						monafteve_now=3;

					if(tt_exp->tm_hour >=6 && tt_exp->tm_hour <12)
						monafteve_exp=1;
					else if(tt_exp->tm_hour >=12 && tt_exp->tm_hour <20)
						monafteve_exp=2;
					else
						monafteve_exp=3;

					if(monafteve_now > monafteve_exp)
					{
						for(int i=monafteve_now;i<3;++i)
							ValidMonthVector[i]=1;
						for(int i=0;i<=monafteve_exp;++i)
							ValidMonafteveVector[i]=1;
					}
					else
						for(int i=monafteve_now;i<=monafteve_exp;++i)
							ValidMonafteveVector[i]=1;
				}
			}
			ValidVector *vector=new ValidVector(ValidMinuteVector,ValidHourVector
					,ValidMonafteveVector,ValidWeekVector, ValidMonthVector);
			return vector;
		}
	}

	double ChanceValueCompute::vectorSum(vector<double> a)
	{
		if(a.empty())
			return -1;

		double sum=0;
		for(int i=0;i<a.size();i++)
		{
			sum+=a[i];
		}

		return sum;
	}
}
