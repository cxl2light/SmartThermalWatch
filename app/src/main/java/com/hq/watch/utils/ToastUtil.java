package com.hq.watch.utils;

import android.content.Context;
import android.text.TextUtils;
import android.widget.Toast;

import androidx.annotation.NonNull;

/**
 * Created on 2020/4/27.
 * author :
 * desc :
 */
public class ToastUtil {
    private static Context appContext;

    private ToastUtil() {

    }

    public static void init(@NonNull Context context) {
        appContext = context.getApplicationContext();
    }

    public static void toast(String text) {
        toast(text, false);
    }

    public static void toast(String text, boolean lengthLong) {
        if (TextUtils.isEmpty(text) || appContext == null) {
            return;
        }
        Toast.makeText(appContext, text, lengthLong ? Toast.LENGTH_LONG : Toast.LENGTH_SHORT).show();
    }
}
