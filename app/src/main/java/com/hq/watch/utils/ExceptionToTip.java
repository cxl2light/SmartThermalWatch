package com.hq.watch.utils;

import androidx.annotation.NonNull;

import java.net.ConnectException;
import java.util.concurrent.TimeoutException;

/**
 * Created on 2020/5/31
 * author :
 * desc : 将异常转换为一个提示
 */
public class ExceptionToTip {

    public static String toTip(@NonNull Throwable e) {
        if (e instanceof ConnectException) {
            return "连接到设备异常";
        }
        if (e instanceof TimeoutException) {
            return "连接超时，请稍后重试";
        }
        return e.getMessage();
    }

}
