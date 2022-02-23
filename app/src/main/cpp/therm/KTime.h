#ifndef KTIME_H
#define KTIME_H

#include <sys/time.h>  
#include <sys/sysinfo.h>

//计算延迟，跨平台
class KTime{
public:
	KTime();

	//struct timeval tpstart;
	//struct timeval tpend;  
	long _uptimeStart; 
	long _uptimeEnd; 
	
public:
	void start(); 

	float elapsed(); //返回秒

	static char* getDateTime();
};

#endif

