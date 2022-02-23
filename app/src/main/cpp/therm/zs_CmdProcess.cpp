/*
 * zs_PacketCmdProcess.c
 *
 *  Created on: 2016-10-14
 *      Author: yst
 */
//#include "stdafx.h"
#include "zs_CmdProcess.h"
#include"string.h"
//#include"windows.h"

typedef enum
{
	UPDATE_PACKET_RESET = 0,
	UPDATE_PACKET_INFO,
	UPDATE_PACKET_FIRST,

	UPDATE_PACKET_END = 0xFF00,
	UPDATE_PACKET_SAVE,

}UPDATE_PACKET_TYPE;

/*命令格式
 * 		包头1字节		命令起源标示1字节		附带数据长度2字节	命令4字节		数据，u16DataLenth字节		校验和1字节			包尾1字节
 * 	|	u8Header=0xff	|	u8CmdStemFrom		|	u16DataLenth	|	u32Cmd		|	数据					|	u8Check			|	u8Tail	|
 *
 */

typedef struct
{
	U8 u8Header;
	U8 u8CmdStemFrom;	//命令出自于哪里，例如网络，串口等
	U16 u16DataLenth;	//附带数据长度。
	U32 u32Cmd;			//命令
}ZS_PACKET_FRONT;



typedef bool (*FUNCPTR) (void * ,U16);

typedef struct
{
	U32 u32Cmd;
	FUNCPTR fnRoutie;
}COMMAND_FUN;


#define	PACKET_HEADER				0XFF
#define	PACKET_TAIL					0XFF
#define	PACKET_LENTH_MAX			(63*1024)
#define PACKET_EXTRA_LENTH			(sizeof(ZS_PACKET_FRONT)+2)
#define CMDDATA_LENTH_MAX			(PACKET_LENTH_MAX - PACKET_EXTRA_LENTH)
#define UPDATEDATA_LENTH_MAX		(CMDDATA_LENTH_MAX - 2)


bool OnCommandTransOriData(void * pvdPacket, U16 u16Lenth);
bool OnCommandTransMTSFile(void * pvdPacket, U16 u16Lenth);
bool OnCommandTransDSP2ProgramNormal(void * pvdPacket, U16 u16Lenth);

#define	ZS_ON_COMMAND(cmd,fun)			{cmd,fun},

static COMMAND_FUN g_stCommandFun[] = {
		ZS_ON_COMMAND(COMMAND_TRANS_ORIDATA,					OnCommandTransOriData)
		ZS_ON_COMMAND(COMMAND_TRANS_MTSFILE,					OnCommandTransMTSFile)		
};


ZS_RESULT zs_CPInit(void)
{

	return ZS_RESULT_SUCCESS;
}

/*bool zs_CPSubmitOnePacket(void * pvdPacket, U16 u16Lenth)
 * 提交一个数据包，并处该数据包
 */
bool zs_CPSubmitOnePacket(void * pvdPacket, U16 u16Lenth)
{
	if(pvdPacket == 0)
		return false;
	ZS_PACKET_FRONT * pstPacket;

	pstPacket = (ZS_PACKET_FRONT *)pvdPacket;

	if(pstPacket->u8Header != PACKET_HEADER)
		return false;
	if(u16Lenth < pstPacket->u16DataLenth)
		return false;
	U8 *pu8Tail;
	pu8Tail = (U8*)pvdPacket + (sizeof(ZS_PACKET_FRONT)+1+pstPacket->u16DataLenth);
	if(*pu8Tail != PACKET_TAIL)
		return false;
	U8 * pu8DataSrc;
	pu8DataSrc = (U8*)pvdPacket + sizeof(ZS_PACKET_FRONT);

	U32 i;
	for(i=0;i<sizeof(g_stCommandFun)/sizeof(COMMAND_FUN);i++)
	{
		if(g_stCommandFun[i].u32Cmd == pstPacket->u32Cmd)
		{
			g_stCommandFun[i].fnRoutie(pu8DataSrc, pstPacket->u16DataLenth);
			break;
		}
	}

	return true;
}

/*U16 zs_CPGetPacketMemLenth(U16 u16DataLenth)
 * 对于长度为u16DataLenth的数据，返回打包后数据的长度
 */
U16 zs_CPGetPacketMemLenth(U16 u16DataLenth)
{
	return u16DataLenth + PACKET_EXTRA_LENTH;
}


/*U16 zs_CPGetPacketMemMaxLenth(void)
 * 每个包最大数据长度
 */
U16 zs_CPGetPacketMemMaxLenth(void)
{
	return CMDDATA_LENTH_MAX;
}
U8 CheckSum1Byte(U8* pu8Buf, U32 u32Lenth)
{
	U32 i;
	U8 u8NewValue = 0;
	if(pu8Buf == 0 || u32Lenth == 0)
		return 0;
	for(i=0;i<u32Lenth;i++)
		u8NewValue += pu8Buf[i];
	return u8NewValue;
}
/*U16 zs_CPMakePacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U8 u8CmdStemFrom, U16 u16Lenth)
 *返回0，表示失败，其它表示包的大小
 * U32 u32Cmd:		 命令
 * U8 u8CmdStemFrom：命令来着哪里，例如PC,DSP等
 */
U16 zs_CPMakePacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U8 u8CmdStemFrom, U16 u16Lenth)
{
	U16 u16DataLenth;

	if(pvDst == 0)
		return 0;
    else if(u16Lenth> PACKET_LENTH_MAX)
        return 0;
	ZS_PACKET_FRONT * pstPacket;
	U8 * pu8Check;

	if(pvdSrc == 0)
		u16DataLenth = 0;
	else
		u16DataLenth = u16Lenth;
	pstPacket = (ZS_PACKET_FRONT *)pvDst;
	pstPacket->u8Header			= PACKET_HEADER;
	pstPacket->u8CmdStemFrom	= u8CmdStemFrom;
	pstPacket->u32Cmd			= u32Cmd;
	pstPacket->u16DataLenth		= u16DataLenth;
	if(u16DataLenth != 0)
		memcpy(pstPacket+1,pvdSrc,u16DataLenth);
	pu8Check = (U8*)pvDst + sizeof(ZS_PACKET_FRONT) + u16DataLenth;
	*pu8Check = CheckSum1Byte((U8*) pvdSrc, u16Lenth);
	*(pu8Check+1) = PACKET_TAIL;

	return u16DataLenth+PACKET_EXTRA_LENTH;

}


/*U32 zs_CPMakeLargeDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd,  U32 u32Lenth)
 *返回0，表示失败，其它表示包的大小
 * U32 u32Cmd:		 命令
 */
U32 zs_CPMakeLargeDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U32 u32Lenth)
{
	U16 u16DataLenth;
	U32 u32RestDataLenth;
	U32 u32AllLenth;
	U8 *pu8Src;

	if(pvDst == 0)
		return 0;

	ZS_PACKET_FRONT * pstPacket;
	U8 * pu8Check;
	pu8Src = (U8*)pvdSrc;
	if(pu8Src == 0)
		return 0;

	u32RestDataLenth	= u32Lenth;
	u32AllLenth			= 0;
	pstPacket			= (ZS_PACKET_FRONT *)pvDst;
	while(u32RestDataLenth > 0)
	{
		if(u32RestDataLenth<=CMDDATA_LENTH_MAX)
		{
			u16DataLenth		= (U16)u32RestDataLenth;
			u32RestDataLenth	= 0;
		}
		else
		{
			u16DataLenth		= CMDDATA_LENTH_MAX;
			u32RestDataLenth	= u32RestDataLenth - CMDDATA_LENTH_MAX;
		}
		
		pstPacket->u8Header			= PACKET_HEADER;
		pstPacket->u8CmdStemFrom	= CMD_STEM_FROM_PC;
		pstPacket->u32Cmd			= u32Cmd;
		pstPacket->u16DataLenth		= u16DataLenth;
		if(u16DataLenth != 0)
			memcpy(pstPacket+1,pu8Src,u16DataLenth);
		pu8Check = (U8*)pstPacket + sizeof(ZS_PACKET_FRONT) + u16DataLenth;
		*pu8Check = CheckSum1Byte((U8*) pvdSrc, u32Lenth);
		*(pu8Check+1)	= PACKET_TAIL;
		u32AllLenth		+= u16DataLenth + PACKET_EXTRA_LENTH;
		pstPacket	= (ZS_PACKET_FRONT *)((U8*)pvDst+u32AllLenth);
		pu8Src		= pu8Src + u16DataLenth;
		
	}
	return u32AllLenth;

}


/*U32 zs_CPMakeUpdateDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd,  U32 u32Lenth)
 *返回0，表示失败，其它表示包的大小
 * U32 u32Cmd:		 命令
 */
U32 zs_CPMakeUpdateDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U32 u32Lenth)
{
	#define	BUFF_LENTH		32
	U16 u16DataLenth;
	U32 u32RestDataLenth;
	U32 u32AllLenth;
	U8 *pu8Src;
	U8 u8CmdData[BUFF_LENTH];
	U16 *pu16CmdData = (U16*)&u8CmdData[0];
	void* pvdPktInfo;
	U16 *pu16Temp,u16RealNo;

	if(pvDst == 0)
		return 0;

	ZS_PACKET_FRONT * pstPacket;
	U8 * pu8Check;
	pu8Src = (U8*)pvdSrc;
	if(pu8Src == 0)
		return 0;

	//UPDATE_PACKET_RESET
	*pu16CmdData		= UPDATE_PACKET_RESET;
	*(pu16CmdData+1)	= 0;
	pstPacket	= (ZS_PACKET_FRONT *)pvDst;
	u32AllLenth = zs_CPMakePacket((void*)pstPacket, (void*)&u8CmdData[0], u32Cmd, CMD_STEM_FROM_PC, 2);//数据要进行4字节对齐
	pstPacket	= (ZS_PACKET_FRONT *)((U8*)pvDst+u32AllLenth);

	//UPDATE_PACKET_INFO
	*pu16CmdData		= UPDATE_PACKET_INFO;
	*(pu16CmdData+1)	= (u32Lenth+UPDATEDATA_LENTH_MAX-1)/UPDATEDATA_LENTH_MAX;
	*(pu16CmdData+2)	= u32Lenth&0xffff;
	*(pu16CmdData+3)	= (u32Lenth>>16)&0xffff;
	pvdPktInfo			= (void*)pstPacket;
	u32AllLenth += zs_CPMakePacket((void*)pvdPktInfo, (void*)&u8CmdData[0], u32Cmd, CMD_STEM_FROM_PC, 10);//数据要进行4字节对齐

	pstPacket			= (ZS_PACKET_FRONT *)((U8*)pvDst+u32AllLenth);
	u32RestDataLenth	= u32Lenth;
	u16RealNo			= UPDATE_PACKET_FIRST;
	while(u32RestDataLenth > 0)
	{
		if(u32RestDataLenth<=UPDATEDATA_LENTH_MAX)
		{
			u16DataLenth		= (U16)u32RestDataLenth;
			u32RestDataLenth	= 0;
		}
		else
		{
			u16DataLenth		= UPDATEDATA_LENTH_MAX;
			u32RestDataLenth	= u32RestDataLenth - UPDATEDATA_LENTH_MAX;
		}
		
		pstPacket->u8Header			= PACKET_HEADER;
		pstPacket->u8CmdStemFrom	= CMD_STEM_FROM_PC;
		pstPacket->u32Cmd			= u32Cmd;
		pstPacket->u16DataLenth		= u16DataLenth + 2;//2表示u16RealNo字节数
		pu16Temp = (U16*)(pstPacket+1);
		*pu16Temp = u16RealNo;
		if(u16DataLenth != 0)
			memcpy(pu16Temp+1,pu8Src,u16DataLenth);
		pu8Check = (U8*)pstPacket + sizeof(ZS_PACKET_FRONT) + u16DataLenth + 2;//2表示u16RealNo字节数
		*pu8Check = CheckSum1Byte( pu8Src, u16DataLenth);
		*(pu8Check+1)	= PACKET_TAIL;
		u32AllLenth		+= u16DataLenth + PACKET_EXTRA_LENTH + 2;//2表示u16RealNo字节数
		pstPacket	= (ZS_PACKET_FRONT *)((U8*)pvDst+u32AllLenth);
		pu8Src		= pu8Src + u16DataLenth;
		u16RealNo++;
	}
	//UPDATE_PACKET_END
	*pu16CmdData		= UPDATE_PACKET_END;
	*(pu16CmdData+1)	= 0;
	pvdPktInfo			= (void*)((U8*)pvDst+u32AllLenth);
	u32AllLenth += zs_CPMakePacket((void*)pvdPktInfo, (void*)&u8CmdData[0], u32Cmd, CMD_STEM_FROM_PC, 2);//数据要进行4字节对齐
	//UPDATE_PACKET_SAVE
	*pu16CmdData		= UPDATE_PACKET_SAVE;
	*(pu16CmdData+1)	= 0;
	pvdPktInfo			= (void*)((U8*)pvDst+u32AllLenth);
	u32AllLenth += zs_CPMakePacket((void*)pvdPktInfo, (void*)&u8CmdData[0], u32Cmd, CMD_STEM_FROM_PC, 2);//数据要进行4字节对齐

	return u32AllLenth;

}

/*bool zs_CPGetUpdatePacketInfo( void ** pvdThisPktAddr, U16 * pu16ThisPktLenth, void * pvdDataSrc)
*得到打包好的升级文件数据的包信息
*返回：若为true，则成功，若为false则表示失败
*/
bool zs_CPGetUpdatePacketInfo( void ** pvdPktStartAddr, U16 * pu16ThisPktLenth, void * pvdDataSrc)
{
	ZS_PACKET_FRONT * pstPacket;
	pstPacket = (ZS_PACKET_FRONT *)pvdDataSrc;
	U8* pu8Check;
	U8 * pu8Data;
	U16 u16PacketNo;

	if(pstPacket->u8Header != PACKET_HEADER)
		return false;
	if(pstPacket->u8CmdStemFrom != CMD_STEM_FROM_PC)
		return false;
	if(pstPacket->u16DataLenth > CMDDATA_LENTH_MAX)
		return false;

	pu8Check = (U8*)pstPacket + sizeof(ZS_PACKET_FRONT) + pstPacket->u16DataLenth;
	pu8Data = (U8*)pstPacket + sizeof(ZS_PACKET_FRONT);
	u16PacketNo = *pu8Data++;
	u16PacketNo += (*pu8Data++)<<8;
	if( (u16PacketNo >= UPDATE_PACKET_FIRST) && (u16PacketNo<UPDATE_PACKET_END) )
	{
		U8 u8CheckSum = CheckSum1Byte( (U8*)pstPacket + sizeof(ZS_PACKET_FRONT)+ 2, pstPacket->u16DataLenth-2);//2表示u16RealNo字节数
		if(  (u8CheckSum != *pu8Check) || (*(pu8Check+1) != PACKET_TAIL) )
			return false;
	}
	else
	{
		if(*(pu8Check+1)	!= PACKET_TAIL )
			return false;
	}
	*pvdPktStartAddr	= pvdDataSrc;
	*pu16ThisPktLenth	= pstPacket->u16DataLenth + PACKET_EXTRA_LENTH;

	return true;
}



bool OnCommandTransOriData(void * pvdPacket, U16 u16Lenth)
{

	return true;
}


bool OnCommandTransMTSFile(void * pvdPacket, U16 u16Lenth)
{

	return true;
}





