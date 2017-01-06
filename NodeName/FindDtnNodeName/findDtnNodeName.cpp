#include<fstream>
#include<string.h>

using namespace std;

#define MAXIMUM 256

int main()
{
	char *buffer=new char[MAXIMUM];
	fstream fr;
	fr.open("/etc/dtn.conf",ios::in);
	if(!fr)
	{
		printf("/etc/dtn.config is not exist,error\n");
	    delete [] buffer;
		return 0;
	}
	const char *d = " \t";
	char *strArray;
	char temp[50];
    while(!fr.eof() )
    {
    	fr.getline (buffer,MAXIMUM);
    	strArray=strtok(buffer,d);

    	if(strArray!=NULL && !strcmp(strArray,"route"))
		{
		    strArray=strtok(NULL,d);
			if(!strcmp(strArray,"local_eid"))
			{
			    strArray=strtok(NULL,d);
				int i,j;
			    for(i=0;strArray[i]!='\0';++i)
			    	;
			    for(j=1;j<i-1;++j)
			    	strArray[j-1]=strArray[j];
			    strArray[j-1]='\0';
			    printf("%s",strArray);

			    delete [] buffer;
			    return 0;
			}
		}
    }
    delete [] buffer;
    return 0;
}
