#include<vector>
#include<string>
#include<stdio.h>


using namespace std;

namespace dtn
{
 class ValidVector {
 public :
	vector<double> minuteVector;
	vector<double> hourVector;
	vector<double> monafteveVector;
	vector<double> weekVector;
	vector<double> monthVector;

	ValidVector(
			vector<double> minuteVector,vector<double> hourVector,
			vector<double> monafteveVector,vector<double> weekVector,
			vector<double> monthVector)
	{
		this->minuteVector=minuteVector;
		this->hourVector=hourVector;
		this->monafteveVector=monafteveVector;
		this->weekVector=weekVector;
		this->monthVector=monthVector;
	}

	vector<double> getMinuteVector()
	{
		return minuteVector;
	}

	vector<double> getHourVector()
	{
		return hourVector;
	}

	vector<double> getMonAftEveVector()
	{
		return monafteveVector;
	}

	vector<double> getWeekVector()
	{
		return weekVector;
	}

	vector<double> getMonthVector()
	{
		return monthVector;
	}


	string toString()
	{
		string s;
		s.append("valid vector:\n");
		if(!minuteVector.empty())
		{
			string minutestr;
			minutestr.append("minute vector:");
			char c[10];
			for(int i=0;i<minuteVector.size();++i)
			{
				sprintf(c,"%.6f ",minuteVector[i]);
				minutestr.append(c);
			}
			minutestr.append(";\n");
			s.append(minutestr);
		}
		if(!hourVector.empty())
		{
			string hourstr;
			hourstr.append("hour vector:");
			char c[10];
			for(int i=0;i<hourVector.size();++i)
			{
				sprintf(c,"%.6f ",hourVector[i]);
				hourstr.append(c);
			}
			hourstr.append(";\n");
			s.append(hourstr);
		}
		if(!monafteveVector.empty())
		{
			string monaftevestr;
			monaftevestr.append("monafteve vector:");
			char c[10];
			for(int i=0;i<monafteveVector.size();++i)
			{
				sprintf(c,"%.6f ",monafteveVector[i]);
				monaftevestr.append(c);
			}
			monaftevestr.append(";\n");
			s.append(monaftevestr);
		}
		if(!weekVector.empty())
		{
			string weekstr;
			weekstr.append("week vector:");
			char c[10];
			for(int i=0;i<weekVector.size();++i)
			{
				sprintf(c,"%.6f ",weekVector[i]);
				weekstr.append(c);
			}
			weekstr.append(";\n");
			s.append(weekstr);
		}
		if(!monthVector.empty())
		{
			string monthstr;
			monthstr.append("month vector:");
			char c[10];
			for(int i=0;i<monthVector.size();++i)
			{
				sprintf(c,"%.6f ",monthVector[i]);
				monthstr.append(c);
			}
			monthstr.append(";\n");
			s.append(monthstr);
		}

		return s;
	}
};
}
