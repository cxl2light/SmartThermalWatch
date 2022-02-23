package com.hq.watch.net;

import com.hq.watch.net.api.DeviceCtrlApi;
import com.hq.watch.net.api.DeviceFindApi;
import com.hq.watch.net.client.DeviceCtrlClient;
import com.hq.watch.net.client.DeviceFindClient;

/**
 * Created on 2020/11/21
 * author :
 * desc : 网络请求api管理类
 */
public class ApiManager {
    private static volatile DeviceCtrlApi deviceCtrlApi;

    private ApiManager() {

    }

    public static DeviceFindApi getDeviceFindApi(String ip) {
        return DeviceFindClient.create(ip).create(DeviceFindApi.class);
    }

    public static DeviceFindApi getDeviceApis() {
        return DeviceFindClient.creates().create(DeviceFindApi.class);
    }

    public static DeviceCtrlApi getDeviceCtrlApi(String ip) {
        if (deviceCtrlApi == null) {
            synchronized (ApiManager.class) {
                if (deviceCtrlApi == null) {
                    deviceCtrlApi = new DeviceCtrlClient(ip)
                            .getRetrofit().create(DeviceCtrlApi.class);
                }
            }
        }
        return deviceCtrlApi;
    }

    public static void release() {
        deviceCtrlApi = null;
    }

}
