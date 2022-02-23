package com.hq.watch.dialog;

import android.content.Context;
import android.content.DialogInterface;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.Gravity;
import android.view.Window;
import android.view.WindowManager;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatDialog;

/**
 * Created on 2018/5/3.
 * author :
 * desc : 公共的基类Dialog
 */
public class BaseDialog extends AppCompatDialog {

    private OnDismissListener mOnDismissListener;
    private float widthPercent = 0.6f;

    public BaseDialog(Context context, int theme) {
        super(context, theme);
    }

    public BaseDialog(Context context, boolean cancelable, OnCancelListener cancelListener) {
        super(context, cancelable, cancelListener);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        super.setOnDismissListener(new OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialog) {
                if (mOnDismissListener != null) {
                    mOnDismissListener.onDismiss(dialog);
                }
            }
        });
    }

    protected void setWidthPercent(float widthPercent) {
        this.widthPercent = widthPercent;
    }

    public boolean defaultParams() {
        return true;
    }

    private void setParams() {
        final Window window = getWindow();
        if (window == null) {
            return;
        }
        final WindowManager.LayoutParams params = window.getAttributes();
        if (widthPercent > 0 && widthPercent <= 1) {
            params.width = (int) (getResources().getDisplayMetrics().widthPixels * widthPercent);
        } else {
            params.width = WindowManager.LayoutParams.WRAP_CONTENT;
        }
        params.height = WindowManager.LayoutParams.WRAP_CONTENT;
        params.gravity = Gravity.CENTER;
        window.setAttributes(params);
    }

    @Override
    public void show() {
        super.show();
        if (defaultParams()) {
            setParams();
        }
    }

    @Override
    public void setOnDismissListener(@Nullable OnDismissListener listener) {
        mOnDismissListener = listener;
    }

    public Resources getResources() {
        return getContext().getResources();
    }
}
