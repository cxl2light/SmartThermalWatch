package com.hq.watch.net.api;

import com.hq.watch.base.bean.DeviceBaseInfo;

import io.reactivex.Observable;
import okhttp3.ResponseBody;
import retrofit2.http.GET;

/**
 * Created on 2020/11/21
 * author :
 * desc : 设备控制api
 */
public interface DeviceFindApi {
    /**
     * 获取配置参数
     */
    @GET("protocol.cgi?action=getCamera")
    Observable<DeviceBaseInfo> getDeviceInfo();

    /**
     * https://qinkung1.oss-us-west-1.aliyuncs.com/telescope.ini
     * 检查版本更新
     * @return
     */
    @GET("https://qinkung1.oss-us-west-1.aliyuncs.com/telescope.ini")
    Observable<ResponseBody> checkVersion();

}
