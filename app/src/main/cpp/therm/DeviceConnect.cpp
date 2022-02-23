//
// Created by Administrator on 2021/9/3.
//
#include <jni.h>
#include <string>

// 引入log头文件
#include  <android/log.h>
#include <cstring>
#include "IR_APP_SDK.h"
#include "zs_Types.h"
#include "cJSON.h"

// log标签
#define  TAG    "这里填写日志的TAG"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

//#include "DeviceConnect.h"
//https://www.jianshu.com/p/e576c7e1c403

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initDevice(JNIEnv *env, jobject jobj, jint n) {
    jint sum = 0;
//    char ipv4[32] = "192.168.0.1";
    char ipv4[32] = "192.168.0.1606";

    int nChannelNo = IR_APP_NewChannel(ipv4,0,0,0,0);  //1.创建通道
    printf("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);

    int retLink = IR_APP_SynLink(nChannelNo,3000);  //2.连接设备
    printf("ZeOne=retLink = %d\n\n", retLink);
    LOGI("ZeOne=retLink = %d\n\n", retLink);

    static int rulerIndex = 0;
    int result = IR_APP_SET_PALETTE(nChannelNo,rulerIndex); //3.发送命令   //返回 0，发送命令失败； 成功 1
    LOGI("ZeOne=command result= %d\n\n", result);
    ++rulerIndex;
    LOGI("ZeOne=command rulerIndex= %d\n\n", rulerIndex);

    printf("dis link\n\n");
    LOGI("ZeOne=dis link\n\n");

//    IR_APP_SynDisLink(nChannelNo);

//    IR_APP_DelChannel(nChannelNo);
    return nChannelNo;
}

jstring charTojstring(JNIEnv* env, char* pat) {
    //定义java String类 strClass
    jclass strClass = (env)->FindClass("java/lang/String");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (env)->NewStringUTF("GB2312");
    //将byte数组转换为java String,并输出
    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}

void CallbackGetJsonCamera(int channelNo, char *pJson)
{
    printf("GetJsonCamera:  %d-%s\n", channelNo, pJson);
    LOGI("ZeOne=GetJsonCamera:  %d-%s\n", channelNo, pJson);
}

void CallbackGetJsonDevConfig2(int channelNo, char *pJson)
{
    printf("GetJsonDevConfig: %d-%s\n", channelNo, pJson);
    LOGI("ZeOne=GetJsonDevConfig: %d-%s\n", channelNo, pJson);
}

//nUpgrade=0 成功;  非0 失败
void CallbackGetUpgrade2(int channelNo, int nUpgrade)
{
    printf("GetUpgrade:  code: %d\n", nUpgrade);
    LOGI("ZeOne=GetUpgrade:  code: %d\n", nUpgrade);
}


int CallbackGetUpgradeProgress2(int channelNo,float nProgress, int endFlag)
{
    printf("GetUpgradeProgress:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);
    LOGI("ZeOne=GetUpgradeProgress:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);

    int stopFlag = 1;     //停止传输
    stopFlag = 0;         //继续传输

    return stopFlag;
}

int nChannelNo = 0;
JavaVM *g_VM;

jobject g_obj;
void CallbackGetJsonDevConfig(int channelNo, char *pJson)
{
    printf("GetJsonDevConfig: %d-%s\n", channelNo, pJson);
    LOGI("ZeJson=GetJsonDevConfig: %d-%s\n", channelNo, pJson);

    int mNeedDetach;
    JNIEnv *env;
    //获取当前native线程是否有没有被附加到jvm环境中
    int getEnvStat = g_VM->GetEnv((void **)   &env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if (g_VM->AttachCurrentThread(&env, NULL) != 0) {
            LOGI("ZeOne=AttachCurrentThread: %d-%s\n", channelNo, pJson);
            return;
        }
        mNeedDetach = JNI_TRUE;
    }

    jclass clazz = env->GetObjectClass(g_obj);  //注意是用传过来的对象，不是 jobj
    if(clazz == NULL) {
        LOGI("ZeOne=GetObjectClass: %d-%s\n", channelNo, pJson);
        return;
    }
    //获取方法 id
    jmethodID javaCallbackId = env->GetMethodID(clazz, "onDeviceConfig", "(ILjava/lang/String;)V");
    if(javaCallbackId == NULL) {
        LOGI("ZeOne=GetMethodID: %d-%s\n", channelNo, pJson);
        return;
    }
    //执行回调
    env->CallVoidMethod(g_obj, javaCallbackId,channelNo,charTojstring(env,pJson));

    //释放当前线程
    if(mNeedDetach) {
        g_VM->DetachCurrentThread();
    }
    env = NULL;
}

jobject g_obj_ai;
void CallbackGetJsonAnalyseResult(int channelNo, char *pJson)
{

    AI_OBJ_LIST* pList = (AI_OBJ_LIST*)pJson;
    cJSON* root = cJSON_CreateArray();
    int num = pList->u32AiObjNum;

    LOGI("ZeJson=CallbackGetJsonAnalyseResult: %d-%d-%d\n", channelNo,num,pList->stAiObjElement->u16Dist);

    for (int i = 0; i < num; i++) {
        cJSON* obj_list = cJSON_CreateObject();
        cJSON* item = cJSON_CreateInt((uint64)pList->stAiObjElement[i].u16Dist, 1);
        cJSON_AddItemToObject(obj_list, "u16Dist", item);

        item = cJSON_CreateInt((uint64)pList->stAiObjElement[i].u16AIType, 1);
        cJSON_AddItemToObject(obj_list, "u16AIType", item);

        item = cJSON_CreateDouble((double)pList->stAiObjElement[i].ftScore, 1);
        cJSON_AddItemToObject(obj_list, "ftScore", item);

        item = cJSON_CreateInt((uint64)pList->stAiObjElement[i].u32Rev, 1);
        cJSON_AddItemToObject(obj_list, "u32Rev", item);

        cJSON_AddItemToObject(root, "AI_OBJ_LIST", obj_list);

    }

    char* out = NULL;
    out = cJSON_Print(root);

    LOGI("ZeJson=CallbackGetJsonAnalyseResult: %d-%s\n", num, out);

    int mNeedDetach;
    JNIEnv *env;
    //获取当前native线程是否有没有被附加到jvm环境中
    int getEnvStat = g_VM->GetEnv((void **)   &env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if (g_VM->AttachCurrentThread(&env, NULL) != 0) {
            LOGI("ZeOne=AttachCurrentThread: %d-%s\n", channelNo, out);
            return;
        }
        mNeedDetach = JNI_TRUE;
    }

    jclass clazz = env->GetObjectClass(g_obj_ai);  //注意是用传过来的对象，不是 jobj
    if(clazz == NULL) {
        LOGI("ZeOne=GetObjectClass: %d-%s\n", channelNo, out);
        return;
    }
    //获取方法 id
    jmethodID javaCallbackId = env->GetMethodID(clazz, "onAnalyseResult", "(ILjava/lang/String;)V");
    if(javaCallbackId == NULL) {
        LOGI("ZeOne=GetMethodID: %d-%s\n", channelNo, out);
        return;
    }

    //执行回调
    env->CallVoidMethod(g_obj_ai, javaCallbackId,channelNo,charTojstring(env,out));

    //释放当前线程
    if(mNeedDetach) {
        g_VM->DetachCurrentThread();
    }
    env = NULL;
}

jobject g_obj_upgrade;
void CallbackGetUpgrade(int channelNo, int nUpgrade)
{
    printf("GetUpgrade: %d-%d\n", channelNo, nUpgrade);
    LOGI("ZeOne=UpgradeStatus: %d-%d\n", channelNo, nUpgrade);

    int mNeedDetach;
    JNIEnv *env;
    //获取当前native线程是否有没有被附加到jvm环境中
    int getEnvStat = g_VM->GetEnv((void **)   &env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if (g_VM->AttachCurrentThread(&env, NULL) != 0) {
            LOGI("ZeOne=AttachCurrentThread: %d-%d\n", channelNo, nUpgrade);
            return;
        }
        mNeedDetach = JNI_TRUE;
    }

    jclass clazz = env->GetObjectClass(g_obj_upgrade);  //注意是用传过来的对象，不是 jobj
    if(clazz == NULL) {
        LOGI("ZeOne=GetObjectClass: %d-%d\n", channelNo, nUpgrade);
        return;
    }
    //获取方法 id
    jmethodID javaCallbackId = env->GetMethodID(clazz, "onUpgrade", "(II)V");
    if(javaCallbackId == NULL) {
        LOGI("ZeOne=GetMethodID: %d-%d\n", channelNo, nUpgrade);
        return;
    }
    //执行回调
    env->CallVoidMethod(g_obj_upgrade, javaCallbackId,channelNo,nUpgrade);

    //释放当前线程
//    if(mNeedDetach) {
//        g_VM->DetachCurrentThread();
//    }
//    env = NULL;
}

jobject g_obj_upgrade_progress;
int CallbackGetUpgradeProgress(int channelNo,float nProgress, int endFlag)
{
    printf("GetUpgradeProgress:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);
    LOGI("ZeOne=GetUpgradeProgress1001:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);

    int stopFlag = 1;     //停止传输
    stopFlag = 0;         //继续传输

    int mNeedDetach;
    JNIEnv *env;
    //获取当前native线程是否有没有被附加到jvm环境中
    int getEnvStat = g_VM->GetEnv((void **)   &env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if (g_VM->AttachCurrentThread(&env, NULL) != 0) {
            LOGI("ZeOne=GetUpgradeProgress1002:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);
            stopFlag = 1;
            return stopFlag;
        }
        mNeedDetach = JNI_TRUE;
    }
    jclass clazz = env->GetObjectClass(g_obj_upgrade_progress);  //注意是用传过来的对象，不是 jobj
    if(clazz == NULL) {
        LOGI("ZeOne=GetUpgradeProgress1003:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);
        stopFlag = 1;
        return stopFlag;
    }
    //获取方法 id
    jmethodID javaCallbackId = env->GetMethodID(clazz, "onUpgradeFile", "(IFI)V");
    if(javaCallbackId == NULL) {
        LOGI("ZeOne=GetUpgradeProgress1004:  nProgress: %.2f, endFlag:%d\n", nProgress,  endFlag);
        stopFlag = 1;
        return stopFlag;
    }
    if (g_obj_upgrade_progress == NULL){
        LOGI("ZeOne=GetUpgradeProgress3003:  nProgress: %.2f, endFlag:%d\n",nProgress,  endFlag);
    }
    //执行回调
    env->CallVoidMethod(g_obj_upgrade_progress, javaCallbackId,channelNo,nProgress,endFlag);
    //释放当前线程
//    if(mNeedDetach) {
//        g_VM->DetachCurrentThread();
//    }
//    env = NULL;
    return stopFlag;
}

/**
 * 初始化，0，判断已知通道号是否有效
 *        1.如果无效，创建通道 ，连接设备
 *        2.如果有效，则无需创建和连接
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_hq_monitor_jni_ParamsJni_initDeviceChannel(JNIEnv *env, jobject jobj, jstring ip,jobject listenerConfig,jobject listenerUpgrade,jobject listenerAiObjInfo) {
    jint sum = 0;
    char ipv4[32] = "192.168.0.999";
    //utf-8 字符
    const char *str = env->GetStringUTFChars(ip, JNI_FALSE);
    LOGI("ZeOne=mIp1 = %s,%d\n\n", ipv4,sizeof(str));
    strncpy(ipv4,str,32);
    LOGI("ZeOne=mIp2 = %s\n\n", ipv4);
    int isAvailable = IR_APP_IsChannelAvailable(nChannelNo); //可用1，不可用0
    int isLinked = IR_APP_IsLinked(nChannelNo); //已连接1，未连接0
    LOGI("ZeOne= nChannelNo = %d, isAvailable = %d,isLinked = %d\n\n", nChannelNo,isAvailable,isLinked);
    int retLink = 0;
    if (isAvailable != 1 || isLinked != 1){
        //JavaVM是虚拟机在JNI中的表示，等下再其他线程回调java层需要用到
        env->GetJavaVM(&g_VM);
        //生成一个全局引用，回调的时候findclass才不会为null
        g_obj = env->NewGlobalRef(listenerConfig);
        g_obj_ai = env->NewGlobalRef(listenerAiObjInfo);
        g_obj_upgrade = env->NewGlobalRef(listenerUpgrade);
        // 生成一个全局引用保留下来，以便回调
//        jobject g_obj = env->NewGlobalRef(jobj);

        nChannelNo = IR_APP_NewChannel(ipv4, CallbackGetJsonCamera, CallbackGetJsonDevConfig , CallbackGetUpgrade, CallbackGetJsonAnalyseResult);  //1.创建通道
        printf("ZeOne=nChannelNo = %d\n\n", nChannelNo);
        LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);

        retLink = IR_APP_SynLink(nChannelNo, 3000);  //2.连接设备
        printf("ZeOne=retLink = %d\n\n", retLink);
        LOGI("ZeOne=retLink = %d\n\n", retLink);
    }

//    return (jstring) (retLink);
    return ;
}

/***************************************************************
*  @brief     上传升级文件
*  @param     channelNo   通道号 ;   filePath 文件路径
*  @return       成功1，失败0
*  @usage:    耗时操作,必须在线程中使用该函数,  FunGetUpgradeProgress 回调函数,进度条
**************************************************************/
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initUpgradeFile(JNIEnv *env, jobject jobj,jstring filePath,jobject listenerUpgradeFile) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    char mPath[512] = "E:\\book\\update\\a.jpg";
    //utf-8 字符
    const char *str = env->GetStringUTFChars(filePath, JNI_FALSE);
    LOGI("ZeOne=mPath1 = %s,%d\n\n", mPath,sizeof(str));
    strncpy(mPath,str,512);
    LOGI("ZeOne=mPath2 = %s\n\n", mPath);

    //JavaVM是虚拟机在JNI中的表示，等下再其他线程回调java层需要用到
    env->GetJavaVM(&g_VM);
    //生成一个全局引用，回调的时候findclass才不会为null
    g_obj_upgrade_progress = env->NewGlobalRef(listenerUpgradeFile);
    int result = IR_APP_UPGRADE(nChannelNo,mPath,CallbackGetUpgradeProgress); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}


extern "C" JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initUpgradeFile2(JNIEnv *env, jobject instance,jstring source_str,jstring dest_str) {
    const char* sourcestr = env->GetStringUTFChars(source_str,JNI_FALSE);
    const char* deststr = env->GetStringUTFChars(dest_str,JNI_FALSE);
    FILE * fp;
    FILE * fd;
    char buf[1024 * 10];
    fp=fopen(sourcestr,"rw");
    if(fp==NULL) {
        LOGI("ZeOne=mFile , open source filed");
    }
    fd=fopen(deststr,"w");//当文件不存在时，自动创建该文件
    if(fd==NULL) {
        LOGI("ZeOne=mFile , open dest filed");
    }
    LOGI("ZeOne=mFile , Copy Start");
    while(fgets(buf, sizeof(buf),fp)!=NULL){
        fputs(buf,fd);
    }
    LOGI("ZeOne=mFile , Copy Done");
    if(fp!=NULL){
        fclose(fp);
    }
    if(fd!=NULL){
        fclose(fd);
    }
    env->ReleaseStringUTFChars(source_str,sourcestr);
    env->ReleaseStringUTFChars(dest_str,deststr);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initGetDeviceConfig(JNIEnv *env, jobject jobj) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_GET_DEVCONFIG(nChannelNo); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initGetAnalyseResult(JNIEnv *env, jobject jobj) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_GET_ANALYSERESULT(nChannelNo); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

/**
 *  重启设备
 *  成功1，失败0
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initReboot(JNIEnv *env, jobject jobj) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_REBOOT(nChannelNo); //3.发送命令   // 成功1，失败0
    return result;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initDisLink(JNIEnv *env, jobject jobj) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    IR_APP_SynDisLink(nChannelNo); //3.发送命令
    return 1;
}

//调用 Java 公有方法
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_CallbackGetJsonCamera2(JNIEnv *env, jobject jobj,int channelNo,jstring pJson) {
    //1.获取对应 class 的实体类
    jclass jclazz = env->GetObjectClass(jobj);
    //2.获取方法的 id
    jmethodID mid = env->GetMethodID(jclazz, "setDeviceConfig", "(Ljava/lang/String;)V");
    //3.字符数组转换为字符串
    char c[10] = "male";
    jstring jsex = env->NewStringUTF(c);
    //4.通过该 class 调用对应的 public 方法
    env->CallVoidMethod(jobj, mid, jsex);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetX(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_XCORDINATE(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetY(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_YCORDINATE(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

/**
 * 设置对比度
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetContrast(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_CONTRAST(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

/**
 * 设置对比度
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetBrightness(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_BRIGHTNESS(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

/**
 * 设置降噪
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetNoiseReduction(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_NOISEREDUCTION(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

/**
 * 设置降噪
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetReticle(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_RETICLE(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetZoom(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_ZOOM(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

/**
 * 设置锐度
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetSharpness(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_SHARPNESS(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

/**
 * 设置锐度
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetPalette(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_PALETTE(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetDistanceMeasurement(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_DISTANCEENABLE(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetTrack(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_TRACEENABLE(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetPip(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_PIP(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hq_monitor_jni_ParamsJni_initSetGPS(JNIEnv *env, jobject jobj, jint n) {
    LOGI("ZeOne=nChannelNo = %d\n\n", nChannelNo);
    int result = IR_APP_SET_GPSENABLE(nChannelNo,n); //3.发送命令   //返回 0，发送命令失败； 成功 1
    return result;
}


