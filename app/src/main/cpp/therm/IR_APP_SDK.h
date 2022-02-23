#ifndef IR_APP_SDK_H
#define IR_APP_SDK_H

#include "IR_APP_Define.h"

#ifdef __cplusplus
extern "C"
{
#endif
  /***************************************************************
  *  @brief     新建设备通道
  *  @param     ipv4   IP地址 ;   funGetJsonCamera 回调函数; funGetJsonDevConfig 回调函数; funGetUpgrade 回调函数;funGetJsonAnalyseResult 回调函数;
  *  @return       返回通道号(有效的通道号1~16)
  *  @usage:      
 **************************************************************/
  int IR_APP_NewChannel(char *ipv4, FunGetJsonCamera funGetJsonCamera, FunGetJsonDevConfig funGetJsonDevConfig, FunGetUpgrade funGetUpgrade, FunGetJsonAnalyseResult funGetJsonAnalyseResult);

  /***************************************************************
  *  @brief     删除通道号
  *  @param     channelNo   通道号 ; 
  *  @return       
  *  @usage:      
 **************************************************************/
  void IR_APP_DelChannel(int channelNo);

  /***************************************************************
  *  @brief     判断通道号是否可用
  *  @param     channelNo   通道号 ; 
  *  @return       可用1，不可用0
  *  @usage:      
 **************************************************************/
  int IR_APP_IsChannelAvailable(int channelNo);

  /***************************************************************
  *  @brief     连接相机
  *  @param     channelNo   通道号 ;   msTimeOut 连接超时(毫秒)
  *  @return       返回链接结果，成功1，失败0
  *  @usage:    可能为耗时操作  
 **************************************************************/
  int IR_APP_SynLink(int channelNo, int msTimeOut);

  /***************************************************************
  *  @brief     断开连接
  *  @param     channelNo   通道号 ;
  *  @return       
  *  @usage:      
 **************************************************************/
  void IR_APP_SynDisLink(int channelNo);

  /***************************************************************
  *  @brief     是否链接中
  *  @param     channelNo   通道号 ;
  *  @return    已连接1，未连接0   
  *  @usage:      
 **************************************************************/
  int IR_APP_IsLinked(int channelNo);

  /***************************************************************
  *  @brief     设置调色板
  *  @param     channelNo   通道号 ;   value 调色板索引
  *  @return       成功1，失败0
  *  @usage:    value(0~5)  
 **************************************************************/
  int IR_APP_SET_PALETTE(int channelNo, int value);

  /***************************************************************
  *  @brief     设置变倍
  *  @param     channelNo   通道号 ;   value 变倍值
  *  @return       成功1，失败0
  *  @usage:    value(10~60)  
 **************************************************************/
  int IR_APP_SET_ZOOM(int channelNo, int value);

  /***************************************************************
  *  @brief     请求获取相机信息
  *  @param     channelNo   通道号 
  *  @return       成功1，失败0
  *  @usage:    在回调函数中获取返回json字符串 FunGetJsonCamera
 **************************************************************/
  int IR_APP_GET_CAMERA(int channelNo);

  /***************************************************************
  *  @brief     请求获取相机配置
  *  @param     channelNo   通道号 
  *  @return       成功1，失败0
  *  @usage:    在回调函数中获取返回json字符串 FunGetJsonDevConfig
 **************************************************************/
  int IR_APP_GET_DEVCONFIG(int channelNo);

  /***************************************************************
  *  @brief     请求获取侦测报警
  *  @param     channelNo   通道号
  *  @return       成功1，失败0
  *  @usage:    在回调函数中获取返回json字符串 FunGetJsonAnalyseResult
 **************************************************************/
  int IR_APP_GET_ANALYSERESULT(int channelNo);

  /***************************************************************
  *  @brief     设置锐度
  *  @param     channelNo   通道号 ;   value 锐度值
  *  @return       成功1，失败0
  *  @usage:    value(0~10) 
 **************************************************************/
  int IR_APP_SET_SHARPNESS(int channelNo, int value);

  /***************************************************************
  *  @brief     设置亮度
  *  @param     channelNo   通道号 ;   value 亮度值
  *  @return       成功1，失败0
  *  @usage:    value(1~10) 
 **************************************************************/
  int IR_APP_SET_BRIGHTNESS(int channelNo, int value);

  /***************************************************************
  *  @brief     设置对比度
  *  @param     channelNo   通道号 ;   value 对比度值
  *  @return       成功1，失败0
  *  @usage:    value(1~10) 
 **************************************************************/
  int IR_APP_SET_CONTRAST(int channelNo, int value);

  /***************************************************************
  *  @brief     设置降噪
  *  @param     channelNo   通道号 ;   value 降噪值
  *  @return       成功1，失败0
  *  @usage:    value(0~10) 
 **************************************************************/
  int IR_APP_SET_NOISEREDUCTION(int channelNo, int value);

  /***************************************************************
  *  @brief     设置分化
  *  @param     channelNo   通道号 ;   value 降噪值
  *  @return       成功1，失败0
  *  @usage:    value(0~7) 
 **************************************************************/
  int IR_APP_SET_RETICLE(int channelNo, int value);

  /***************************************************************
  *  @brief     设置坐标X
  *  @param     channelNo   通道号 ;   value =x
  *  @return       成功1，失败0
  *  @usage:    value(-512~512) 
 **************************************************************/
  int IR_APP_SET_XCORDINATE(int channelNo, int value);

  /***************************************************************
  *  @brief     设置坐标Y
  *  @param     channelNo   通道号 ;   value =Y
  *  @return       成功1，失败0
  *  @usage:    value(-384~384) 
 **************************************************************/
  int IR_APP_SET_YCORDINATE(int channelNo, int value);

  /***************************************************************
  *  @brief     使能GPS
  *  @param     channelNo   通道号 ;   value =1 或0
  *  @return       成功1，失败0
  *  @usage:    使能1，关闭0
 **************************************************************/
  int IR_APP_SET_GPSENABLE(int channelNo, int value);

  /***************************************************************
  *  @brief     使能追踪
  *  @param     channelNo   通道号 ;   value =1 或0
  *  @return       成功1，失败0
  *  @usage:    使能1，关闭0
 **************************************************************/
  int IR_APP_SET_TRACEENABLE(int channelNo, int value);

  /***************************************************************
  *  @brief     画中画
  *  @param     channelNo   通道号 ;   value =1 或0
  *  @return       成功1，失败0
  *  @usage:    使能1，关闭0
 **************************************************************/
  int IR_APP_SET_PIP(int channelNo, int value);

  /***************************************************************
  *  @brief     使能测距
  *  @param     channelNo   通道号 ;   value =1 或0
  *  @return       成功1，失败0
  *  @usage:    使能1，关闭0
 **************************************************************/
  int IR_APP_SET_DISTANCEENABLE(int channelNo, int value);

  /***************************************************************
  *  @brief     上传升级文件
  *  @param     channelNo   通道号 ;   filePath 文件路径
  *  @return       成功1，失败0
  *  @usage:    耗时操作,必须在线程中使用该函数,  FunGetUpgradeProgress 回调函数,进度条
 **************************************************************/
  int IR_APP_UPGRADE(int channelNo, char *filePath, FunGetUpgradeProgress funGetUpgradeProgress);

  /***************************************************************
  *  @brief     重启设备
  *  @param     channelNo   通道号 ;
  *  @return       成功1，失败0
  *  @usage:    在回调函数中获取成功标注 FunGetUpgrade
 **************************************************************/
  int IR_APP_REBOOT(int channelNo);

#ifdef __cplusplus
}
#endif

#endif
