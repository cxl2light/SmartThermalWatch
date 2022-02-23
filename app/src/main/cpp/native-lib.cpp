#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_hq_monitor_device_ConnectDeviceActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++链接";
    return env->NewStringUTF(hello.c_str());
}