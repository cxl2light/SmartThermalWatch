package com.hq.watch.device;

import androidx.annotation.NonNull;

import com.hq.watch.base.bean.DeviceBaseInfo;

import java.util.ArrayList;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public interface IDeviceScanView {

    void showDeviceList(@NonNull ArrayList<DeviceBaseInfo> mScanDeviceList);

    void showProgress(int progress);

    void scanFinished();

    void scanTip(String tip);
}
