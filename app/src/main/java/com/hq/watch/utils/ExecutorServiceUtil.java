package com.hq.watch.utils;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import okhttp3.internal.Util;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public class ExecutorServiceUtil {

    private static volatile ExecutorService mExecutorService;

    public static ExecutorService getExecutorService() {
        if (mExecutorService == null) {
            synchronized (ExecutorServiceUtil.class) {
                if (mExecutorService  == null) {
                    mExecutorService = new ThreadPoolExecutor(2,
                            Runtime.getRuntime().availableProcessors() * 2,
                            30, TimeUnit.SECONDS,new SynchronousQueue<>(),
                            Util.threadFactory("Monitor Executor", false));
                }
            }
        }
        return mExecutorService;
    }

}
