#include<fstream>
#include<string.h>
#include<iostream>

using namespace std;
namespace dtn
{
class HandleMydtnConfig
{
private:
	static HandleMydtnConfig *instance;
	HandleMydtnConfig();
public:
	static HandleMydtnConfig *Getinstance()
	{
		if(instance==NULL)
			instance = new HandleMydtnConfig();
		return instance;
	}
};
}
