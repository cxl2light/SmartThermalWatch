
#ifndef ZS_TYPES_H_
#define ZS_TYPES_H_

//#include "MtType.h"

typedef unsigned char  		U8;
typedef	char  				S8;
typedef unsigned short int	U16;
typedef signed   short int	S16;
typedef unsigned int  		U32;
typedef signed int    		S32;
typedef long long  		    U64;

typedef enum
{
	ZS_RESULT_FALSE		= 0,
	ZS_RESULT_SUCCESS,			
	ZS_RESULT_MALLOC_FAILED,					//内存分配失败
	ZS_RESULT_HANDLE_ERROR,						//句柄为空
	ZS_RESULT_EXTERN_SENDPACKET_FAILED,			//Extern_SendPacket返回错误
	ZS_RESULT_EXTERN_RCVPACKET_NULL,
	ZS_RESULT_EXTERN_SENDPACKET_NULL,

	ZS_RESULT_INVALID_NUMBER,					//数字无效，例如点，区域等的序号，
	ZS_RESULT_DATA_CHECK_FAILED,
	ZS_RESULT_NONE
}ZS_RESULT;

typedef enum
{
	IMAGE_CONTRAST_BRIGHT_AUTO 	= 0	,			//亮度对比度全自动
	IMAGE_CONTRAST_AUTO	,						//对比度自动，亮度手动
	IMAGE_BRIGHT_AUTO		,					//亮度自动，对比度手动
	IMAGE_CONTRAST_BRIGHT_MANUAL,				//亮度对比度全手动
	IMG_MODE_END
} IMAGE_CONTRAST_BRIGHT_MODE;

//网络数据包类型

#if 0
typedef enum
{
	PACKET_VERSION=0,						//版本信息包，里面包含了各种数据包的版本信息，必须得到该包后才能做各种包的解析
	PACKET_IMG,								//原始图像数据
	PACKET_MTPARADATA,						//测温参数内存数据
	PACKET_MTRECORDFILE,					//测温采集数据文件
	PACKET_MTREALTIMEINFO,					//测温实时信息
	PACKET_IMG_MTPARAM_MTAPP_JPG,			//测温的所有数据及信息
	PACKET_TYPE_END							//

}NET_PACKET_TYPE;

#elif 0
//网络数据包结构
typedef enum
{
    PACKET_VERSION=0,						//版本信息包，里面包含了各种数据包的版本信息，必须得到该包后才能做各种包的解析
    PACKET_IMG,								//图像数据包
    PACKET_MTPARADATA,						//测温参数内存数据
    PACKET_MTRECORDFILE,					//测温采集数据文件
    PACKET_MTREALTIMEINFO,					//测温实时信息
    PACKET_IMG_MTPARAM_MTAPP_JPG,			//测温的所有数据及信息

    PACKET_RESPONSE_CMD_RESULT = 0x80,				//仪器更新后的反馈信息
    PACKET_DEV_INFO_RESPONSE,

    PACKET_CMD_BEGIN = 0x1000,
    RES_UPDATE_DSP2_PROGRAM_DEFAULT,
    RES_UPDATE_DSP2_PROGRAM_NORMAL,
    RES_UPDATE_DSP2_PROGRAM_BOOTLOADER,
    RES_UPDATE_DSP2_COMMON_DATA,
    RES_SET_IP_ADDR,
    RES_SET_MAC_VALUE,
    RES_SET_RAW_FRAME_RATE,
    RES_SET_VIDEO_PROFILE,
    RES_SET_VIDEO_BITRATE,
    RES_SET_VIDEO_FRAME_RATE,
    RES_SET_UART1_OUTPUT,
    RES_SET_REBOOT,
    RES_SET_USER_PASSWORD,
    RES_SET_UART1_BAUD,
    RES_GET_DEVICE_INFO,
    RES_GET_ENCODE_INFO,
    RES_SET_TCP_PORT,
    RES_SET_UDP_PORT,
    RES_SET_RAW_DATA_SPEED,
    RES_GET_DEV_INFO,
    RES_SET_LOCAL_TIME,
    RES_SET_OSD_DISP_TIME,
    RES_SET_OSD_DISP_DEVNAME,
    RES_GET_FIRMWARE_INFO,
    RES_SET_FIRMWARE_INFO,
    RES_RESTORE_DEFAULT_CONFIG,
    RES_SET_IMAGE_CSC_ATTR,
    RES_SET_IMAGE_DCI_PARAM,
    RES_PTZ_TO_RS485,

    RES_SET_ENCODE_JPEG= 0x1000 + 0x100,
    RES_GET_JPEG,
    RES_HEARTBEAT_PACKET,
    RES_SYNC_DSP,

    RES_BEGIN_UPGRADE	,  //开始升级
    RES_STOP_UPGRADE,       //停止升级
    RES_GET_VIDEO_CONFIG,
    RES_GET_VIDEO_CONFIG_SUB,
    RES_GET_IMAGE_CONFIG,
    RES_GET_RAW_FRAME_CONFIG,
    RES_GET_OSD_CONFIG,
    RES_SET_JPG_CONFIG,
    RES_SET_UART2_BAUD,
    RES_SET_UART_ATTR,      //设置串口属性
    RES_GET_CRUISE_PATH,    //获取巡航路径
    RES_SET_CRUISE_PATH,    //设置巡航路径
    RES_DO_CRUISE_PATH,     //执行巡航路径
    RES_GET_PC_DATE,        //获取PC数据
    RES_SET_PC_DATE,        //设置PC数据
    RES_GET_CRUISE_STATE,   //获取巡航状态
    RES_SET_DEVICE_NAME,    //设置设备名称
    RES_CURRENT_CRUISE_PRESET, //当前的巡航预置点
    RES_SET_DEV_INFO,
    RES_WRITE_FILE_TO_SD   ,
    RES_READ_FILE_FROM_SD  ,
    RES_WRITE_DATA_TO_RS232  ,
    RES_WRITE_DATA_TO_RS485  ,
    RES_EVENT,              //事件
    RES_KoreanShinwoo,      //返回索引
    RES_GET_ALL_PRESET_NAMES,
    RES_PTZ_TO_RS232,
    RES_DATA_FROM_RS485,
    RES_DATA_FROM_RS232,

    RES_SET_DATA_STREAM_ATTR,   //设置流属性
    RES_GRAY_ALARMINFO,         //灰度报警反馈
    RES_CURTIME,
    RES_DSPCONFIG,
    RES_GRAY_HIGHTEMP,
    RES_GRAY_ALARMSTATE,

    RES_SET_GE_HEATING_ATTR,
    RES_GET_GE_HEATING_ATTR,
    RES_SET_STREAM_NAME,
    RES_GET_STREAM_NAME,

    RES_GET_RUNTIME_INFO,       //获取运行时信息
    RES_PUSH_START_TIME_STAMP,  //推送时间戳
    RES_GET_PTZSCHEDULE,        //定时巡检
    RES_SET_SND_TIMEOUT,        //超时设置
    RES_GET_ONVIFALARM,         //Onvif 报警
    RES_SET_PTZSCHEDULE,        //定时巡检 成功标志
    RES_GET_DEVTYPE_NAME,       //设备类型名
    RES_MTINFO_RECT,            //MT_RECT 实时测温框

    RES_DEVICE_STOPQUERY,
    RES_PTZ_VERSIONQUERY,
    RES_PTZ_DEVICEINFO,         //PTZ设备信息
    RES_GPS,                    //GPS信息
    RES_COOLING_WORKTIME,       //制冷机工作时长
    RES_LASERINFO,              //激光测距值回传

    RES_LOCALGPS,               //本地GPS

    PACKET_TYPE_END,
}NET_PACKET_TYPE;

#else

//网络数据包结构
typedef enum
{
    RES_GET_CAMERA,         //获取设备信息
	RES_GET_DEVCONFIG,      //获取设备参数
	RES_UPGRADE,            //升级结果
    RES_REBOOT,             //重启
	RES_ANALYSERESULT, 		//ai检测结果
	PACKET_END,
}NET_PACKET_TYPE;


#endif

typedef enum
{
	IMG_AUTO_ALL 	= 0	,                //全自动
	IMG_AUTO_GANIN		,                //自动增益
	IMG_AUTO_BRIGHT		,				 //自动亮度
	IMG_MANUAL			,				 //全手动
} IMG_AGC_MODE;

typedef enum
{
	IMG_SC_STADARD 	= 0	,                //跟随色标的顶底端颜色
	IMG_SC_WHITEBLACK	,                //高温白，低温黑
	IMG_SC_REDBLUE		,				 //高温红，低温蓝
	IMG_SC_END			,				 //
} IMG_SATURATED_COLOR;

typedef void*  ZS_MTONLINE_HANDLE;
typedef int (*Extern_RcvPacket) (void * pParam, char * pchBuf ,int nByteNum);	//返回值表示接收到的数据字节数；char * pchBuf ：接收到的数据地址；int nByteNum：接收的数据长度。

typedef int (*Extern_RcvPacketEx)(void * pParam, char** pBuf, int* nLen, unsigned int* pu32DataType);

typedef bool (*Extern_SendPacket) (void * pParam,char * pchBuf ,int nByteNum);	//返回值true表示成功；char * pchBuf ：发送数据地址；int nByteNum：发送数据长度。建议采用阻塞方式

typedef enum
{
	VIRTUAL_KEY_MENU = 0,
	VIRTUAL_KEY_LEFT,
	VIRTUAL_KEY_RIGHT,
	VIRTUAL_KEY_UP,
	VIRTUAL_KEY_DOWN,
	//长按
	VIRTUAL_KEY_MENU_LONG,
	VIRTUAL_KEY_LEFT_LONG,
	VIRTUAL_KEY_RIGHT_LONG,
	VIRTUAL_KEY_UP_LONG,
	VIRTUAL_KEY_DOWN_LONG,

    VIRTUAL_KEY_STOP, //20180419 chenjian 要求加入该命令

}VIRTUAL_KEYBOARD;


typedef enum
{
    IMG_COLORRULE_BLACK = 0,                //黑热色标
    IMG_COLORRULE_WHITE = 1,                //白热色标
    IMG_COLORRULE_01,						//铁红
    IMG_COLORRULE_02,						//彩虹
    IMG_COLORRULE_03,						//熔岩
    IMG_COLORRULE_04,						//彩虹增强
    IMG_COLORRULE_05,						//熔融金属
    IMG_COLORRULE_06,						//蓝红
    IMG_COLORRULE_07,						//琥珀
    //初始数据版本的色标到此为止
    IMG_COLORRULE_08,
    IMG_COLORRULE_09,
    IMG_COLORRULE_10,
    IMG_COLORRULE_11,
    IMG_COLORRULE_12,
    IMG_COLORRULE_13,
    IMG_COLORRULE_14,
    IMG_COLORRULE_15,
    IMG_COLORRULE_16,
    IMG_COLORRULE_NONE
} COLORRULE_TYPE;

enum ColorRule
{
    IMG_COLORRULE_BLACK_HOT = 0,    //黑热色标
    IMG_COLORRULE_WHITE_HOT,        //白热色标
    IMG_COLORRULE_IRON,				//铁红
    IMG_COLORRULE_RAINBOW,			//彩虹
    IMG_COLORRULE_LAVA,				//熔岩
    IMG_COLORRULE_RAINBOW_ENHANCE,	//彩虹增强
    IMG_COLORRULE_MOLTEN_METAL,		//熔融金属
    IMG_COLORRULE_BLUE_RED,			//蓝红
    IMG_COLORRULE_AMBER,			//琥珀
};


typedef enum
{
	MTAPP_OBJ_ENABLE_BIT	= 0,	//测温对象测温功能是否打开标志位
	MTAPP_OBJ_REVISE_BIT	= 1,	//测温对象温度修正功能是否打开标志位
}MTAPP_OBJ_CONTROL_FLAG;//0~31

#define MT_OBJ_CONTROL_MASK_ENABLE	(1u<<MTAPP_OBJ_ENABLE_BIT)
#define MT_OBJ_CONTROL_MASK_REVISE	(1u<<MTAPP_OBJ_REVISE_BIT)


typedef enum
{
	ALARM_STATUS_BIT					= 0,	//目标是否报警
	ALARM_HIGHER_TEMPER_STATUS_BIT		= 1,	//高温报警
	ALARM_LOWER_TEMPER_STATUS_BIT		= 2,	//低温报警
	ALARM_GLOBAL_STATUS_BIT				= 3,	//全局是否有报警
	ALARM_HIGH_SHOW_BIT					= 4,	//测温对象高温是否显示
	ALARM_LOW_SHOW_BIT					= 5,	//测温对象低温是否显示
	ALARM_FLASH_BIT						= 6,	//如果报警了，测温对象是否闪烁标志位
	ALARM_HIGH_FLASH_BIT					= 4,	//如果报警了,高温是否闪烁
	ALARM_LOW_FLASH__BIT					= 5,	//如果报警了,低温是否闪烁
	ALARM_FLAG_END
}ALARM_FLAG;//0~31

#define	MT_ALARM_FLAG_STATUS_TARGET		(1u<<ALARM_STATUS_BIT)
#define	MT_ALARM_FLAG_STATUS_HIGH		(1u<<ALARM_HIGHER_TEMPER_STATUS_BIT)
#define	MT_ALARM_FLAG_STATUS_LOW		(1u<<ALARM_LOWER_TEMPER_STATUS_BIT)
#define	MT_ALARM_FLAG_STATUS_GLOBAL		(1u<<ALARM_GLOBAL_STATUS_BIT)
#define	MT_ALARM_FLAG_STATUS_HIGH_SHOW	(1u<<ALARM_HIGH_SHOW_BIT)
#define	MT_ALARM_FLAG_STATUS_LOW_SHOW	(1u<<ALARM_LOW_SHOW_BIT)
#define	MT_ALARM_FLAG_STATUS_FLASH		(1u<<ALARM_FLASH_BIT)
#define	MT_ALARM_FLAG_STATUS_HIGH_FLASH		(1u<<ALARM_HIGH_FLASH_BIT)
#define	MT_ALARM_FLAG_STATUS_LOW_FLASH		(1u<<ALARM_LOW_FLASH__BIT)

typedef struct
{
    U16 u16X;					//水平方向坐标
    U16 u16Y;					//垂直方向坐标
}ZS_POINT;

typedef enum
{
    ALARM_TYPE_HIGHER = 0,
    ALARM_TYPE_LOWER,
    ALARM_TYPE_INTER,
    ALARM_TYPE_OUTER,
    ALARM_TYPE_END
}ALARM_TYPE;			//报警模式

typedef struct
{
    ALARM_TYPE 		enAlarmType;			//报警的类型 超温报警 低温报警 区间报警 两头报警
    U32			 	u32AlarmlFlag;			//ALARM_FLAG,报警标志位
    float			ftHighTemper;			//超温报警时的报警温度 低温报警时无效 区间报警跟两头报警时的温度上限
    float			ftLowTemper;			//低温报警时的报警温度 高温报警时无效  区间报警跟两头报警时的温度下限
    U8				u8AlarmIoOutSwitch;		//是否打开io通断报警
    U8				u8Reserve[7];			//
}ALARM_INFO;

typedef struct
{
    U32 					u32CtrlFlag;			//控制标志位,MTAPP_OBJ_CONTROL_FLAG
    ZS_POINT 				stPt;					//点坐标
    U16 					u16EmissX1000;			//比辐射率x1000
    S16 					s16Gray;				//该点灰度值
    U32 					u32ObjDistX10;			//测温目标距离*10，单位为分米，例如1米距离，u32DistantX10=10;
    float 					ftTemper;				//该点温度值
    float 					ftBKTemper;				//目标背景温度
    float					ftReviseTemper;			//修正温度
    ALARM_INFO 				stAlarmType;			//报警信息
    U8						u8Reserve[8];			//保留

}MT_POINT;

typedef struct
{
    U32 					u32CtrlFlag;			//控制标志位,MTAPP_OBJ_CONTROL_FLAG
    ZS_POINT 				stStartPt;			//线段的起始端点
    ZS_POINT 				stEndPt;			//线段的结束端点
    U16 					u16EmissX1000;			//比辐射率x1000
    S16 					s16AvgGray;				//线段灰度平均值
    S16 					s16MinGray;				//线段灰度最小值
    S16 					s16MaxGray;				//线段灰度最大值
    U32 					u32ObjDistX10;			//测温目标距离*10，单位为分米，例如1米距离，u32DistantX10=10;
    float 					ftAvgTemper;			//线段区域平均温度
    float 					ftMinTemper;			//线段区域最低温度
    float 					ftMaxTemper;			//线段区域最高温度
    float 					ftBKTemper;				//目标背景温度
    float					ftReviseTemper;			//修正温度
    ZS_POINT 				stMaxPt;			//线段灰度最大值像素点的坐标
    ZS_POINT 				stMinPt;			//线段灰度最小值像素点的坐标
    ALARM_INFO 				stAlarmType;			//报警信息
    U8						u8Reserve[8];			//保留
}MT_LINE;

typedef struct
{
    U32 					u32CtrlFlag;			//控制标志位,MTAPP_OBJ_CONTROL_FLAG
    ZS_POINT 				stTopLeft;			//矩形左上点坐标
    ZS_POINT 				stDownRight;		//矩形右下点坐标
    U16 					u16EmissX1000;			//比辐射率x1000
    S16 					s16AvgGray;				//矩形区域灰度平均值
    S16 					s16MinGray;				//矩形区域灰度最小值
    S16 					s16MaxGray;				//矩形区域灰度最大值
    U32 					u32ObjDistX10;			//测温目标距离*10，单位为分米，例如1米距离，u32DistantX10=10;
    float 					ftAvgTemper;			//矩形区域平均温度
    float 					ftMinTemper;			//矩形区域最低温度
    float 					ftMaxTemper;			//矩形区域最高温度
    float 					ftBKTemper;				//目标背景温度
    float					ftReviseTemper;			//修正温度
    ZS_POINT 				stMaxPt;			//矩形区域灰度最大值像素点的坐标
    ZS_POINT 				stMinPt;			//矩形区域灰度最小值像素点的坐标
    ALARM_INFO 				stAlarmType;			//报警信息
    U8						u8Reserve[8];			//保留
}MT_RECT;

typedef struct
{
	U8			u8StallNo;			//当前档位
	U8			u8StallNum;			//档位数量
	U8			u8LensType;			//镜头类型号
	U8			u8Hum;				//湿度 1-100之间
	float 		ftStallMaxTemper;	//当前档位测温最大温度值
	float 		ftStallMinTemper;	//当前档位测温最小温度值
	float 		ftEmissivity;		//比辐射率
	float 		ftAmbientTemper;	//环温温度
	float 		ftBKTemper;			//目标背景温度
	U32 		u32ObjDistX10;		//测温目标距离*10，单位为分米，例如1米距离，u32DistantX10=10;
}MT_PARAM_INFO;
typedef struct
{
	U16 u16With;					//水平方向坐标
	U16 u16High;					//垂直方向坐标
}ZS_RESOLUTION;
typedef enum
{
    RESOLUTION_TYPE_DIGITAL = 0, //数字部分原始数据  384*288
    RESOLUTION_TYPE_SIMULATE,    //抓拍 模拟
    RESOLUTION_TYPE_ORIGINAL,    //抓拍 原始数据 对应 抓拍 模拟 360*288
	RESOLUTION_TYPE_END
}RESOLUTION_TYPE;

typedef struct
{
    U8 u8TempEqualFlag;		//等温显示开关
    U8 u8TempSectionFlag;	//分段拉伸开关
    U8 u8SectGray1;			//分段拉伸灰度1
    U8 u8SectGray2;			//分段拉伸灰度2
    U32	u32TempEqColor;		//等温显示色标
    float ftTempEqualMax;	//等温显示上限温度
    float ftTempEqualMin;	//等温显示下限温度
}MT_SPECIAL_PARA;

typedef struct
{
	U32 		u32CtrlFlag;		//控制标志位,MTAPP_OBJ_CONTROL_FLAG
	U32 		u32InstrumentType;	//仪器类型
	U32 		u32InstrumentID;	//仪器序列号
	ZS_RESOLUTION stResolution[RESOLUTION_TYPE_END];	//图像分辨率
	float		ftHistMaxTemper;	//直方图最高温度
	float		ftHistMinTemper;	//直方图最低温度
	float		ftMaxTemper;		//图像最高温度
	float		ftMinTemper;		//图像最低温度
	float		ftAvgTemper;		//图像平均温度
    float		ftReviseTemper;		//修正温度
	U16			u16AgcMode;			//图像AGC模式，自动，手动
	U16			u16SCMode;			//饱和色模式，标准，红蓝，黑白,也即超出色标上下温度范围的着色方式
	S16			s16HistMaxGray;		//直方图最大灰度
	S16			s16HistMinGray;		//直方图最小灰度
	S16			s16MaxGray;			//图像最大灰度
	S16			s16MinGray;			//图像最小灰度
	S16			s16AvgGray;			//图像平均灰度
	U16			u16Reserve;
	ZS_POINT 	stMaxPt;			//图像最高温度像素点的坐标
	ZS_POINT 	stMinPt;			//图像最低温度像素点的坐标
	COLORRULE_TYPE enColorRule;		//色标索引值
	U32			u32OriTransRatex10;	//原始数据帧率X10
	MT_PARAM_INFO stMtPara;			//测温相关信息
	ALARM_INFO stAlarmType;
	float		ftSensorTemper[5];
	float		ftTransCoff;		//透过率
    float		ftAStallMinTemp;	//低温档下限温度
    float		ftAStallMaxTemp;	//低温档上限温度
    float		ftBStallMinTemp;	//高温档下限温度
    float		ftBStallMAxTemp;	//高温档上限温度
    MT_SPECIAL_PARA stMTSpeciaPara;	//等温显示分段拉伸参数
    U8			u8ImageMode;		//图像模式
    U8			u8ImageMirroring;	//图像镜像值
    U8			u8ImageIDE;			//图像IDE
    U8			u8Reserve[1];		//保留
    U32			u32Reserve[17];		//保留
}MT_IMG_INFO;


///////////////////////////////////////新加入的结构体
#define LENS_NUM				4
typedef struct
{
    U8 u8LensNum;							//镜头数量
    U8 u8LensNoCur;							//当前镜头编号
    U8 u8StallNum;							//档位的数量
    U8 u8StallNoCur;						//当前档位

    S16 s16MinTx10[6];                      //最低温度量程
    S16 s16MaxTx10[6];                      //最高温度量程
    char chLensName[LENS_NUM][16];
    U16 u16FocalLength[LENS_NUM];			//焦距
    U16 u16FNum[LENS_NUM];					//镜头F数

}LENS_STALL_INFO;
///////////////////////////////////////


#define MT_APP_RESOLUTION_DIGITAL_WITH	384
#define MT_APP_RESOLUTION_DIGITAL_HIGH	288
#define MT_APP_RESOLUTION_SIMULATE_WITH	720
#define MT_APP_RESOLUTION_SIMULATE_HIGH	576
#define MT_APP_RESOLUTION_ORIGINAL_WITH	360
#define MT_APP_RESOLUTION_ORIGINAL_HIGH	288


typedef enum
{
	MT_OBJ_TYPE_POINT = 0,
	MT_OBJ_TYPE_LINE,
	MT_OBJ_TYPE_RECT,
	MT_OBJ_TYPE_IMG_INFO,
    MT_OBJ_TYPE_MT_INFO,     //镜头，档位，新版本
	MT_OBJ_TYPE_END
}MT_OBJ_TYPE;

/////////////////////////
////////////////////////
/// ////////////////////
typedef struct tag_DSP_CONFIG_PARA_IMAGE
{								//功能名称		范围							默认值
    //COMMAND_SET_DSP_IMAGE_PARA 图像设置
    U32  u32ImageMode;          //图像模式		1HDR 2天地  3普通  4默认		4默认
    U32  u32TDFV;               //时域滤波值		0-10						0
    U32  u32SDFV;               //空域滤波值		0-10						4
    U32  u16IDEEnhance;			//增强			0-10						5
    U32  u32ImagePoloar;		//图像极性		0黑热 1白热					1白热
    U32  u32PseudoCrValue;		//伪彩色值		0黑热 1白热 2铁红 3彩虹 4熔岩 5彩虹增强 6熔融金属 7蓝红 8琥珀色 	1白热
    float ftZoomValue;          //放大倍数		1.0-8.0						1.0
    U16  u16Gain;				//增益			0-254						128
    U16  u16Bright;				//亮度			0-254						128
    U16  u16ImgMir;             //镜像			0不镜像 1x轴镜像 2y轴镜像 3XY轴镜像 	0不镜像
    U16  u16Reserve;
}DSP_CONFIG_PARA_IMAGE;

typedef struct tag_DSP_CONFIG_PARA_FOCUS_LENS
{								//功能名称		范围							默认值
    //COMMAND_SET_DSP_FOCUS_INFO聚焦电机 只设置远焦 近焦 使能
    U16 u16AutoFarLocation;     //远焦位置				10-4000				2000
    U16 u16AutoNearLocation;    //近焦位置				10-4000				2500  远端位置大于近端位置
    U16 u16AutoCurLocation;     //当前的位置				在远端位置和近端位置之间
    U16 u16AFPotEnableF;		//自动聚焦电位计使能   	0关闭 1打开			1打开
    U32 u32AFLength;			//聚焦行程				1-6*25				3*25
    U32 u32FocusLensFinallyDir; //当前镜头方向			0由小到大的方向 1由大到小的方向  2方向不明 刚刚开机镜头没有动过
}DSP_CONFIG_PARA_FOCUS_LENS;


//typedef struct tag_DSP_CONFIG_PARA_FOCUS_LENS
//{								//功能名称		范围							默认值
//    //COMMAND_SET_DSP_FOCUS_INFO聚焦电机 只设置远焦 近焦 使能
//    U16 u16AutoFarLocation;     //远焦位置				10-4000				2000
//    U16 u16AutoNearLocation;    //近焦位置				10-4000				2500  远端位置大于近端位置
//    U16 u16AutoCurLocation;     //当前的位置				在远端位置和近端位置之间
//    U16 u16AFPotEnableF;		//自动聚焦电位计使能   	0关闭 1打开			1打开
//    U32 u32AFLength;			//聚焦行程				1-6*25				3*25
//    U32 u32FocusLensFinallyDir; //当前镜头方向			0由小到大的方向 1由大到小的方向  2方向不明 刚刚开机镜头没有动过
//}DSP_CONFIG_PARA_FOCUS_LENS;


typedef struct tag_DSP_CONFIG_PARA_ZOOM_LENS
{								//功能名称		范围							默认值
    //COMMAND_SET_DSP_ZOOM_INFO变倍电机  只设置长焦 广角 使能
    U16 u16LensFarLoc;          //长焦位置  镜头			10-4000				2000
    U16 u16LensNearLoc;         //广角位置  镜头			10-4000				2500   长焦位置 大于 广角位置
    U16 u16LensCurLoc;          //镜头当前位置			在长焦和广角之间
    U16 u16LFPotEnableF;        //镜头调焦电位计使能		0关闭 1打开			1打开
    U32 u32ZoomLensFinallyDir;	 //当前镜头方向       		0由小到大的方向，1由大到小的方向 2方向不明 刚刚开机镜头没有动过
}DSP_CONFIG_PARA_ZOOM_LENS;

typedef struct tag_DSP_CONFIG_PARA_HT_ALARM
{								//功能名称		范围							默认值
    //COMMAND_SET_FUNCTION_HT_ALARM_PARA 高温报警 已经实现
    U16   u16HighTemperOpenF;   //高温报警是否开启标志	0关闭 1打开			0关闭
    U16   u16HighTemperTraceF;  //高温追踪是否开启标志	0关闭 1打开			0关闭
    float ftHighTemper;         //高温报警温度			0-1000.0			120
    U32   u32HighTemperCoff;    //高温报警系数			1-256				32
}DSP_CONFIG_PARA_HT_ALARM;


typedef struct tag_DSP_CONFIG_PARA_HT_PROTECT
{								//功能名称		范围							默认值
    //COMMAND_SET_DSP_HT_PROTECT   高温保护  只设置u16HTPOpenF  u16ProtectTime
    U16 u16HTPOpenF;			//高温保护				0关闭 1打开			1打开
    //新高温保护参数开始
    U16 u16HTPVskValue;			//高温保护低灵敏度VSK值	10-4000				1026
    U16 u16ContinueTime;        //持续时间 检测高温物体	20-300				100
    U16 u16PauseTime;           //暂停时间 排除误差干扰	10-10*60			5*60
    U16 u16ProtectTime;         //发生高温保护时间		60-60*60			5*60
    U16 u16Reserve;
}DSP_CONFIG_PARA_HT_PROTECT;

typedef struct tag_DSP_CONFIG_PARA_FPA_INFO
{								//功能名称		范围							默认值
    //暂时不提供设置
    U16 u16FidSet;							//FID设置值
    U16 u16VskSet;							//VSK设置值
    U16 u16IntSet;							//lnt设置值
    U16 u16GainSet;							//Gain设置值

    //探测器温度 不提供设置
    float ftSensorTemper[5];

}DSP_CONFIG_PARA_FPA_INFO;


typedef struct tag_DSP_CONFIG_PARA_CROSS_INFO
{								//功能名称		范围							默认值
    //COMMAND_SET_DSP_CROSS_PARA十字光标设置
    U16 u16CrossShow;			//是否显示		0不显示 1显示					开机不显示
    U16 u16X;					//十字光标X		0-u16OutImageWidth			u16OutImageWidth/2
    U16 u16Y;					//十字光标Y		0-u16OutImageHeigth			u16OutImageHeigth/2
    U16 u16SaveFlag;			//是否保存设置值 设置的时候用	0不保存 1保存
}DSP_CONFIG_PARA_CROSS_INFO;
typedef struct tag_DSP_CROSS_SIZE
{
    U16 u16UsrCursorW;			//十字光标宽
    U16 u16UsrCursorH;			//十字光标高
    U16 u16UserCursorSkip;		//十字光标中空
}DSP_CROSS_SIZE;
//DSP配置反馈
typedef struct tag_DSP_CONFIG_PARA
{								//功能名称		范围							默认值
    U32 u32Version;				//版本号			1000开始
    U32 u32Length;				//长度			sizeof(DSP_CONFIG_PARA)
    //COMMAND_SET_DSP_LANGUAGE 	设置语言
    U32 u32Language;			//语言标志      	0中文 1英文					0中文

    //COMMAND_SET_DSP_ACCURACY	设置测温精度
    float ftMTAccV;				//测温精度  		0.1-10.0					0.1

    //COMMAND_GUI_LAYOUT_SET  设置测温界面 已经实现
    U16 u16GuiType;				//界面类型           	1正常   0测温					1正常

    //暂时不提供设置 数据口极性
    U16 u16DataPortPolarity;	//数据口极性	0FALLCLK_FALLFS 1FALLCLK_RISEFS 2RISECLK_FALLFS 3RISECLK_RISEFS 	3RISECLK_RISEFS

    //暂时不提供设置 积分时间
    U16 u16ImgLowSensiInt;      //积分时间      低灵敏度

    U16 u16OutImageWidth;					//输出视频的宽度
    U16 u16OutImageHeigth;					//输出视频的高度

    U16 u16TemperatureUnit;		//温度单位      20190225 修改加入，陈建，温度单位
    U16 u16AFAfterZoom;	//0否  1是   默认1 变倍后是否自动聚焦

    U16 u16CrShowF;				//是否显示彩条

    U8 u8LensZoomEzoom;				//光学变倍和电子变倍联动 0关闭 1打开
    U8 u8Reserve;
    U16 u16UsrCursorW;			//十字光标宽
    U16 u16UsrCursorH;			//十字光标高
    U16 u16UserCursorSkip;		//十字光标中空

    DSP_CONFIG_PARA_IMAGE	stImagePara;

    DSP_CONFIG_PARA_HT_ALARM stHtAlarmPara;

    DSP_CONFIG_PARA_FOCUS_LENS stFocusLensPara;

    DSP_CONFIG_PARA_ZOOM_LENS	stZoomLensPara;

    DSP_CONFIG_PARA_HT_PROTECT stHtProtectPara;

    DSP_CONFIG_PARA_FPA_INFO stFpaInfoPara;

    DSP_CONFIG_PARA_CROSS_INFO stCrossPara;

}DSP_CONFIG_PARA;

typedef struct tag_DSP_CONFIG_PARA_MOTO_PRESET
{								//功能名称		范围							默认值
    U16 u16PresetNum;			//预置点的值 		1-255
    U16 u16PreSetWay;			//预置点种类	 	0变倍预置点 1聚焦预置点 2变倍聚焦预置点 3变倍完成自动聚焦预置点
}DSP_CONFIG_PARA_MOTO_PRESET;
typedef struct
{
    U8 queryType;           //设置的请求类型，0:lrf(激光测距) 1:gps 2:pt (云台角度值) 3:trace(目标跟踪状态)
    U8 state;               //状态值 0:未开始请求          1:已开始请求
    U8 reverse[2];          //保留
}NETSET_QUERY_STATE_t;

typedef struct
{
    S32 s32State;		//测距仪状态   0无法探测到设备，1一个目标，2两个目标，3超程，4，激光正常工作,5无激光
    U32 u32FiltLen;		//选通量 最小测距距离20-5000
    U32 u32Battery;		//店里等级0-9
    float ftLen0;		//目标1距离
    float ftLen1;		//目标2距离，一个目标时  该值无效
}ZS_DIASTIMETER;

typedef struct
{
    int lontiD;         //经度，度
    int lontiM;         //经度，分
    int lontiS;         //经度，秒
    int latiD;          //纬度，度
    int latiM;          //纬度，分
    int latiS;          //纬度，秒
    int isUseLocalGps;  //是否使用本地GPS值
    int longtitude;    //用小数点表示的精度
    int latitude;      //用小数点表示的纬度
}NETSET_GPS_t;

//#include"zsTempData/zs_lk_types.h"

typedef struct
{
    U8	u8Para[4];
    U16 u16Para[4];
    bool bPara[4];
    U32 u32Para[4];
    S32 s32Para[4];
    float ftPara[4];
}ZS_PACKET_CMD_PARA;

typedef enum
{
	EN_AI_TYPE_NONE = 0,        //无
	EN_AI_TYPE_BIRD,            //鸟
	EN_AI_TYPE_DUCK,            //鸭 鹅
	EN_AI_TYPE_HARE,            //兔子
	EN_AI_TYPE_DEER,            //鹿
	EN_AI_TYPE_PIG,             //猪
	EN_AI_TYPE_COW,             //牛
	EN_AI_TYPE_HORSE,           //马
	EN_AI_TYPE_ROO,             //袋鼠
	EN_AI_TYPE_OSTRICH,         //鸵鸟
	EN_AI_TYPE_ELEPHANT,        //大象
	EN_AI_TYPE_WOLF,            //狼
	EN_AI_TYPE_TIGER,           //虎
	EN_AI_TYPE_LION,            //狮
	EN_AI_TYPE_BEAR,            //熊
	EN_AI_TYPE_HUMAN,           //人
	EN_AI_TYPE_ANTELO,          //羚羊 绵羊
	EN_AI_TYPE_CAR,             //汽车
	EN_AI_TYPE_BUS,             //巴士
	EN_AI_TYPE_TRAIN,           //火车
	EN_AI_TYPE_CHAIR,           //椅子
	EN_AI_TYPE_RHINO,           //犀牛
	EN_AI_TYPE_CROCODILE,       //鳄鱼
	EN_AI_TYPE_HIPPO,           //河马
	EN_AI_TYPE_CAMEL,           //骆驼
	EN_AI_TYPE_END,

}EN_AI_TYPE;

#define   MAX_AI_DETECT_NUM  32
#define   MAX_AI_LIST_NUM    4

typedef struct _tagAISave
{
	U16   u16Dist;
	U16   u16AIType;
	float ftScore;
	U32   u32Rev[2];
}AI_OBJ_E;

typedef struct _tagAIList
{
	U32      u32AiObjNum;
	U64      u64Tick;
	U32      u32Rev[5];
	AI_OBJ_E stAiObjElement[MAX_AI_DETECT_NUM];
}AI_OBJ_LIST;
#endif /* ZS_TYPES_H_ */
