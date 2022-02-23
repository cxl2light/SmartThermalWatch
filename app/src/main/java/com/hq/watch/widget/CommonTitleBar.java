package com.hq.watch.widget;

import android.app.Activity;
import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import androidx.appcompat.widget.LinearLayoutCompat;
import com.hq.watch.R;

/**
 * @author Administrator
 * @date 2022/2/21 0021 19:49
 */
public class CommonTitleBar extends LinearLayoutCompat implements View.OnClickListener {

    final View backBtn;

    private Context mContext;

    public CommonTitleBar(Context context) {
        this(context, null);
    }

    public CommonTitleBar(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public CommonTitleBar(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.mContext = context;
        setOrientation(VERTICAL);
        LayoutInflater.from(context).inflate(R.layout.layout_tool_bar, this);
        backBtn = findViewById(R.id.img_back);
        backBtn.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.img_back) {
            if (mContext instanceof Activity) {
                ((Activity) mContext).finish();
            }
            return;
        }
    }
}
