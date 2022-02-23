package com.hq.watch.utils;

import android.content.Context;
import android.util.DisplayMetrics;
import android.util.TypedValue;

/**
 * 单位转换的工具类,收集而来
 */
public class DensityUtil {

    /**
     * dp转px
     */
    public static int dpToPx(Context context, float dpVal) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dpVal, getDisplayMetrics(context));
    }

    /**
     * sp转px
     */
    public static int spToPx(Context context, float spVal) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP, spVal, getDisplayMetrics(context));
    }

    /**
     * px转dp
     */
    public static float pxToDp(Context context, float pxVal) {
        final float scale = getDisplayMetrics(context).density;
        return (pxVal / scale);
    }

    /**
     * px转sp
     */
    public static float pxToSp(Context context, float pxVal) {
        return (pxVal / getDisplayMetrics(context).scaledDensity);
    }

    private static DisplayMetrics getDisplayMetrics(Context context) {
        return context.getResources().getDisplayMetrics();
    }

}
