#include"GetAdminAreaService.h"

int main()
{
	cout<<"启动查询节点所处区域程序："<<endl;
	GetAdminAreaService *service=GetAdminAreaService::Getinstance();
	service->init();
	return 0;
}
