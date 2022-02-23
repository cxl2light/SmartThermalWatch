/*
 * zs_CmdProcess.h
 *
 *  Created on: 2016-10-14
 *      Author: yst
 */

#ifndef ZS_CMDPROCESS_H_
#define ZS_CMDPROCESS_H_

#include "zs_Types.h"
#include "zs_CmdList.h"


typedef enum
{
	CMD_STEM_FROM_PC	= 0,
	CMD_STEM_FROM_DSP1,
	CMD_STEM_FROM_DSP2,
	CMD_STEM_FROM_END
}CMDSTEMFROM;




ZS_RESULT zs_CPInit(void);

/*U16 zs_CPGetPacketMemMaxLenth(void)
 * 每个包最大数据长度
 */
U16 zs_CPGetPacketMemMaxLenth(void);

/*U16 zs_CPMakePacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U8 u8CmdStemFrom, U16 u16Lenth)
 *返回0，表示失败，其它表示包的大小
 * U32 u32Cmd:		 命令
 * U8 u8CmdStemFrom：命令来着哪里，例如PC,DSP等
 */
U16 zs_CPMakePacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U8 u8CmdStemFrom, U16 u16Lenth);

/*U32 zs_CPMakeLargeDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd,  U32 u32Lenth)
 *返回0，表示失败，其它表示总包的大小
 * 对大数据进行打包，多个包
 * U32 u32Cmd:		 命令
 */
U32 zs_CPMakeLargeDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U32 u32Lenth);

/*U32 zs_CPMakeUpdateDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd,  U32 u32Lenth)
 *返回0，表示失败，其它表示包的大小
 * U32 u32Cmd:		 命令
 */
U32 zs_CPMakeUpdateDataPacket(void *pvDst, void *pvdSrc, U32 u32Cmd, U32 u32Lenth);

/*bool zs_CPGetUpdatePacketInfo( void ** pvdThisPktAddr, U16 * pu16ThisPktLenth, void * pvdDataSrc)
*得到打包好的升级文件数据的包信息
*返回：若为true，则成功，若为false则表示失败
*/
bool zs_CPGetUpdatePacketInfo( void ** pvdPktStartAddr, U16 * pu16ThisPktLenth, void * pvdDataSrc);

#endif /* ZS_CMDPROCESS_H_ */