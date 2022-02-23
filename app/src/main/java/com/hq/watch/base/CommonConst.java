package com.hq.watch.base;

import android.content.Context;

import androidx.annotation.NonNull;

import com.hq.watch.utils.ScreenUtils;

public class CommonConst {

    public static final float COMMON_WIDGET_WIDTH_PERCENT = 1 / 4f;

    private static int commonWidth = -1;

    public static int getCommonWidgetWidth(@NonNull Context context) {
        if (commonWidth <= 0) {
            commonWidth = (int) (ScreenUtils.getScreenWidthPixels(context) * CommonConst.COMMON_WIDGET_WIDTH_PERCENT);
        }
        return commonWidth;
    }

}
