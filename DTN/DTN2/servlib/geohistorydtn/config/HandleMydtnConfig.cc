#include"HandleMydtnConfig.h"
#ifndef FVLEVEL_H_
#define FVLEVEL_H_
#include"servlib/geohistorydtn/frequencyVector/FrequencyVectorLevel.h"
#endif

#ifndef FCONFIG_H_
#define FCONFIG_H_
#include "servlib/geohistorydtn/config/FrequencyConfig.h"
#endif

namespace dtn
{
HandleMydtnConfig * HandleMydtnConfig::instance=NULL;
HandleMydtnConfig::HandleMydtnConfig()
{
	vector<int> frequencysort;
	fstream fr;
	int minute=FrequencyVectorLevel::minuteVector;
	int hour=FrequencyVectorLevel::hourVector;
	int monafteve=FrequencyVectorLevel::monAftEveVector;
	int week=FrequencyVectorLevel::weekVector;
	int month=FrequencyVectorLevel::monthVector;
	fr.open("./mydtn.conf",ios::in);
	if(!fr.is_open())
	{
		cout<<"error"<<endl;
	}
	else
	{
		char buffer[200];
		const char *d=" \t";
		while(!fr.eof())
		{
			fr.getline (buffer,200);
			char *part=strtok(buffer,d);
			if(part!=NULL)
			{
				if(!strcmp(part,"frequency"))
				{
					part=strtok(NULL,d);
					while(part!=NULL)
					{
						if(!strcmp(part,"minute"))
							frequencysort.push_back(minute);
						else if(!strcmp(part,"hour"))
							frequencysort.push_back(hour);
						else if(!strcmp(part,"monafteve"))
							frequencysort.push_back(monafteve);
						else if(!strcmp(part,"week"))
							frequencysort.push_back(week);
						else if(!strcmp(part,"month"))
							frequencysort.push_back(month);
						part=strtok(NULL,d);
					}
				}
				else
					continue;
			}

		}
	}
	fr.close();
	if(!frequencysort.empty())
	{
		for(vector<int>::iterator it=frequencysort.begin();
			it!=frequencysort.end();++it)
			FrequencyConfig::frequcyType.push_back(*it);
	}
	else
	{
		FrequencyConfig::frequcyType.push_back(minute);
		FrequencyConfig::frequcyType.push_back(hour);
	}
}
}
