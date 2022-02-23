#include "KTime.h"

#include <sys/time.h>

#include <time.h>
#include"stdio.h"

KTime::KTime()
{

	//gettimeofday(&tpstart,NULL); 
	//gettimeofday(&tpend,NULL); 
	//_clockStart = clock();
	//_clockEnd = clock();
	struct sysinfo infoStart;
	sysinfo(&infoStart);
	_uptimeStart = infoStart.uptime;

	struct sysinfo infoEnd;
	sysinfo(&infoEnd);
	_uptimeEnd = infoEnd.uptime;

}

void KTime::start()
{

	//gettimeofday(&tpstart,NULL); 
	struct sysinfo infoStart;
	sysinfo(&infoStart);
	_uptimeStart = infoStart.uptime;

}

float KTime::elapsed()
{

	struct sysinfo infoEnd;
	sysinfo(&infoEnd);
	_uptimeEnd = infoEnd.uptime;

	return (_uptimeEnd - _uptimeStart);

}

char* KTime::getDateTime()
{
	static char dataTime[256] = {0};

	time_t now ;

	struct tm *tm_now ;

	time(&now) ;

	tm_now = localtime(&now) ;

	sprintf(dataTime,"now datetime: %d-%d-%d %d:%d:%d\n",
		tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, 
		tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec) ;

	return dataTime;

}
