package com.hq.watch.utils;

import static android.view.View.NO_ID;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.appcompat.widget.LinearLayoutCompat;

/**
 * 获得屏幕相关的辅助类
 * 收集而来
 */
public class ScreenUtils {

    private ScreenUtils() {

    }

    /**
     * 获得屏幕高度的分辨率
     */
    public static int getScreenWidthPixels(@NonNull Context context) {
        final DisplayMetrics outMetrics = context.getResources().getDisplayMetrics();
        return outMetrics.widthPixels;
    }

    /**
     * 获得屏幕高度的分辨率
     */
    public static int getScreenHeightPixels(@NonNull Context context) {
        final DisplayMetrics outMetrics = context.getResources().getDisplayMetrics();
        return outMetrics.heightPixels;
    }

    /**
     * 获得屏幕高度
     */
    public static int getScreenWidth() {
        final DisplayMetrics outMetrics = Resources.getSystem().getDisplayMetrics();
        return (int) (outMetrics.widthPixels / outMetrics.density);
    }

    /**
     * 获得屏幕宽度
     */
    public static int getScreenHeight() {
        final DisplayMetrics outMetrics = Resources.getSystem().getDisplayMetrics();
        return (int) (outMetrics.heightPixels / outMetrics.density);
    }

    private static int STATUS_BAR_HEIGHT = -1;

    /**
     * 获得状态栏的高度
     */
    public static int getStatusHeight() {
        if (STATUS_BAR_HEIGHT > 0) {
            return STATUS_BAR_HEIGHT;
        }
        final Resources resources = Resources.getSystem();
        final int resourceId = resources.getIdentifier("status_bar_height", "dimen", "android");
        STATUS_BAR_HEIGHT = resources.getDimensionPixelSize(resourceId);
        return STATUS_BAR_HEIGHT;
    }

    /**
     * 获取当前屏幕截图，包含状态栏
     */
    public static Bitmap snapShotWithStatusBar(Activity activity) {
        final View view = activity.getWindow().getDecorView();
        view.setDrawingCacheEnabled(true);
        view.buildDrawingCache();
        Bitmap bmp = view.getDrawingCache();
        int width = getScreenWidth();
        int height = getScreenHeight();
        final Bitmap bp = Bitmap.createBitmap(bmp, 0, 0, width, height);
        view.destroyDrawingCache();
        return bp;
    }

    /**
     * 获取当前屏幕截图，不包含状态栏
     */
    public static Bitmap snapShotWithoutStatusBar(Activity activity) {
        final View view = activity.getWindow().getDecorView();
        view.setDrawingCacheEnabled(true);
        view.buildDrawingCache();
        Bitmap bmp = view.getDrawingCache();
        final Rect frame = new Rect();
        activity.getWindow().getDecorView().getWindowVisibleDisplayFrame(frame);
        int statusBarHeight = frame.top;
        int width = getScreenWidth();
        int height = getScreenHeight();
        Bitmap bp = Bitmap.createBitmap(bmp, 0, statusBarHeight, width, height
                - statusBarHeight);
        view.destroyDrawingCache();
        return bp;
    }

    public static int getViewWidth(Context context) {
        int height = ScreenUtils.getScreenHeightPixels(context);
        int width = height * 720 / 576;
        Log.d("ZeLayoutParams","width=" + width + ",height=" + height);
        return width;
    }

    public static LinearLayoutCompat.LayoutParams getVideoLayoutParams(Context context) {
        int height = ScreenUtils.getScreenHeightPixels(context);
        int width = height * 720 / 576;
        Log.d("ZeLayoutParams","width=" + width + ",height=" + height);
        return new LinearLayoutCompat.LayoutParams(width,height);
    }

    public static FrameLayout.LayoutParams getVideoLayoutParamsFrame(Context context) {
        int height = ScreenUtils.getScreenHeightPixels(context);
        int width = height * 720 / 576;
        Log.d("ZeLayoutParams","width=" + width + ",height=" + height);
        return new FrameLayout.LayoutParams(width,height);
    }

    public static ViewGroup.LayoutParams getLayoutParams(Context context) {
        int height = ScreenUtils.getScreenHeightPixels(context);
        int width = height * 720 / 576;
        Log.d("ZeLayoutParams","width=" + width + ",height=" + height);
        return new ViewGroup.LayoutParams(width,height);
    }

    private static final String NAVIGATION= "navigationBarBackground";

    // 该方法需要在View完全被绘制出来之后调用，否则判断不了
    //在比如 onWindowFocusChanged（）方法中可以得到正确的结果
    public static  boolean isNavigationBarExist(@NonNull Activity activity){
        ViewGroup vp = (ViewGroup) activity.getWindow().getDecorView();
        if (vp != null) {
            for (int i = 0; i < vp.getChildCount(); i++) {
                vp.getChildAt(i).getContext().getPackageName();
                if (vp.getChildAt(i).getId()!= NO_ID && NAVIGATION.equals(activity.getResources().getResourceEntryName(vp.getChildAt(i).getId()))) {
                    return true;
                }
            }
        }
        return false;
    }
}
