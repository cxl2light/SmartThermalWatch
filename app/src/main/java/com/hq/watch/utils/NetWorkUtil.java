package com.hq.watch.utils;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.wifi.WifiManager;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.hq.watch.base.GlobalConst;
import com.hq.watch.BuildConfig;

import java.io.BufferedReader;
import java.io.FileReader;
import java.lang.reflect.Method;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public class NetWorkUtil {
    private static final String TAG = "NetWorkUtil";

    /**
     * 获取WiFi是否可用
     */
    public static boolean isWifiEnabled(@NonNull Context context) {
        final Context appContext = context.getApplicationContext();
        final WifiManager wifiMgr = (WifiManager) appContext.getSystemService(Context.WIFI_SERVICE);
        if (wifiMgr == null || !wifiMgr.isWifiEnabled()) {
            return false;
        }
        if (wifiMgr.getWifiState() == WifiManager.WIFI_STATE_ENABLED) {
            final ConnectivityManager connManager = (ConnectivityManager) appContext.getSystemService(Context.CONNECTIVITY_SERVICE);
            final Network network = connManager.getActiveNetwork();
            return network != null;
        }
        return false;
    }

    /**
     * 获取当前设备连接WiFi后的ip地址
     */
    @Nullable
    public static String getWifiIp(@NonNull Context context) {
        final Context appContext = context.getApplicationContext();
        if (!isWifiEnabled(appContext)) {
            return null;
        }
        final WifiManager wifiMgr = (WifiManager) appContext.getSystemService(Context.WIFI_SERVICE);
        int ipAsInt = wifiMgr.getConnectionInfo().getIpAddress();
        if (ipAsInt == 0) {
            return null;
        } else {
            return int2ip(ipAsInt);
        }
    }

    /**
     * 将ip的整数形式转换成ip形式
     */
    private static String int2ip(int ipInt) {
        final StringBuilder sb = new StringBuilder(20);
        sb.append(ipInt & 0xFF).append(".");
        sb.append((ipInt >> 8) & 0xFF).append(".");
        sb.append((ipInt >> 16) & 0xFF).append(".");
        sb.append((ipInt >> 24) & 0xFF);
        return sb.toString();
    }

    /**
     * 判断热点是否开启
     */
    public static boolean isApOn(Context context) {
        try {
            final WifiManager wifiManager = (WifiManager) context
                    .getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            final Method method = wifiManager.getClass().getDeclaredMethod("isWifiApEnabled");
            method.setAccessible(true);
            return (boolean) method.invoke(wifiManager);
        } catch (Throwable ignored) {
        }
        return false;
    }


    /**
     * 获取连接到本机AP热点设备的IP地址
     */
    public static ArrayList<String> getApConnectIp() throws Exception {
        final ArrayList<String> connectIpList = new ArrayList<>();
        final BufferedReader br = new BufferedReader(new FileReader("/proc/net/arp"));
        String line;
        while ((line = br.readLine()) != null) {
            if (GlobalConst.isDebug()) {
                Log.d(TAG, line);
            }
            if (line.startsWith("IP") || line.startsWith("ip")) {
                continue;
            }
            final String[] split = line.split(" +");
            if (split.length >= 4) {
                connectIpList.add(split[0]);
            }
        }
        return connectIpList;
    }

    public static String getApHostIp() {
        String hostIp = null;
        try {
            final Enumeration<NetworkInterface> networkInterface = NetworkInterface.getNetworkInterfaces();
            for (; networkInterface.hasMoreElements(); ) {
                final NetworkInterface singleInterface = networkInterface.nextElement();
                for (Enumeration<InetAddress> IpAddresses = singleInterface.getInetAddresses(); IpAddresses.hasMoreElements(); ) {
                    final InetAddress inetAddress = IpAddresses.nextElement();
                    if (inetAddress.isLoopbackAddress() || inetAddress instanceof Inet6Address) {
                        continue;
                    }
                    final String displayName = singleInterface.getDisplayName();
//                    Log.d(TAG, "网络接口名称：" + displayName);
//                    Log.d(TAG, "Ip地址：" + inetAddress.getHostAddress());
                    if (BuildConfig.DEBUG) {
                        Log.d(TAG, "网络接口名称：" + displayName);
                        Log.d(TAG, "Ip地址：" + inetAddress.getHostAddress());
                    }

                    //小米手机 11，网络接口名称：wlan1
                    if (displayName.contains("wlan1") || displayName.contains("wlan0") || displayName.contains("eth0") || displayName.contains("ap0")) {
                        hostIp = inetAddress.getHostAddress();
                        break;
                    }
                }
                if (!TextUtils.isEmpty(hostIp)) {
                    break;
                }
            }
        } catch (SocketException ex) {
            Log.e(TAG, ex.toString());
        }
        return hostIp;
    }
}
