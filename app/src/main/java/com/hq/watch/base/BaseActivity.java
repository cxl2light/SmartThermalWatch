package com.hq.watch.base;

import android.os.Build;
import android.os.Bundle;
import android.view.WindowManager;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.hq.watch.utils.StatusBarUtil;

/**
 * @author Administrator
 * @date 2022/2/22 0022 09:24
 */
public class BaseActivity extends AppCompatActivity {

    protected AppCompatActivity mActivity;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mActivity = this;
        initBase();
    }

    protected void initBase() {
        // 如果使用沉浸式状态栏
        if (isTranslucentNavigation()) {
            // 透明状态栏
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            StatusBarUtil.init(mActivity);
        }
        if (safeArea()) {
            final WindowManager.LayoutParams lp = getWindow().getAttributes();
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_NEVER;
                getWindow().setAttributes(lp);
            }
        }
    }

    protected boolean safeArea() {
        return false;
    }

    private boolean isTranslucentNavigation() {
        return true;
    }

}
