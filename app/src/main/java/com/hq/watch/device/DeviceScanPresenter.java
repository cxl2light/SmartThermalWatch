package com.hq.watch.device;

import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.MainThread;
import androidx.annotation.NonNull;

import com.hq.watch.MyApp;
import com.hq.watch.utils.ToastUtil;
import com.hq.watch.base.bean.DeviceBaseInfo;
import com.hq.watch.utils.ExecutorServiceUtil;
import com.hq.watch.R;
import com.hq.watch.net.ApiManager;
import com.hq.watch.utils.NetWorkUtil;
import com.hq.watch.utils.RxUtil;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

import io.reactivex.disposables.CompositeDisposable;
import io.reactivex.disposables.Disposable;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public class DeviceScanPresenter {
    private static final List<String> BLACK_IP_LIST = new ArrayList<>(10);
    private final static String TAG = "DeviceScanPresenter";
    private final CompositeDisposable mDisposableSet = new CompositeDisposable();
    private final ArrayList<DeviceBaseInfo> mScanDeviceList = new ArrayList<>(10);
    private final ReentrantLock pubLock = new ReentrantLock();
    private final Condition finished = pubLock.newCondition();
    private final LinkedBlockingQueue<String> mIpBucket;
    private final int bufferSize;

    private final IDeviceScanView mView;
    private final int startIp = 1;
    private final int endIp = 255;
    private int finishedSize = 0;
    private int mTotalScanIp = 0;

    private volatile boolean mDestroy = false;

    static {
        BLACK_IP_LIST.add("0.0.0.0");
    }

    public DeviceScanPresenter(@NonNull IDeviceScanView deviceScanView) {
        mView = deviceScanView;
        /*final int availableProcessors = Runtime.getRuntime().availableProcessors();
        bufferSize = availableProcessors * 5;*/
        bufferSize = 255;
        // 因为这里主要是网络io操作，所以多开几个线程
        mIpBucket = new LinkedBlockingQueue<>(bufferSize);
    }

    public boolean isValidateIp(String ip) {
        return !TextUtils.isEmpty(ip) && !BLACK_IP_LIST.contains(ip);
    }

    /**
     * 扫描WiFi局域网设备
     *
     * @param curIp 当前设备ip
     */
    public void scanWifiDevice(String curIp) {
        if (!isValidateIp(curIp)) {
            mView.scanTip(curIp);
            return;
        }
        final String[] ipArr = curIp.split("\\.");
        if (ipArr.length != 4) {
            return;
        }
        final String ipPrefix = ipArr[0] + "." + ipArr[1] + "." + ipArr[2] + ".";

        mScanDeviceList.clear();
        finishedSize = 0;
        mTotalScanIp = endIp - startIp + 1;
        forEachIp(ipPrefix);
    }

    private volatile boolean noMore = false;

    private void forEachIp(@NonNull final String ipPrefix) {
        ExecutorServiceUtil.getExecutorService().execute(() -> {
            int putSize = 0;
            for (int i = startIp; i <= endIp; i++) {
                if (mDestroy) {
                    break;
                }
                putSize++;
                try {
                    if (putSize > bufferSize) {
                        // 保证最多只有bufferSize个网络请求
                        pubLock.lock();
                        finished.await();
                        Log.d(TAG, "有一个扫描请求结束了，这里再添加一个到队列中");
                    }
                    if (mDestroy) {
                        break;
                    }
                    final String ip = ipPrefix + i;
                    mIpBucket.put(ip);
                    Log.d(TAG, "添加IP：" + ip);
                } catch (InterruptedException ignore) {
                    // ignore
                } finally {
                    if (pubLock.isLocked()) {
                        pubLock.unlock();
                    }
                }
            }
            noMore = true;
        });

        beginScan();
    }

    /**
     * 扫描连接AP热点的所有设备
     */
    public void scanApDeviceList() {
        List<String> ipList;
        try {
            ipList = NetWorkUtil.getApConnectIp();
        } catch (Exception e) {
            Log.e(TAG, e.toString());
            final String hostIp = NetWorkUtil.getApHostIp();
            Log.d(TAG,"hostIp=" + hostIp);
            ToastUtil.toast(MyApp.getAppContext().getString(R.string.tip_cur_ip_addr, hostIp));
            scanWifiDevice(hostIp);
            return;
        }
        if (ipList.isEmpty()) {
            ToastUtil.toast(MyApp.getAppContext().getString(R.string.tip_hot_spot_no_device));
            mView.scanFinished();
            return;
        }
        mScanDeviceList.clear();
        finishedSize = 0;
        mTotalScanIp = ipList.size();
        ExecutorServiceUtil.getExecutorService().execute(() -> {
            int putSize = 0;
            for (String ip : ipList) {
                if (mDestroy) {
                    break;
                }
                putSize++;
                try {
                    if (putSize > bufferSize) {
                        // 保证最多只有bufferSize个网络请求
                        pubLock.lock();
                        finished.await();
                        Log.d(TAG, "有一个扫描请求结束了，这里再添加一个到队列中");
                    }
                    if (mDestroy) {
                        break;
                    }
                    mIpBucket.put(ip);
                    Log.d(TAG, "添加IP：" + ip);
                } catch (InterruptedException ignore) {
                    // ignore
                } finally {
                    if (pubLock.isLocked()) {
                        pubLock.unlock();
                    }
                }
            }
            noMore = true;
        });
        beginScan();
    }

    /**
     * 开始扫描
     */
    private void beginScan() {
        ExecutorServiceUtil.getExecutorService().execute(() -> {
            while (true) {
                final String ip = mIpBucket.poll();
                if ((ip == null && noMore) || mDestroy) {
                    break;
                }
                if (ip == null) {
                    continue;
                }
                Log.d(TAG, "扫描IP：" + ip);
                getDeviceInfo(ip);
            }
        });
    }

    private void getDeviceInfo(@NonNull String ip) {
        final Disposable disposable = RxUtil.apply(ApiManager.getDeviceFindApi(ip).getDeviceInfo())
                .subscribe(deviceBaseInfo -> {
                    if (mDestroy) {
                        return;
                    }
                    Log.d(TAG, "扫描到设备：" + ip);
                    deviceBaseInfo.setIp(ip);
                    mScanDeviceList.add(deviceBaseInfo);
                    mView.showDeviceList(new ArrayList<>(mScanDeviceList));
                    finishOneScan();
                }, throwable -> {
                    if (mDestroy) {
                        return;
                    }
                    finishOneScan();
                    Log.d(TAG, "扫描失败IP：" + ip);
                    Log.d(TAG, throwable.toString());
                });
        mDisposableSet.add(disposable);
    }

    private int getTotalScanIp() {
        return mTotalScanIp;
    }

    /**
     * 测试用
     */
    public void testResponse() {
        mTotalScanIp = 1;
        getDeviceInfo("www.baidu.com");
    }

    /**
     * 完成一个扫描
     */
    @MainThread
    private void finishOneScan() {
        finishedSize++;
        final int all = getTotalScanIp();
        mView.showProgress(finishedSize * 100 / all);
        if (finishedSize >= all) {
            mView.scanFinished();
        }
        pubLock.lock();
        finished.signal();
        pubLock.unlock();
    }

    public void destroy() {
        mDestroy = true;
        mDisposableSet.dispose();
        mIpBucket.clear();
    }

}
