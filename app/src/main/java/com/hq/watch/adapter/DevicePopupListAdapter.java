package com.hq.watch.adapter;

import androidx.annotation.NonNull;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.viewholder.BaseViewHolder;
import com.hq.watch.base.bean.DeviceBaseInfo;
import com.hq.watch.R;


public class DevicePopupListAdapter extends BaseQuickAdapter<DeviceBaseInfo, BaseViewHolder> {
    public DevicePopupListAdapter() {
        super(R.layout.item_device_popup);
    }
    @Override
    protected void convert(@NonNull BaseViewHolder helper, DeviceBaseInfo item) {
          helper.setText(R.id.tvItemOne,item.getDevName());
    }
}