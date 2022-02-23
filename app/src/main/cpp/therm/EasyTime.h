#ifndef EASY_TIME_H
#define EASY_TIME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class EasyTime
{
public:
	EasyTime();
	~EasyTime();

protected:
	clock_t start;

public:
	void Start();
	
	void Restart();

	double Elapsed();	//秒
};

#endif
