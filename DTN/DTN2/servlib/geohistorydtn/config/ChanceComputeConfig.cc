#include"ChanceComputeConfig.h"

namespace dtn
{
	//分钟级别的阈值
	const double ChanceComputeConfig::MINUTE_LEVEL_AREA_THRESHOLD=1;
	//小时级别的阈值
	const double ChanceComputeConfig::HOUR_LEVEL_AREA_THRESHOLD=1;
	//上午中午下午级别的阈值
	const double ChanceComputeConfig::MONAFTEVE_LEVEL_AREA_THRESHOLD=1;
	//星期级别的阈值
	const double ChanceComputeConfig::WEEK_LEVEL_AREA_THREASHOLD=1;
	//月级别的阈值
	const double ChanceComputeConfig::MONTH_LEVEL_AREA_THRESHOLD=1;



	//将邻居相遇的机会值化为小于1的值，需要除以分母
	//分钟级别
	const double ChanceComputeConfig::MINUTE_LEVEL_NEIGHBOUR_DENOMINATOR=60;
	//小时级别
	const double ChanceComputeConfig::HOUR_LEVEL_NEIGHBOUR_DENOMINATOR=24;
	//上午中午下午级别
	const double ChanceComputeConfig::ChanceComputeConfig::MONAFTEVE_LEVEL_NEIGHBOUR_DENOMINATOR=3;
	//星期级别
	const double ChanceComputeConfig::WEEK_LEVEL_NEIGHBOUR_DENOMINATOR=7;
	//月级别
	const double ChanceComputeConfig::MONTH_LEVEL_NEIGHBOUR_DENOMINATOR=12;
}
