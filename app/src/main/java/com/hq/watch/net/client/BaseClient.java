package com.hq.watch.net.client;

import android.text.TextUtils;

import androidx.annotation.NonNull;


import com.hq.watch.net.converter.EmptyResponseConverter;

import java.util.concurrent.TimeUnit;

import okhttp3.OkHttpClient;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Retrofit;
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public abstract class BaseClient {
    // 所有 OkHttp 对象共用一个连接池。
    protected static final int TIME_OUT = 3000;
    private static final String BASE_URL_PLACE_HOLDER = "http://ip/cgi-bin/";

    protected Retrofit mRetrofit = null;
    protected String mBaseUrl;

    public Retrofit getRetrofit() {
        if (mRetrofit == null) {
            final Retrofit.Builder retrofitBuilder = new Retrofit.Builder()
                    .client(getOkHttpClient());
            retrofitBuilder
                    .baseUrl(mBaseUrl)
                    .addConverterFactory(EmptyResponseConverter.create())
                    .addConverterFactory(GsonConverterFactory.create())
                    .addCallAdapterFactory(RxJava2CallAdapterFactory.create());
            mRetrofit = retrofitBuilder.build();
        }
        return mRetrofit;
    }

    private static OkHttpClient sOkHttpClient = null;

    @NonNull
    protected static OkHttpClient getOkHttpClient() {
        if (sOkHttpClient == null) {
            synchronized (BaseClient.class) {
                if (sOkHttpClient == null) {
                    final OkHttpClient.Builder okHttpClientBuilder = new OkHttpClient.Builder();
                    okHttpClientBuilder.interceptors().clear();
                    //设置请求超时时间
                    okHttpClientBuilder
                            .connectTimeout(TIME_OUT, TimeUnit.MILLISECONDS)
                            .readTimeout(TIME_OUT, TimeUnit.MILLISECONDS)
                            .writeTimeout(TIME_OUT, TimeUnit.MILLISECONDS);

                    HttpLoggingInterceptor interceptor = new HttpLoggingInterceptor();
                    interceptor.setLevel(HttpLoggingInterceptor.Level.BODY);
                    okHttpClientBuilder.addInterceptor(interceptor);

                    sOkHttpClient = okHttpClientBuilder.build();
                }
            }
        }
        return sOkHttpClient;
    }

    public static String getBaseUrl(String ip) {
        if (TextUtils.isEmpty(ip)) {
            ip = "127.0.0.1";
        }
        return BASE_URL_PLACE_HOLDER.replace("ip", ip);
    }

}
