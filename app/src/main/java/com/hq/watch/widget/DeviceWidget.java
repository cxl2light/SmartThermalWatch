package com.hq.watch.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.appcompat.widget.LinearLayoutCompat;

import com.hq.watch.base.bean.DeviceBaseInfo;
import com.hq.watch.widget.WidgetSelectorBtn;
import com.hq.watch.R;

/**
 * Created on 2020/4/25.
 * author :
 * desc :
 */
public class DeviceWidget extends LinearLayoutCompat implements View.OnClickListener {
    private final WidgetSelectorBtn deviceItemBtn;
    private OnSelectListener mOnSelectListener;
    private DeviceBaseInfo mData;

    public DeviceWidget(Context context) {
        this(context, null);
    }

    public DeviceWidget(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public DeviceWidget(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setOrientation(VERTICAL);
        LayoutInflater.from(context).inflate(R.layout.widget_device_item, this);
        deviceItemBtn = findViewById(R.id.device_item_btn);
        deviceItemBtn.setOnClickListener(this);
    }

    public void setOnSelectListener(OnSelectListener onSelectListener) {
        mOnSelectListener = onSelectListener;
    }

    public void setData(DeviceBaseInfo data) {
        mData = data;
        if (data == null) {
            setVisibility(GONE);
            return;
        }
        deviceItemBtn.setText(data.getDevName());
        deviceItemBtn.setSelected(data.isSelected());
        setVisibility(VISIBLE);
    }

    @Override
    public void onClick(View v) {
        if (mData != null && mOnSelectListener != null && v.getId() == R.id.device_item_btn) {
            mOnSelectListener.onSelect(v, mData);
        }
    }

    public interface OnSelectListener {
        void onSelect(@NonNull View v, @NonNull DeviceBaseInfo data);
    }

}
