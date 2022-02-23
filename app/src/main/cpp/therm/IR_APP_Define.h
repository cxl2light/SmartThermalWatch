
#ifndef IR_APP_DEFINE_H
#define IR_APP_DEFINE_H


#include"stdio.h"

#define DEFAULT_RET (0)
#define DEFAULT_TCP_PORT (4321)
#define MAX_CHANNEL_COUNT (16)
#define LOG_Prefix ("IR-APP: ")
#define  IR_Unknown_Error (0)
#define  IR_OK (1)  //命令发送成功


#define LOG(format, ...)
//     \
//	{char str[512]={0}; \
//	sprintf(str, format, __VA_ARGS__) ;\
//	printf(str);}

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){ delete(p); p = NULL; } }
#endif

/*
{
	"softVer":	"V1.0.1 Sep  6 2021 09:36:23",
	"hardware":	"ZS-T400",
	"devName":	"ZS-T400"
}
*/
typedef void (  *FunGetJsonCamera)(int channelNo,char* pJson);

/*
{
	"palette":	0,
	"zoom":	10,
	"sharpness":	5,
	"contrast":	5,
	"brightness":	5,
	"noiseReduction":	5,
	"reticle":	0,
	"x":	0,
	"y":	0,
	"trackEn":	0,
	"gpsEn":	0,
	"distanceEn":	0
}
*/
typedef void (  *FunGetJsonDevConfig)(int channelNo,char* pJson);


//static AI_OBJ_LIST g_stAIObjList[MAX_AI_LIST_NUM];
typedef void (  *FunGetJsonAnalyseResult)(int channelNo,char* pJson);

//nUpgrade=0 成功;  非0 失败
typedef void (  *FunGetUpgrade)(int channelNo,int nUpgrade);


//nProgress 传输进度0.0~1.0 ;  endFlag 1:传输完成, 0:传输未完成    --- 返回停止标记
typedef int (  *FunGetUpgradeProgress)(int channelNo,float nProgress, int endFlag);

#endif