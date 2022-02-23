package com.hq.watch.net.client;

import androidx.annotation.NonNull;

import retrofit2.Retrofit;
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * Created on 2020/11/21
 * author :
 * desc : 发现设备
 */
public class DeviceFindClient{

    private static String BASE_URL_ONE = "https://qinkung1.oss-us-west-1.aliyuncs.com";

    private DeviceFindClient() {

    }

    public static Retrofit create(@NonNull String ip) {
        final Retrofit.Builder retrofitBuilder = new Retrofit.Builder()
                .client(BaseClient.getOkHttpClient())
                .baseUrl(BaseClient.getBaseUrl(ip))
                .addConverterFactory(GsonConverterFactory.create())
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create());
        return retrofitBuilder.build();
    }

    public static Retrofit creates() {
        final Retrofit.Builder retrofitBuilder = new Retrofit.Builder()
                .client(BaseClient.getOkHttpClient())
                .baseUrl(BASE_URL_ONE)
                .addConverterFactory(GsonConverterFactory.create())
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create());
        return retrofitBuilder.build();
    }
}
