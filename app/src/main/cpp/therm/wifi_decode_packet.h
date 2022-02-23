#ifndef __WIFI_DECODE_PACKET_H__
#define __WIFI_DECODE_PACKET_H__

//#include "stdafx.h"
#include "wifi_define.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RECV_PACKET_NUM 12

enum
{
    RECV_BUFFER_STATUS_IDLE = 0,			//空闲
    RECV_BUFFER_STATUS_RECEIVING,		//正在接收中
    RECV_BUFFER_STATUS_FINISH,			//完成
    RECV_BUFFER_STATUS_HANDLED,	//收到的包已经被处理
};

enum
{
    RET_PKT_SUCCESS,
    RET_PKT_NULL_POINTER,	//null pointer
    RET_PKT_MALLOC_FAILED,	//malloc failed
    RET_PKT_NOT_FIND_HEADER,	//
    RET_PKT_WAITING,
    RET_PKT_BROKEN,
    RET_PKT_BUFFER_FULL,	//receive buffers is full  we need to handle packets which be received
    RET_RECV_BUFFER_NOT_ENOUGH,		//接收缓冲区不足


};

typedef unsigned char BYTE;

struct RECV_FRAME
{
    unsigned int u32DataType;
    int  nFrameStat;
    int  nStatus;
    int nLen;
    BYTE* pData;
};


struct PACKET_INFO
{
    BYTE* pRecv;		//receive raw data
    int nReadPos;
    int nWritePos;
    BYTE* pTemp;
    int nCurrPkt;
    struct RECV_FRAME packet[RECV_PACKET_NUM];

    int recv_packet_size;		//接收包缓冲
    int recv_buf_size;			//接收内存缓冲

    int (*packet_init)(struct PACKET_INFO* pInfo);
    int (*packet_uninit)(struct PACKET_INFO* pInfo);
    int (*process_packet)(struct PACKET_INFO* pInfo, BYTE* pBuf, int nBufLen);
    int (*is_have_packet)(struct PACKET_INFO* pInfo);
    int (*get_a_packet)(struct PACKET_INFO* pInfo, unsigned char* pBuf, int* nLen, unsigned int* pu32DataType);

    clock_t recvTimeOut;
};


//extern struct PACKET_INFO packet_info;
extern struct PACKET_INFO packet_info_big;
extern struct PACKET_INFO packet_info_small;

#endif //__WIFI_DECODE_PACKET_H__
