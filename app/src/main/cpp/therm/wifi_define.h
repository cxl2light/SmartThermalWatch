#ifndef __WIFI_DEFINE_H__
#define __WIFI_DEFINE_H__

//#include "rsi_global.h"
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned short uint16_t;
typedef int int32;

enum
{
    FRAME_DATA,
    FRAME_CMD,
};

//文件存储时的文件头
typedef struct tagWIFI_FRAME_HEADER
{
    char strFrameType[4];      //frame flag "zsdf"
    uint32 uDataType;
    uint32 uHdrCRC;
    uint32 uDataCRC;
    uint32 uTransFrameHdrSize;	//sizeof(WIFI_FRAME_HEADER)
    uint32 uTransFrameSize;		//sizeof(WIFI_FRAME_HEADER) + Data Size
    uint32 uTransFrameNo;		//传输帧流水号

    //数据帧变量
    uint32 uDataFrameSize;		//数据帧的总大小
    uint32 uDataFrameTotal;		//一帧数据被分成传输帧的个数
    uint32 uDataFrameCurr;		//数据帧当前的帧号
    uint32 uDataInFrameOffset;   	//数据帧在整帧的偏移
    uint8 Data[0];

#if 0
    char strFrameType[4];      //frame flag "ZSDF"
    U32 uHdrCRC;
    U32 uDataCRC;
    U32 uTransFrameHdrSize;	//sizeof(WIFI_FRAME_HEADER)
    U32 uTransFrameSize;		//sizeof(WIFI_FRAME_HEADER) + Data Size
    U32 uTransFrameNo;		//传输帧流水号

    //数据帧变量
    U32 uDataFrameSize;		//数据帧的总大小
    U32 uDataFrameTotal;		//一帧数据被分成传输帧的个数
    U32 uDataFrameCurr;		//数据帧当前的帧号
    U32 uDataInFrameOffset;   	//数据帧在整帧的偏移
    U8 Data[];
#endif




}WIFI_FRAME_HEADER;

//class tagFRAME_DATA
//{
//	int m_frameNO; //帧号
//	int m_totalpackets;//每一帧的总包数
//	int m_currentPackets;//当前的包数
//	int m_valid;
//	cv::Mat image;
//
//}FRAME_DATA;

uint16_t cyg_crc16(const unsigned char *buf, int len); //CRC校验



#endif //__WIFI_DEFINE_H__
