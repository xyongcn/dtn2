#include <pthread.h>
#include<math.h>
#include"DTN2interactorThread.h"

using namespace std;

NodeInfo *node;

int main(void)
{
	node=NodeInfo::GetInstance();

	if(node->Mode==NodeInfo::GPS_Mode)//已启动
	{
		cout<<"GPS MODE"<<endl;
	}

	else if(node->Mode==NodeInfo::Config_Mode)
	{
		//确定首个配置位置
		ConfigLocation *configLoc = ConfigLocation::GetInstance();
		ParseConfigFile *pcf = configLoc->getFirstLocation();
		printf("首个位置经纬度：%.8lf,%.8lf\n",pcf->longitude,pcf->latitude);
		cout<<"_______________________________________________"<<endl;
		}
		else
		{

			cout<<"Mode Error!"<<"Exception: 错误的启动模式"<<endl;
		}
	    InteractorThread *aodvqueryListener = InteractorThread::Getinstance();
	    DTN2interactorThread *dtn2queryListener = DTN2interactorThread::Getinstance();
	    dtn2queryListener->init();
	    aodvqueryListener->init();
	    pthread_join(aodvqueryListener->id,NULL);
	    pthread_join(dtn2queryListener->id,NULL);


		return 0;
}
