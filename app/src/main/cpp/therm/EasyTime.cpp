#include "EasyTime.h"



EasyTime::EasyTime()
{
	Start();
}


EasyTime::~EasyTime()
{
}

void EasyTime::Start()
{
	start = clock();
}

void EasyTime::Restart()
{
	start = clock();
}

double EasyTime::Elapsed()
{
	clock_t stop = clock();

	return (double)(stop - start) / (double)CLOCKS_PER_SEC;
}
