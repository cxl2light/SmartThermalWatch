package com.hq.watch.base;

import android.content.Context;
import android.os.Build;
import android.os.Environment;
import android.text.TextUtils;

import java.io.File;

/**
 * Created on 2020/12/20
 * author :
 * desc :全局配置
 */
public class GlobalConfig {
    private static final String DEFAULT_DEVICE_NAME = "defaultDevice";
    public static final String PICTURE_POSTFIX = ".JPEG";
    public static final String VIDEO_POSTFIX = ".mp4";
    private static String preDeviceName;
    private static String preDirPath;

    public static String getRootSavePath(Context context, String deviceName) {
        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.Q)
        {
            deviceName = deviceName == null ? "" : deviceName.trim();
            if (TextUtils.isEmpty(deviceName)) {
                deviceName = DEFAULT_DEVICE_NAME;
            }
            deviceName = deviceName.replace(" ", "_");
            if (deviceName.equals(preDeviceName) && !TextUtils.isEmpty(preDirPath)) {
                return preDirPath;
            }
            preDeviceName = deviceName;
            File dirFile = new File(Environment.getExternalStoragePublicDirectory(""), "Smart_Thermal");
            dirFile.mkdirs();
            dirFile = new File(dirFile, deviceName);
            dirFile.mkdirs();
            preDirPath = dirFile.getAbsolutePath();

        }
        else {
            if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
                    || !Environment.isExternalStorageRemovable()) {
                //外部存储可用
                preDirPath = context.getExternalFilesDir(null).getPath();
            }else {
                //外部存储不可用
                preDirPath = context.getFilesDir().getPath() ;
            }
        }
        return preDirPath;
    }

}
