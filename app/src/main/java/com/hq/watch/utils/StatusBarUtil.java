package com.hq.watch.utils;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.graphics.Color;
import android.os.Build;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import androidx.annotation.IntDef;
import androidx.annotation.NonNull;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * Desc:沉浸式状态栏工具类
 * 参考:
 * https://github.com/Tencent/QMUI_Android
 * https://github.com/gyf-dev/ImmersionBar
 * README
 * StatusBarUtil 只向外暴露了三个方法：
 *
 * @see #init(Activity)  ：在基类已经初始化了
 * @see #setStatusBarBlackMode(Activity) :设置当前状态栏黑色图标和字体（默认）
 * @see #setStatusBarWhiteMode(Activity) ：设置当前状态栏白色图标和字体
 * <p>
 * 所以，实际情况下，您只可能会用到最后一个方法
 * 当 Activity 标题栏为非白色时，需要您设置状态栏为白色
 */
@SuppressWarnings("JavaReflectionMemberAccess")
@SuppressLint("ObsoleteSdkInt")
public final class StatusBarUtil {

    private final static int STATUS_BAR_TYPE_DEFAULT = 0;
    private final static int STATUS_BAR_TYPE_MIUI = 1;
    private final static int STATUS_BAR_TYPE_FLYME = 2;
    private final static int STATUS_BAR_TYPE_ANDROID6 = 3; // Android 6.0

    private static @StatusBarType
    int mStatusBarType = STATUS_BAR_TYPE_DEFAULT;


    public static void init(@NonNull Activity activity) {
        init(activity.getWindow());
    }

    private static void init(Window window) {
        if (!supportTranslucent()) {
            return;
        }
        //适配刘海屏
        fitCutoutScreen(window);

        //小米和魅族 4.4 以上版本支持沉浸式
        //小米 Android 6 及以后版本设置黑色字体需要 clear FLAG_TRANSLUCENT_STATUS
        //直接使用官方模式即可
        if (DeviceOSUtil.isMeizu()
                || (DeviceOSUtil.isMIUI() && Build.VERSION.SDK_INT < Build.VERSION_CODES.M)) {
            window.setFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS,
                    WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            return;
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M
                    && supportTranslucentStatusBar()) {
                // Android 6 以后可以改变状态栏字体颜色，因此可以自行设置为透明
                // ZUK Z1 是个另类，自己应用可以实现字体颜色变化，但是没有开放 API
                window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                window.setStatusBarColor(Color.TRANSPARENT);
            } else {
                window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                window.setStatusBarColor(0x40000000);
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            mStatusBarType = STATUS_BAR_TYPE_ANDROID6;
            window.getDecorView().setSystemUiVisibility(window.getDecorView().getSystemUiVisibility() | View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);
        }
    }

    /**
     * 设置状态栏黑色图标
     * 支持 Android 4.4 以上版本 MIUI 和 Flyme，以及 Android 6 以上版本
     */
    public static boolean setStatusBarBlackMode(@NonNull Activity activity) {
        if (DeviceOSUtil.isZTKC2016()) {
            return false;
        }
        if (mStatusBarType != STATUS_BAR_TYPE_DEFAULT) {
            return setStatusBarBlackMode(activity, mStatusBarType);
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            if (isMIUICustomStatusBarLightModeImpl() && setMiuiStatusBarMode(activity.getWindow(), true)) {
                mStatusBarType = STATUS_BAR_TYPE_MIUI;
                return true;
            } else if (setFlymeStatusBarMode(activity.getWindow(), true)) {
                mStatusBarType = STATUS_BAR_TYPE_FLYME;
                return true;
            } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                setAndrod6StatusBarMode(activity.getWindow(), true);
                mStatusBarType = STATUS_BAR_TYPE_ANDROID6;
                return true;
            }
        }
        return false;
    }

    /**
     * 设置状态栏白色图标
     * 支持 Android 4.4 以上版本 MIUI 和 Flyme，以及 Android 6 以上版本
     */
    public static boolean setStatusBarWhiteMode(@NonNull Activity activity) {
        if (mStatusBarType == STATUS_BAR_TYPE_DEFAULT) {
            return true;
        }
        if (mStatusBarType == STATUS_BAR_TYPE_MIUI) {
            return setMiuiStatusBarMode(activity.getWindow(), false);
        } else if (mStatusBarType == STATUS_BAR_TYPE_FLYME) {
            return setFlymeStatusBarMode(activity.getWindow(), false);
        } else if (mStatusBarType == STATUS_BAR_TYPE_ANDROID6) {
            return setAndrod6StatusBarMode(activity.getWindow(), false);
        }
        return true;
    }

    private static boolean setStatusBarBlackMode(Activity activity, @StatusBarType int statusBarType) {
        if (statusBarType == STATUS_BAR_TYPE_MIUI) {
            return setMiuiStatusBarMode(activity.getWindow(), true);
        } else if (statusBarType == STATUS_BAR_TYPE_FLYME) {
            return setFlymeStatusBarMode(activity.getWindow(), true);
        } else if (statusBarType == STATUS_BAR_TYPE_ANDROID6) {
            return setAndrod6StatusBarMode(activity.getWindow(), true);
        }
        return false;
    }

    /**
     * 设置状态栏为黑色，需要 MIUI 6 以上
     *
     * @param window  需要设置的窗口
     * @param isBlack 是否是黑色
     */
    @SuppressLint("PrivateApi")
    private static boolean setMiuiStatusBarMode(Window window, boolean isBlack) {
        boolean result = false;
        if (window != null) {
            final Class clazz = window.getClass();
            try {
                final Class layoutParams = Class.forName("android.view.MiuiWindowManager$LayoutParams");
                final Field field = layoutParams.getField("EXTRA_FLAG_STATUS_BAR_DARK_MODE");
                final int darkModeFlag = field.getInt(layoutParams);
                final Method extraFlagField = clazz.getMethod("setExtraFlags", int.class, int.class);
                if (isBlack) {
                    extraFlagField.invoke(window, darkModeFlag, darkModeFlag);//状态栏透明且黑色字体
                } else {
                    extraFlagField.invoke(window, 0, darkModeFlag);//清除黑色字体
                }
                result = true;
            } catch (Exception ignored) {

            }
        }
        return result;
    }

    /**
     * 设置状态栏图标为深色和魅族特定的文字风格
     */
    private static boolean setFlymeStatusBarMode(Window window, boolean isBlack) {
        boolean result = false;
        if (window != null) {
            // flyme 在 6.2.0.0A 支持了 Android 官方的实现方案，旧的方案失效
            setAndrod6StatusBarMode(window, isBlack);

            try {
                WindowManager.LayoutParams lp = window.getAttributes();
                Field darkFlag = WindowManager.LayoutParams.class
                        .getDeclaredField("MEIZU_FLAG_DARK_STATUS_BAR_ICON");
                Field meizuFlags = WindowManager.LayoutParams.class
                        .getDeclaredField("meizuFlags");
                darkFlag.setAccessible(true);
                meizuFlags.setAccessible(true);
                int bit = darkFlag.getInt(null);
                int value = meizuFlags.getInt(lp);
                if (isBlack) {
                    value |= bit;
                } else {
                    value &= ~bit;
                }
                meizuFlags.setInt(lp, value);
                window.setAttributes(lp);
                result = true;
            } catch (Exception ignored) {

            }
        }
        return result;
    }

    @TargetApi(Build.VERSION_CODES.M)
    private static boolean setAndrod6StatusBarMode(Window window, boolean isBlack) {
        View decorView = window.getDecorView();
        int systemUi = isBlack ? View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR : View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
        systemUi = changeStatusBarModeRetainFlag(window, systemUi);
        decorView.setSystemUiVisibility(systemUi);
        if (DeviceOSUtil.isMIUIV9()) {
            // MIUI 9 低于 6.0 版本依旧只能回退到以前的方案
            // https://github.com/Tencent/QMUI_Android/issues/160
            setMiuiStatusBarMode(window, isBlack);
        }
        return true;
    }

    /**
     * 是否支持 Android 6 的 window.setStatusBarColor(Color.TRANSPARENT)
     */
    private static boolean supportTranslucentStatusBar() {
        return !(DeviceOSUtil.isZUKZ1() || DeviceOSUtil.isZTKC2016());
    }

    /**
     * 是否支持沉浸式状态栏
     */
    private static boolean supportTranslucent() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT
                && !(DeviceOSUtil.isEssential() && Build.VERSION.SDK_INT < 26);
    }

    /**
     * 适配刘海屏
     * https://developer.android.com/guide/topics/display-cutout
     */
    private static void fitCutoutScreen(@NonNull Window window) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            WindowManager.LayoutParams lp = window.getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.
                    LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
    }

    @IntDef({STATUS_BAR_TYPE_DEFAULT, STATUS_BAR_TYPE_MIUI, STATUS_BAR_TYPE_FLYME, STATUS_BAR_TYPE_ANDROID6})
    @Retention(RetentionPolicy.SOURCE)
    private @interface StatusBarType {

    }

    @TargetApi(23)
    private static int changeStatusBarModeRetainFlag(Window window, int out) {
        out = retainSystemUiFlag(window, out, View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
        out = retainSystemUiFlag(window, out, View.SYSTEM_UI_FLAG_FULLSCREEN);
        out = retainSystemUiFlag(window, out, View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        out = retainSystemUiFlag(window, out, View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        out = retainSystemUiFlag(window, out, View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
        out = retainSystemUiFlag(window, out, View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        return out;
    }

    private static int retainSystemUiFlag(Window window, int out, int type) {
        int now = window.getDecorView().getSystemUiVisibility();
        if ((now & type) == type) {
            out |= type;
        }
        return out;
    }

    /**
     * 更改状态栏图标、文字颜色的方案是否是MIUI自家的， MIUI9 && Android 6 之后用回Android原生实现
     * 见小米开发文档说明：https://dev.mi.com/console/doc/detail?pId=1159
     */
    private static boolean isMIUICustomStatusBarLightModeImpl() {
        if (DeviceOSUtil.isMIUIV9() && Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            return true;
        }
        return DeviceOSUtil.isMIUIV5() || DeviceOSUtil.isMIUIV6() ||
                DeviceOSUtil.isMIUIV7() || DeviceOSUtil.isMIUIV8();
    }
}
