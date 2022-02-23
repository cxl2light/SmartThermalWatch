package com.hq.watch.utils;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.os.Build;
import android.os.Environment;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Locale;
import java.util.Properties;

/**
 * Desc:手机系统相关工具类
 */
@SuppressLint("PrivateApi")
public final class DeviceOSUtil {

    private static final String TAG = "DeviceOSUtil";

    private static final String KEY_MIUI_VERSION_NAME = "ro.miui.ui.version.name";
    private static final String KEY_FLYME_VERSION_NAME = "ro.build.display.id";
    private static final String KEY_EMUI_VERSION_NAME = "ro.build.version.emui";

    private static String sMiuiVersionName;
    private static String sFlymeVersionName;
    private static String sEMUIVersionName;

    private static final String BRAND = Build.BRAND.toLowerCase();
    private final static String MEIZUBOARD[] = {"m9", "M9", "mx", "MX"};

    static {
        Properties properties = new Properties();
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
            // Android 8 以下先通过拿配置信息获取手机信息
            // Android 8 以上可能会存在权限问题
            FileInputStream inputStream = null;
            try {
                inputStream = new FileInputStream(
                        new File(Environment.getRootDirectory(), "build.prop"));
                properties.load(inputStream);
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (inputStream != null) {
                    try {
                        inputStream.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        sMiuiVersionName = getSystemProperty(properties, KEY_MIUI_VERSION_NAME);
        sFlymeVersionName = getSystemProperty(properties, KEY_FLYME_VERSION_NAME);
        sEMUIVersionName = getSystemProperty(properties, KEY_EMUI_VERSION_NAME);
    }

    private DeviceOSUtil() {

    }

    /**
     * 1.首先通过 build.prop 文件获取
     * 2.获取不到就通过 SystemProperties
     */
    private static String getSystemProperty(Properties properties, String key) {
        String value = properties.getProperty(key);
        if (!TextUtils.isEmpty(value)) {
            return value.toLowerCase();
        }
        try {
            Class<?> clazz = Class.forName("android.os.SystemProperties");
            Method method = clazz.getMethod("get", String.class, String.class);
            return (String) method.invoke(clazz, key, "");
        } catch (Exception e) {
            //ignore
        }
        return "";
    }

    /**
     * 是否是 小米
     */
    public static boolean isXiaoMi() {
        return Build.MANUFACTURER.toLowerCase().equals("xiaomi");
    }

    /**
     * 是否是 Vivo
     */
    public static boolean isVivo() {
        return BRAND.contains("vivo") || BRAND.contains("bbk");
    }

    /**
     * 是否是 Oppo
     */
    public static boolean isOppo() {
        return BRAND.contains("oppo");
    }

    /**
     * 是否是 华为
     */
    public static boolean isHuawei() {
        return BRAND.contains("huawei") || BRAND.contains("honor");
    }

    /**
     * 是否是 魅族
     */
    public static boolean isMeizu() {
        return isMeizuPhone(MEIZUBOARD) || isFlyme();
    }

    /**
     * 国外某种手机
     */
    public static boolean isEssential() {
        return BRAND.contains("essential");
    }

    /**
     * 是否是 MIUI 系统
     * https://dev.mi.com/doc/?p=254
     */
    public static boolean isMIUI() {
        return !TextUtils.isEmpty(sMiuiVersionName);
    }

    public static boolean isMIUIV5() {
        return "v5".equals(sMiuiVersionName);
    }

    public static boolean isMIUIV6() {
        return "v6".equals(sMiuiVersionName);
    }

    public static boolean isMIUIV7() {
        return "v7".equals(sMiuiVersionName);
    }

    public static boolean isMIUIV8() {
        return "v8".equals(sMiuiVersionName);
    }

    public static boolean isMIUIV9() {
        return "v9".equals(sMiuiVersionName);
    }


    /**
     * 是否是 Flyme 系统
     */
    public static boolean isFlyme() {
        return sFlymeVersionName.contains("flyme");
    }

    private static boolean isMeizuPhone(String[] boards) {
        final String board = Build.BOARD;
        if (board == null) {
            return false;
        }
        for (String board1 : boards) {
            if (board.equals(board1)) {
                return true;
            }
        }
        return false;
    }

    //判断是否是 ZUK Z1 和 ZTK C2016
    //两台设备虽然都是 Android 6，但是不支持改变状态栏颜色
    public static boolean isZUKZ1() {
        final String board = Build.MODEL;
        return board != null && board.toLowerCase().contains("zuk z1");
    }

    public static boolean isZTKC2016() {
        final String board = Build.MODEL;
        return board != null && board.toLowerCase().contains("zte c2016");
    }

    public static String getLanguage(Context context) {
        Locale locale = context.getResources().getConfiguration().locale;
        String language = locale.getLanguage();
        Log.d(TAG,language);
        return language;
//        if (language.endsWith("zh"))
//            return true;
//        else
//            return false;
    }

    public static void setLocalEnglish(Context context) {
        String lang = getLanguage(context);
        if (lang.contains("de") || lang.contains("zh")){
            return;
        }
        Resources resources = context.getResources();
        DisplayMetrics dm = resources.getDisplayMetrics();
        Configuration config = resources.getConfiguration();
        // 应用用户选择语言
        config.locale = Locale.ENGLISH;
        resources.updateConfiguration(config, dm);
    }
}
