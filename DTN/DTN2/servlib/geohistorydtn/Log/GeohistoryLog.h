
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <pthread.h>

using namespace std;
#define MAX_CHAR_ONCE 511

namespace dtn{

class GeohistoryLog
{
private:
	static GeohistoryLog *instance;
	GeohistoryLog()
		{
			name="./logDocuments/GeohistoryLog.txt";

			errno = 0;
			g_pWTLog_File_descriptor = fopen(name.c_str(),"a");
			if (NULL == g_pWTLog_File_descriptor)
			{
				if (EINVAL == errno)
				{
					printf("err:fopen log file %s failed\n",name.c_str());
				}
				else
				{
					printf("err:unknow\n");
				}
			}

			   pthread_mutex_init(&g_Mutex,NULL);

		}
public:
    static GeohistoryLog* GetInstance()
    {
        if(instance == NULL)  //判断是否第一次调用
        	instance = new GeohistoryLog();

        return instance ;
    }
	 enum LOG_LEVEL
	{
	        INFO_LEVEL = 0,
	        ERROR_LEVEL ,
	        WARNING_LEVEL

	};

	pthread_mutex_t g_Mutex;

	FILE * g_pWTLog_File_descriptor;

	string name;


	 int  LogAppend(LOG_LEVEL level,char * fmt,...)
	{
		short nHeadLen(0);
		int n, size = MAX_CHAR_ONCE;
		char *p, *np;
		va_list ap;
		if ((p = (char*)malloc(size)) == NULL)
			return NULL;

		while (1)
		{
			switch(level)
			{
				case INFO_LEVEL:sprintf(p,"INFO:"); nHeadLen = 5; break;
				case WARNING_LEVEL:sprintf(p,"WARNING:"); nHeadLen = 8; break;
				case ERROR_LEVEL:
				default:sprintf(p,"ERR:"); nHeadLen = 4;break;
			}

			va_start(ap, fmt);
			n = vsnprintf(p+nHeadLen, size, fmt, ap);
			va_end(ap);

			strcat(p,"\n");

			if (n > -1 && n < size)
			{

	               pthread_mutex_lock(&g_Mutex );
	               fseek(g_pWTLog_File_descriptor, 0, SEEK_END);
					int h=fwrite(p,sizeof(char),strlen(p),g_pWTLog_File_descriptor);
					fflush(g_pWTLog_File_descriptor);
	               pthread_mutex_unlock(&g_Mutex );

					break;
			}
			if (n > -1)
				size = n+8;
			else
				size *= 2;
			if ((np = (char*)realloc (p, size)) == NULL)
			{
				free(p);
				return NULL;
			}
			else
			{
				p = np;
			}
		}



		return 0;
	}

	int LogClose()
	{

	    pthread_mutex_destroy(&g_Mutex );

		if(EOF == fclose(g_pWTLog_File_descriptor))
		{
			printf("err:fclose failed\n");
			return -1;
		}
		return 0;
	}

};
//GeohistoryLog * GeohistoryLog::instance=NULL;
}



//    geohistoryLog->LogClose();
//	CurrentLocationFromSimulator *location=CurrentLocationFromSimulator::Getinstance();
//location->start();
//GeohistoryLog *geohistoryLog=GeohistoryLog::GetInstance();
//#include"geohistorydtn/areaConnectiveSimulation/CurrentLocationFromSimulator.h"
