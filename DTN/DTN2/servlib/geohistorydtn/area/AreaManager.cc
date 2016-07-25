#include "AreaManager.h"

namespace dtn
{

	const string AreaManager::tag="AreaManager";
	const string AreaManager::historyAreaFilePath="/home/gaorui/workspace/DTN/historyarea.obj";
	const string AreaManager::historyAreaMovingFilePath="/home/gaorui/workspace/DTN/areamoving.txt";

	AreaManager * AreaManager::instance=NULL;
}
