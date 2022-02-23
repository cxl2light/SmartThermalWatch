package com.hq.watch.dialog;

import android.content.Context;
import android.view.View;

import androidx.appcompat.widget.AppCompatTextView;

import com.hq.watch.R;

/**
 * Created on 2020/5/28
 * author :
 * desc :
 */
public class CommonConfirmDialog extends com.hq.watch.dialog.BaseDialog implements View.OnClickListener {

    private AppCompatTextView contentTv;
    private View.OnClickListener confirmListener, cancelListener;

    public CommonConfirmDialog(Context context) {
        this(context, R.style.BaseDialog);
    }

    public CommonConfirmDialog(Context context, int theme) {
        super(context, theme);
        setContentView(R.layout.dialog_common_confirm);
        contentTv = findViewById(R.id.content_tv);
        final View confirmBtn = findViewById(R.id.confirm_button);
        confirmBtn.setOnClickListener(this);
        final View cancelBtn = findViewById(R.id.cancel_button);
        cancelBtn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        dismiss();
        if (v.getId() == R.id.confirm_button) {
            if (confirmListener != null) {
                confirmListener.onClick(v);
            }
            return;
        }
        if (v.getId() == R.id.cancel_button) {
            if (cancelListener != null) {
                cancelListener.onClick(v);
            }
        }
    }

    public void setContent(String content) {
        contentTv.setText(content);
    }

    public void setConfirmListener(View.OnClickListener confirmListener) {
        this.confirmListener = confirmListener;
    }

    public void setCancelListener(View.OnClickListener cancelListener) {
        this.cancelListener = cancelListener;
    }
}
