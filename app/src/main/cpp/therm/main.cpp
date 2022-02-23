#include "stdio.h"
#include "IR_APP_SDK.h"
#include <unistd.h>
//
void CallbackGetJsonCamera(int channelNo, char *pJson)
{
    printf("GetJsonCamera:  %d-%s\n", channelNo, pJson);
}

int CallbackGetJsonDevConfig(int channelNo, char *pJson)
{
    printf("GetJsonDevConfig: %d-%s\n", channelNo, pJson);
}

int CallbackGetJsonAnalyseResult(int channelNo, char *pJson)
{
    printf("CallbackGetJsonAnalyseResult: %d-%s\n", channelNo, pJson);
}

//nUpgrade=0 成功;  非0 失败
void CallbackGetUpgrade(int channelNo, int nUpgrade)
{
    printf("GetUpgrade:  code: %d\n", nUpgrade);
}


int CallbackGetUpgradeProgress(int channelNo,float nProgress, int endFlag)
{
    printf("GetUpgradeProgress:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);
    
    int stopFlag = 1;     //停止传输
    stopFlag = 0;         //继续传输  

    return stopFlag;
}


/*
    printf("/n************************************\n");
    printf("innput 0 - IR_APP_SET_PALETTE \n");
    printf("innput 1 - IR_APP_SET_ZOOM \n");
    printf("innput 2 - IR_APP_GET_CAMERA \n");
    printf("innput 3 - IR_APP_GET_DEVCONFIG \n");
    printf("innput 4 - IR_APP_SET_SHARPNESS \n");
    printf("innput 5 - IR_APP_SET_BRIGHTNESS \n");
    printf("innput 6 - IR_APP_SET_CONTRAST \n");
    printf("innput 7 - IR_APP_SET_NOISEREDUCTION \n");
    printf("innput 8 - IR_APP_SET_RETICLE \n");
    printf("innput 9 - IR_APP_SET_XCORDINATE \n");
    printf("innput 10 - IR_APP_SET_YCORDINATE \n");
    printf("innput 11 - IR_APP_SET_TRACEENABLE \n");
    printf("innput 12 - IR_APP_SET_DISTANCEENABLE \n");
    printf("innput 13 - IR_APP_UPGRADE \n");
    printf("innput 14 - IR_APP_REBOOT \n");
    printf("/n************************************\n");
*/

int main()
{
    char ipv4[32] = "192.168.8.115";

    int nChannelNo = IR_APP_NewChannel(ipv4, CallbackGetJsonCamera, CallbackGetJsonDevConfig, CallbackGetUpgrade, CallbackGetJsonAnalyseResult);
    printf("nChannelNo IR_APP_NewChannel()= %d\n\n", nChannelNo);

    int retLink = IR_APP_SynLink(nChannelNo, 3000);
    printf("retLink = %d\n\n", retLink);

    while (retLink)
    {
        printf("input cmd\n");

        /* code */
        int cmd = 0;

        scanf("%d", &cmd);

        if (0 == cmd)
        {
            static int rulerIndex = 0;
            int ret = IR_APP_SET_PALETTE(nChannelNo, rulerIndex);
            ++rulerIndex;
            rulerIndex = rulerIndex % 6;

            printf("cmd ret= %d\n", ret);
        }
        else if (1 == cmd)
        {
            static int value = 10;
            int ret = IR_APP_SET_ZOOM(nChannelNo, value);
            value += 10;
            value = value > 60 ? 10 : value;

            printf("cmd ret= %d\n", ret);
        }
        else if (2 == cmd)
        {
            int ret = IR_APP_GET_CAMERA(nChannelNo);
            printf("cmd ret= %d\n", ret);
        }
        else if (3 == cmd)
        {
            int ret = IR_APP_GET_DEVCONFIG(nChannelNo);
            printf("cmd ret= %d\n", ret);
        }
        else if (4 == cmd)
        {
            static int value = 0;
            int ret = IR_APP_SET_SHARPNESS(nChannelNo, value);
            ++value;
            value = value % 11;

            printf("cmd ret= %d\n", ret);
        }
        else if (5 == cmd)
        {
            static int value = 1;
            int ret = IR_APP_SET_BRIGHTNESS(nChannelNo, value);
            ++value;
            value = value > 10 ? 1 : value;

            printf("cmd ret= %d\n", ret);
        }
        else if (6 == cmd)
        {
            static int value = 1;
            int ret = IR_APP_SET_CONTRAST(nChannelNo, value);
            ++value;
            value = value > 10 ? 1 : value;

            printf("cmd ret= %d\n", ret);
        }
        else if (7 == cmd)
        {
            static int value = 0;
            int ret = IR_APP_SET_NOISEREDUCTION(nChannelNo, value);
            ++value;
            value = value % 11;

            printf("cmd ret= %d\n", ret);
        }
        else if (8 == cmd)
        {
            static int value = 0;
            int ret = IR_APP_SET_RETICLE(nChannelNo, value);
            ++value;
            value = value % 8;

            printf("cmd ret= %d\n", ret);
        }
        else if (9 == cmd)
        {
            static int value = 0;
            int ret = IR_APP_SET_XCORDINATE(nChannelNo, value);

            printf("cmd ret= %d\n", ret);
        }
        else if (10 == cmd)
        {
            static int value = 0;
            int ret = IR_APP_SET_YCORDINATE(nChannelNo, value);

            printf("cmd ret= %d\n", ret);
        }
        else if (11 == cmd)
        {
            static int value = 0;
            int ret = IR_APP_SET_TRACEENABLE(nChannelNo, value);
            value = value == 0 ? 1 : 0;

            printf("cmd ret= %d\n", ret);
        }
        else if (12 == cmd)
        {
            static int value = 0;
            int ret = IR_APP_SET_DISTANCEENABLE(nChannelNo, value);
            value = value == 0 ? 1 : 0;

            printf("cmd ret= %d\n", ret);
        }
        else if (13 == cmd)
        {
            char* path = "/mnt/hgfs/AndroidSDK/T400_V1.3.2.pkg";
            int ret = IR_APP_UPGRADE(nChannelNo, path , CallbackGetUpgradeProgress);

            printf("cmd ret= %d\n", ret);
        }
        else if (14 == cmd)
        {
            int ret = IR_APP_REBOOT(nChannelNo);

            printf("cmd ret= %d\n", ret);

        }else if (15 == cmd)
        {
            int ret = IR_APP_GET_ANALYSERESULT(nChannelNo);
            printf("cmd ret= %d\n", ret);
        }
        else if (16 == cmd)
        {

        }
        else if (100 == cmd)
        {
            /* code */

            break;
        }

        sleep(1);
    }

    printf("dis link\n\n");

    IR_APP_SynDisLink(nChannelNo);

    IR_APP_DelChannel(nChannelNo);

    return 0;
}
