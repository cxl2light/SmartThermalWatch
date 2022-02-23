package com.hq.watch.net.client;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public class DeviceCtrlClient extends BaseClient{

    public DeviceCtrlClient(String ip) {
        mBaseUrl = BaseClient.getBaseUrl(ip);
    }
}
