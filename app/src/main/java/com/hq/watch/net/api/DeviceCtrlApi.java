package com.hq.watch.net.api;

import com.hq.watch.base.bean.EmptyResponse;
import com.hq.watch.base.bean.DeviceBaseInfo;
import com.hq.watch.base.bean.DeviceConfig;

import io.reactivex.Observable;
import retrofit2.http.GET;
import retrofit2.http.Query;

/**
 * Created on 2020/11/21
 * author :
 * desc : 设备控制api
 */
public interface DeviceCtrlApi {
    String path_prefix = "protocol.cgi?action=";

    /**
     * 获取配置参数
     */
    @GET(path_prefix + "getConfig")
    Observable<DeviceConfig> getDeviceConfig();

    /**
     * 获取设备信息
     */
    @GET(path_prefix + "getCamera")
    Observable<DeviceBaseInfo> getDeviceInfo();

    /**
     * 设置设备名称
     */
    @GET(path_prefix + "setDevName")
    Observable<EmptyResponse> setDevName(@Query("name") String devName);

    /**
     * 设置调色板
     */
    @GET(path_prefix + "setPalette")
    Observable<EmptyResponse> setPalette(@Query("value") int value);

    /**
     * 设置变倍
     */
    @GET(path_prefix + "setZoom")
    Observable<EmptyResponse> setZoom(@Query("value") int value);

    /**
     * 设置锐度
     */
    @GET(path_prefix + "setSharpness")
    Observable<EmptyResponse> setSharpness(@Query("value") int value);

    /**
     * 设置对比度
     */
    @GET(path_prefix + "setContrast")
    Observable<EmptyResponse> setContrast(@Query("value") int value);

    /**
     * 设置亮度
     */
    @GET(path_prefix + "setBrightness")
    Observable<EmptyResponse> setBrightness(@Query("value") int value);

    /**
     * 设置降噪
     */
    @GET(path_prefix + "setNoiseReduction")
    Observable<EmptyResponse> setNoiseReduction(@Query("value") int value);

    /**
     * 设置分划
     */
    @GET(path_prefix + "setReticle")
    Observable<EmptyResponse> setReticle(@Query("value") int value);

    /**
     * 设置十字光标X坐标
     */
    @GET(path_prefix + "setX")
    Observable<EmptyResponse> setX(@Query("value") int value);

    /**
     * 设置十字光标Y坐标
     */
    @GET(path_prefix + "setY")
    Observable<EmptyResponse> setY(@Query("value") int value);

    /**
     * 设置跟踪使能
     */
    @GET(path_prefix + "setTrack")
    Observable<EmptyResponse> setTrack(@Query("value") int value);

    /**
     * 设置gps使能
     */
    @GET(path_prefix + "setGPS")
    Observable<EmptyResponse> setGPS(@Query("value") int value);

    /**
     * 设置测距使能
     */
    @GET(path_prefix + "setDistanceMeasurement")
    Observable<EmptyResponse> setDistanceMeasurement(@Query("value") int value);

    /**
     * 抓图
     */
    @GET(path_prefix + "snapshot")
    Observable<EmptyResponse> snapshot();

    /**
     * 录像
     */
    @GET(path_prefix + "recording")
    Observable<EmptyResponse> recording();
}
