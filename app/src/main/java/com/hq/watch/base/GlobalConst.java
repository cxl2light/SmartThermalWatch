package com.hq.watch.base;

/**
 * Created on 2020/5/26
 * author :
 * desc :
 */
public class GlobalConst {
    private GlobalConst() {

    }

    public static final int CONNECT_TIMEOUT = 15000;
    public static final int DATA_TIMEOUT = 10000;

    private static boolean debug = false;

    public static void setDebug(boolean debug) {
        GlobalConst.debug = debug;
    }

    public static boolean isDebug() {
//        return false;
        return debug;
    }
}