//#include "stdafx.h"
#include <malloc.h>
#include<string.h>
#include <stdio.h>




#define WINDOWS_SDK (0)
#if WINDOWS_SDK
#include "windows.h"
#else

#endif


#include "wifi_decode_packet.h"
//#include "..\MAG_SDK_CHANNEL\EasyLog.h"
//#include <WinBase.h>
//#include "crc16.h"
//#include <Strsafe.h>


//
#define RECV_PACKET_SIZE (1*1024*1024)			// 1*1024*1024      2*1024*1024
#define RECV_BUF_SIZE (2*1024*1024)					// 2*1024*1024     3*1024*1024



//#define PRINTF(fmt, args...)        printf(fmt, ##args)
#define PRINTF(fmt)

typedef unsigned short __u16;
typedef unsigned int __u32;
#if 0
#define net_to_host16(x) \
    ((__u16)( \
    (((__u16)(x) & (__u16)0x00ffU) << 8) | \
    (((__u16)(x) & (__u16)0xff00U) >> 8) ))
#define net_to_host32(x) \
    ((__u32)( \
    (((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
    (((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
    (((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
    (((__u32)(x) & (__u32)0xff000000UL) >> 24) ))
#else
#define net_to_host16(x) x
#define net_to_host32(x) x
#endif


static int process_packet(struct PACKET_INFO* pInfo, BYTE* pBuf, int nBufLen);

static int packet_init(struct PACKET_INFO* pInfo)
{
    int i;
    for (i = 0; i < RECV_PACKET_NUM; i ++)
    {
        pInfo->packet[i].nStatus = RECV_BUFFER_STATUS_IDLE;
        pInfo->packet[i].pData =  new BYTE[pInfo->recv_packet_size];		//RECV_PACKET_SIZE

        if (0 == pInfo->packet[i].pData )
        {
            //OutputDebugStringA("SCT-SDK: Fatal error New failure packet[i].pData == 0\n");
            //EasyLog::Write("Fatal error New failure packet[i].pData == 0");
        }


        if (NULL == pInfo->packet[i].pData)
        {
            PRINTF("malloc failed!\n");
            return RET_PKT_MALLOC_FAILED;
        }
    }

    pInfo->pRecv =  new BYTE [pInfo->recv_buf_size];

    if (0 == pInfo->pRecv )
    {
        //OutputDebugStringA("SCT-SDK: Fatal error New failure pRecv == 0\n");
        //EasyLog::Write("Fatal error New failure pRecv == 0");
    }


    if (NULL == pInfo->pRecv)
    {
        PRINTF("malloc failed!\n");
        return RET_PKT_MALLOC_FAILED;
    }

    pInfo->pTemp =  new BYTE[pInfo->recv_buf_size];

    if (0 == pInfo->pTemp )
    {
        //OutputDebugStringA("SCT-SDK: Fatal error New failure pTemp == 0\n");
        //EasyLog::Write("Fatal error New failure pTemp == 0");
    }


    if (NULL == pInfo->pTemp)
    {
        PRINTF("malloc failed!\n");
        return RET_PKT_MALLOC_FAILED;
    }

    pInfo->nReadPos = 0;
    pInfo->nWritePos = 0;
    return RET_PKT_SUCCESS;
}

static int packet_uninit(struct PACKET_INFO* pInfo)
{
    int i;
    for (i = 0; i < RECV_PACKET_NUM; i ++)
    {
        pInfo->packet[i].nStatus = RECV_BUFFER_STATUS_IDLE;
        pInfo->packet[i].nLen = 0;

        if (pInfo->packet[i].pData)
        {
            delete (pInfo->packet[i].pData);
            pInfo->packet[i].pData = 0;
        }

    }

    if (pInfo->pRecv)
    {
        delete (pInfo->pRecv);
        pInfo->pRecv = 0;
    }

    if (pInfo->pTemp)
    {
        delete (pInfo->pTemp);
        pInfo->pTemp= 0;
    }


    return RET_PKT_SUCCESS;
}

static int put_data_to_buf(struct PACKET_INFO* pInfo, BYTE* pBuf, int nLen)
{
    if (NULL == pBuf || nLen < 0)
        return RET_PKT_NULL_POINTER;

    int nRemainLen = (pInfo->nReadPos + pInfo->recv_buf_size - pInfo->nWritePos) % (pInfo->recv_buf_size);

    if(pInfo->nReadPos == pInfo->nWritePos)
        nRemainLen = pInfo->recv_buf_size;

    if (nRemainLen < nLen)
    {
        /*static int i = 0;
        TRACE(_T("nRemainLen < nLen  %d \n"),i++);*/
        pInfo->nReadPos  = 0;
        pInfo->nWritePos = 0;
        pInfo->nCurrPkt = 0;
        //pInfo-> = 0;
        //pInfo->nRdIndex = 0;

        return RET_RECV_BUFFER_NOT_ENOUGH;
    }

    if (pInfo->nWritePos + nLen <= pInfo->recv_buf_size)
    {
        memcpy(pInfo->pRecv + pInfo->nWritePos, pBuf, nLen);
    }
    else
    {
        int nRemain = pInfo->recv_buf_size - pInfo->nWritePos;
        memcpy(pInfo->pRecv + pInfo->nWritePos, pBuf, nRemain);
        memcpy(pInfo->pRecv, pBuf + nRemain, nLen - nRemain);
    }

    pInfo->nWritePos = (pInfo->nWritePos + nLen ) % (pInfo->recv_buf_size);
    return RET_PKT_SUCCESS;
}

static int find_idle_packet(struct PACKET_INFO* pInfo)
{
    int i;
    for(i = 0; i < RECV_PACKET_NUM; i ++)
    {
        if (RECV_BUFFER_STATUS_IDLE == pInfo->packet[i].nStatus)
        {
            pInfo->packet[i].nLen = 0;
            pInfo->packet[i].nFrameStat = 0;
            //memset(pInfo->packet[i].pData, 0x55, RECV_PACKET_SIZE); //20181010 修改,解决丢包问题(注释掉)
            pInfo->nCurrPkt = i;
            return pInfo->nCurrPkt;
        }
    }
    return -1;
}

static int is_have_packet(struct PACKET_INFO* pInfo)
{
    int i;
    for(i = 0; i < RECV_PACKET_NUM; i ++)
    {
        if (RECV_BUFFER_STATUS_FINISH == pInfo->packet[i].nStatus)
            return 1;
    }

    return 0;
}

static int get_a_packet(struct PACKET_INFO* pInfo, unsigned char* pBuf, int* pLen, unsigned int* pu32DataType)
{
    int i;
    for(i = 0; i < RECV_PACKET_NUM; i++)
    {
        if (RECV_BUFFER_STATUS_FINISH == pInfo->packet[i].nStatus)
        {
            /*if (pInfo->packet[i].u32DataType == 1)
            {
            OutputDebugStringA("RECV_BUFFER_STATUS_FINISH 1\n");
            }else if (pInfo->packet[i].u32DataType == 2)
            {
            OutputDebugStringA("RECV_BUFFER_STATUS_FINISH 2-------------------------------------\n");
            }*/

            if (NULL == pBuf)
            {
                *pLen = pInfo->packet[i].nLen;
            }
            else
            {
                *pu32DataType = pInfo->packet[i].u32DataType;
                *pLen = pInfo->packet[i].nLen;
                memcpy(pBuf, pInfo->packet[i].pData, *pLen);
                pInfo->packet[i].nStatus = RECV_BUFFER_STATUS_IDLE;
                pInfo->packet[i].nLen = 0;
                pInfo->packet[i].u32DataType = 0;
            }
            return i;
        }
    }
    return -1;
}

static int net_to_host(WIFI_FRAME_HEADER* pWifiFrameHeader)
{
    return RET_PKT_SUCCESS;
    pWifiFrameHeader->uTransFrameHdrSize = net_to_host32(pWifiFrameHeader->uTransFrameHdrSize);
    pWifiFrameHeader->uTransFrameSize = net_to_host32(pWifiFrameHeader->uTransFrameSize);
    pWifiFrameHeader->uTransFrameNo = net_to_host32(pWifiFrameHeader->uTransFrameNo);
    pWifiFrameHeader->uDataFrameSize = net_to_host32(pWifiFrameHeader->uDataFrameSize);
    pWifiFrameHeader->uDataFrameTotal = net_to_host32(pWifiFrameHeader->uDataFrameTotal);
    pWifiFrameHeader->uDataFrameCurr = net_to_host32(pWifiFrameHeader->uDataFrameCurr);
    pWifiFrameHeader->uDataInFrameOffset = net_to_host32(pWifiFrameHeader->uDataInFrameOffset);

    return RET_PKT_SUCCESS;
}

static int process_packet(struct PACKET_INFO* pInfo, BYTE* pBuf, int nBufLen)
{
#if 0
    char buff1[256] ={0};
    sprintf(buff1, "recv %4d pos:%d\n",nBufLen, pInfo->nReadPos);
    OutputDebugStringA(buff1);
#endif


    int ret;
    ret = put_data_to_buf(pInfo, pBuf, nBufLen);
    if (RET_PKT_SUCCESS != ret)
    {
        return RET_RECV_BUFFER_NOT_ENOUGH;
    }

    int nReadPos = pInfo->nReadPos;
    int nWritePos = pInfo->nWritePos;
    BYTE* pRecv = pInfo->pRecv;
    BYTE* pTemp = pInfo->pTemp;
    BYTE* pSrc = pRecv + nReadPos;
    int i;
    int nIndex;
    int nLen = (nWritePos + pInfo->recv_buf_size - nReadPos) % (pInfo->recv_buf_size);
    WIFI_FRAME_HEADER* pFrameHdr;
    BYTE flag[5] = {"ZSDF"};
    int bFinded = 0;

    //为了方便检索，将接收的数据放于另一个缓冲
    if (nReadPos + nLen <= pInfo->recv_buf_size)
    {
        memcpy(pTemp, pSrc, nLen);
    }
    else
    {
        int nRemain = pInfo->recv_buf_size - nReadPos;
        memcpy(pTemp, pSrc, nRemain);
        memcpy(pTemp + nRemain, pRecv, nLen - nRemain);
    }

    for(i = 0; i < nLen; )
    {
        // find packet header
        while (i + 4 < nLen)
        {
            if(0 == memcmp(pTemp + i, flag, 4))
            {
                bFinded = 1;
                break;
            }
            i ++;
        }

        pSrc = pTemp + i;

        if (!bFinded)
        {
            pInfo->nReadPos = (nReadPos + i )% (pInfo->recv_buf_size);
            return RET_PKT_NOT_FIND_HEADER;
        }

        if (nLen - i < sizeof(WIFI_FRAME_HEADER))
        {
            return RET_PKT_WAITING;
        }

        pFrameHdr = (WIFI_FRAME_HEADER*)pSrc;

/*
        printf("uDataFrameTotal %d,uTransFrameSize %d,uDataFrameSize %d,uDataFrameTotal %d,uDataFrameCurr %d,uDataInFrameOffset %d\n",
            pFrameHdr->uDataFrameTotal,
            pFrameHdr->uTransFrameSize,
            pFrameHdr->uDataFrameSize,
            pFrameHdr->uDataFrameTotal,
            pFrameHdr->uDataFrameCurr,
            pFrameHdr->uDataInFrameOffset);	*/


        pFrameHdr->uHdrCRC = net_to_host16(pFrameHdr->uHdrCRC);
        pFrameHdr->uDataCRC = net_to_host16(pFrameHdr->uDataCRC);
        if (0 &&
            pFrameHdr->uHdrCRC != cyg_crc16(pSrc + 8, sizeof(WIFI_FRAME_HEADER) - 8))
        {
            i ++;
            continue;
        }

        //net_to_host(pFrameHdr); //xiugai

        if ((unsigned int)(nLen - i)  < pFrameHdr->uTransFrameSize)
        {
            return RET_PKT_WAITING;
        }

        if (0 &&
            pFrameHdr->uDataCRC != cyg_crc16(pFrameHdr->Data, pFrameHdr->uTransFrameSize - pFrameHdr->uTransFrameHdrSize))
        {
#if 0
            char buff[256] ={0};
            sprintf(buff,"Error:%d Frame:%d uDataCRC:%X, LocalCRC:%X\n",
                pFrameHdr->uTransFrameNo,
                pFrameHdr->uDataFrameCurr,
                pFrameHdr->uDataCRC);
            printA(buff);
#endif

            i += pFrameHdr->uTransFrameSize;
            continue;
        }

#if 0
        if (pFrameHdr->uDataType == 0x100 + 1)
        {
            char buff[256] ={0};
            sprintf(buff, "SCT-SDK-D-Compress: %X, 0x%X %4d %2d - %d\n",pInfo,
                pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                                            pFrameHdr->uDataFrameCurr);
            OutputDebugStringA(buff);
        }else if (pFrameHdr->uDataType == 1)
        {
            char buff[256] ={0};
            sprintf(buff, "SCT-SDK-D-Raw:%X,  0x%X %4d %2d - %d\n",pInfo,
                pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                pFrameHdr->uDataFrameCurr);
            OutputDebugStringA(buff);
        }else if (pFrameHdr->uDataType == 2)
        {
            char buff[256] ={0};
            sprintf(buff, "SCT-SDK-D-MtParas:%X,  0x%X %4d %2d - %d\n",pInfo,
                pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                pFrameHdr->uDataFrameCurr);
            OutputDebugStringA(buff);

        }else if (pFrameHdr->uDataType == 0x1000 + 0x100 + 31)
        {
            char buff[256] ={0};
            sprintf(buff, "SCT-SDK-D-RS485: %X, 0x%X %4d %2d - %d\n",pInfo,
                pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                pFrameHdr->uDataFrameCurr);
            OutputDebugStringA(buff);
        }
#endif

        if (pFrameHdr->uDataFrameTotal == pFrameHdr->uDataFrameCurr)
        {
            /*TRACE("-----------------Size:%4d No:%5d Fr:%d - %2d \n", g_totalBytes,pFrameHdr->uTransFrameNo,
                pFrameHdr->uDataFrameTotal, pFrameHdr->uDataFrameCurr );*/
        }
        else
        {
            /*TRACE("Size:%4d No:%5d Fr:%d - %2d \n", g_totalBytes,pFrameHdr->uTransFrameNo,
                pFrameHdr->uDataFrameTotal, pFrameHdr->uDataFrameCurr );*/
        }


        if (1)  //新逻辑
        {
            //如果当前正在接收中....
            if (pInfo->packet[pInfo->nCurrPkt].nStatus == RECV_BUFFER_STATUS_RECEIVING)
            {
                //传来的类型和当前的类型不一致
                if (pInfo->packet[pInfo->nCurrPkt].u32DataType != pFrameHdr->uDataType)
                {
                    double sec = (double)( clock() - pInfo->recvTimeOut ) / (double)CLOCKS_PER_SEC;

                    if (sec > 3 &&
                        (pFrameHdr->uDataFrameCurr == 1)) //超过了3秒钟,且来了新的一包
                    {
                        pInfo->recvTimeOut = clock();

                        pInfo->packet[pInfo->nCurrPkt].nStatus = RECV_BUFFER_STATUS_IDLE;

#if  WINDOWS_SDK
                        OutputDebugStringA("SCT-SDK:  Reset STATUS_IDLE\n");

                        //EasyLog::Write("Reset STATUS_IDLE");
#endif

                    }else
                    {
                        i += pFrameHdr->uTransFrameSize;

#if  WINDOWS_SDK
                        OutputDebugStringA("SCT-SDK:  discard noise packet\n");

                        //EasyLog::Write("discard noise packet");
#endif

                        continue;
                    }

                }else
                {
                    pInfo->recvTimeOut = clock();
                }
            }
        }


        /////////////////////
        if (pFrameHdr->uTransFrameHdrSize >  pInfo->recv_packet_size ||
            pFrameHdr->uTransFrameSize >  pInfo->recv_packet_size  ||
            pFrameHdr->uDataFrameSize >  pInfo->recv_packet_size )
        {
#if  WINDOWS_SDK
            OutputDebugStringA("SCT-SDK-Error  len too big-0\n");

            char buff[256] ={0};
            sprintf(buff, "SCT-SDK-Error: %X, 0x%X %4d %2d - %d\n",pInfo,
                pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                pFrameHdr->uDataFrameCurr);
            OutputDebugStringA(buff);

            //EasyLog::Write("SCT-SDK-Error  len too big-%d %d %d\n",pFrameHdr->uTransFrameHdrSize , pFrameHdr->uTransFrameSize, pFrameHdr->uDataFrameSize);
#endif
            continue;
        }



        //长度检查
        if (  (pFrameHdr->uTransFrameSize - pFrameHdr->uTransFrameHdrSize) >
            pInfo->recv_packet_size)
        {
#if  WINDOWS_SDK
            OutputDebugStringA("SCT-SDK-Error  len too big\n");

            char buff[256] ={0};
            sprintf(buff, "SCT-SDK-Error: %X, 0x%X %4d %2d - %d\n",pInfo,
                pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                pFrameHdr->uDataFrameCurr);
            OutputDebugStringA(buff);

            //EasyLog::Write("SCT-SDK-Error  len too big");
#endif
            continue;
        }

        //长度检查2
        if (  (pFrameHdr->uTransFrameSize - pFrameHdr->uTransFrameHdrSize + pFrameHdr->uDataInFrameOffset) >
            pInfo->recv_packet_size)
        {
#if  WINDOWS_SDK
            OutputDebugStringA("SCT-SDK-Error  len too big - Offset\n");

            char buff[256] ={0};
            sprintf(buff, "SCT-SDK-Error: %X, 0x%X %4d %2d - %d\n",pInfo,
                pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                pFrameHdr->uDataFrameCurr);
            OutputDebugStringA(buff);

            //EasyLog::Write("SCT-SDK-Error  len too big - Offset\n");
#endif
            continue;
        }


        //新逻辑
        if (1 != pFrameHdr->uDataFrameCurr)
        {
            if (pInfo->packet[pInfo->nCurrPkt].nStatus   ==  RECV_BUFFER_STATUS_IDLE)
            {
                //OutputDebugStringA("SCT-SDK-Error  lost first packet\n");
                continue;
            }
        }



        //第一包到达
        if (1 == pFrameHdr->uDataFrameCurr)
        {
            nIndex = find_idle_packet(pInfo);
            if (-1 == nIndex)
            {
                pInfo->nReadPos = (nReadPos +  i)% (pInfo->recv_buf_size);
                return RET_PKT_BUFFER_FULL;
                //trans packet
            }
        }


#if 0
        if (pFrameHdr->uTransFrameSize > 1024*1024)
        {
            EasyLog::Write("error  raw uTransFrameSize= %d", pFrameHdr->uTransFrameSize);

        }else if (pFrameHdr->uTransFrameHdrSize> 1024*1024)
        {
                EasyLog::Write("error  raw uTransFrameHdrSize= %d", pFrameHdr->uTransFrameHdrSize);
        }
#endif



        //////////////////////////////
        if (1)  //新逻辑
        {
            //赋值类型，和接收状态
            pInfo->packet[pInfo->nCurrPkt].u32DataType = pFrameHdr->uDataType;
            pInfo->packet[pInfo->nCurrPkt].nStatus    = RECV_BUFFER_STATUS_RECEIVING;
        }

        //////////////////////////////
        memcpy(pInfo->packet[pInfo->nCurrPkt].pData + pFrameHdr->uDataInFrameOffset,
            pFrameHdr->Data,
            pFrameHdr->uTransFrameSize - pFrameHdr->uTransFrameHdrSize);



        pInfo->packet[pInfo->nCurrPkt].nFrameStat ++;

        i += pFrameHdr->uTransFrameSize;
        pInfo->packet[pInfo->nCurrPkt].nLen += pFrameHdr->uTransFrameSize - pFrameHdr->uTransFrameHdrSize;
        pInfo->nReadPos = (nReadPos +  i)% (pInfo->recv_buf_size);

        if (pFrameHdr->uDataFrameTotal == pFrameHdr->uDataFrameCurr)
        {

#if 0
            if (pFrameHdr->uDataType == 1)
            {
                /*char buff[256] ={0};
                sprintf(buff, "SCT-SDK-Debug:  0x%X %4d %2d - %d\n",pFrameHdr->uDataType, pFrameHdr->uTransFrameNo,pFrameHdr->uDataFrameTotal,
                pFrameHdr->uDataFrameCurr);*/
                char buff[256] ={0};
                sprintf(buff, "SCT-SDK-Debug:  One Frame arrive uDataType %d,  uDataFrameSize %d \n", pFrameHdr->uDataType,pFrameHdr->uDataFrameSize);
                OutputDebugStringA(buff);
            }
#endif




            pInfo->packet[pInfo->nCurrPkt].u32DataType = pFrameHdr->uDataType;
            pInfo->packet[pInfo->nCurrPkt].nStatus = RECV_BUFFER_STATUS_FINISH;

            pInfo->packet[pInfo->nCurrPkt].nLen = pFrameHdr->uDataFrameSize;//20181010 修改,解决丢包问题(新增行)


            if(pInfo->packet[pInfo->nCurrPkt].nFrameStat != pFrameHdr->uDataFrameTotal) //20181010 修改,解决丢包问题(注释掉)
            {


                //pInfo->packet[pInfo->nCurrPkt].nStatus = RECV_BUFFER_STATUS_IDLE; //20181010 修改,解决丢包问题(注释掉)
#if 0 && WINDOWS_SDK
                //save file
                //TRACE("Error Frame Toal:%d Real:%d \n", pFrameHdr->uDataFrameTotal, pInfo->packet[pInfo->nCurrPkt].nFrameStat );
                char buff[256] ={0};
                sprintf(buff,"SCT-SDK: Error Frame Toal:%d Real:%d  DataType:%d\n",
                                        pFrameHdr->uDataFrameTotal,
                                        pInfo->packet[pInfo->nCurrPkt].nFrameStat,
                                        pInfo->packet[pInfo->nCurrPkt].u32DataType);
                OutputDebugStringA(buff);
#endif
            }
        }

        //TRACE("Frame:%d len:%d \n",pFrameHdr->uDataFrameCurr, pInfo->packet[pInfo->nCurrPkt].nLen);
    }



    return RET_PKT_SUCCESS;;
}

struct PACKET_INFO packet_info =
{
    NULL,	//pRecv
    0,			//nReadPos
    0,			//nWritePos
    NULL,	//pTemp
    0,			//nCurrPkt
    {
        {0,0,NULL},
    },
    RECV_PACKET_SIZE,
    RECV_BUF_SIZE,
    packet_init,
    packet_uninit,
    process_packet,
    is_have_packet,
    get_a_packet,
    0,
};


struct PACKET_INFO packet_info_big =
{
    NULL,	//pRecv
    0,			//nReadPos
    0,			//nWritePos
    NULL,	//pTemp
    0,			//nCurrPkt
    {
        {0,0,NULL},
    },
    640*480*2+ 1024,			//614400
    RECV_BUF_SIZE,					//1024*1024
    packet_init,
    packet_uninit,
    process_packet,
    is_have_packet,
    get_a_packet,
    0,
};


struct PACKET_INFO packet_info_small =
{
    NULL,	//pRecv
    0,			//nReadPos
    0,			//nWritePos
    NULL,	//pTemp
    0,			//nCurrPkt
    {
        {0,0,NULL},
    },

    64*1024,			//最多64K
    128*1024,		//最多128K

    packet_init,
    packet_uninit,
    process_packet,
    is_have_packet,
    get_a_packet,
    0,
};
