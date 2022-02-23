package com.hq.watch;

import android.app.Application;
import android.content.Context;

import com.hq.watch.base.GlobalConst;
import com.hq.watch.utils.ToastUtil;

/**
 * Created on 2020/4/4.
 * author :
 * desc :
 */
public class MyApp extends Application {
    private static Context appContext;

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
    }

    @Override
    public void onCreate() {
        super.onCreate();

        ToastUtil.init(this);
        GlobalConst.setDebug(BuildConfig.DEBUG);
        appContext = getApplicationContext();
    }

    public static Context getAppContext() {
        return appContext;
    }

}
