package com.hq.watch.device;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.arthenica.mobileffmpeg.FFmpeg;
import com.hq.watch.MyApp;
import com.hq.watch.base.GlobalConfig;
import com.hq.watch.base.bean.DeviceConfig;
import com.hq.watch.base.bean.EmptyResponse;
import com.hq.watch.base.bean.ValuePackage;
import com.hq.watch.jni.ObservableStart;
import com.hq.watch.net.ApiManager;
import com.hq.watch.net.api.DeviceCtrlApi;
import com.hq.watch.utils.ExceptionToTip;
import com.hq.watch.utils.ExecutorServiceUtil;
import com.hq.watch.utils.RxUtil;
import com.hq.watch.utils.ToastUtil;

import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import io.reactivex.disposables.CompositeDisposable;
import io.reactivex.disposables.Disposable;
import io.reactivex.functions.Consumer;

/**
 * Created on 2020/12/2
 * author :
 * desc : 设备kongzhi
 */
public class DeviceCtrlPresenter {
    private static final String TAG = "DeviceCtrlPresenter";

    private static final int ENABLE_VALUE = 1;
    private static final int DISABLE_VALUE = 0;

    private final CompositeDisposable mDisposableSet = new CompositeDisposable();
    private final String mDeviceIp;
    private final String mRtspUrl;
    private final String mDeviceName;

    private DeviceConfig mDeviceConfig;
    @NonNull
    private IDeviceCtrlView mCtrlView;

    public DeviceCtrlPresenter(@NonNull IDeviceCtrlView ctrlView, String deviceIp, String rtspUrl) {
        this(ctrlView, deviceIp, rtspUrl, null);
    }

    public DeviceCtrlPresenter(@NotNull IDeviceCtrlView ctrlView, String deviceIp, String rtspUrl, String deviceName) {
        mCtrlView = ctrlView;
        this.mDeviceIp = deviceIp;
        this.mRtspUrl = rtspUrl;
        mDeviceName = deviceName;
    }

    public String getRtspUrl() {
        return mRtspUrl;
    }

    private Disposable configDisposable;
    /**
     * 获取配置参数
     */
    public void getDeviceConfig() {
        dispose(configDisposable);
        configDisposable = RxUtil.apply(getTcpObserve().initGetDeviceConfig())
                .subscribe(deviceConfig -> {
                      Log.d("ZeOne","deviceConfig=" + deviceConfig);
//                    mDeviceConfig = deviceConfig;
//                    mCtrlView.showDeviceConfig(deviceConfig);
                }, throwable -> {
                    ToastUtil.toast(ExceptionToTip.toTip(throwable));
                });
        mDisposableSet.add(configDisposable);
    }

    @Nullable
    public DeviceConfig deviceConfig() {
        return mDeviceConfig;
    }

    public void dispatchSeekBarValue(@NonNull ValuePackage valuePackage) {
        switch (valuePackage.getType()) {
            default:
                break;
        }
    }

    public void refreshGallery(String filePath) {
        if (TextUtils.isEmpty(filePath)) {
            return;
        }
        final File file = new File(filePath);
        if (!file.exists()) {
            return;
        }
        final Intent intent;
        if (file.isDirectory()) {
            intent = new Intent(Intent.ACTION_MEDIA_MOUNTED);
            intent.setClassName("com.android.providers.media", "com.android.providers.media.MediaScannerReceiver");
        } else {
            intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        }
        intent.setData(Uri.fromFile(file));
        MyApp.getAppContext().sendBroadcast(intent);
    }

    protected String generateFile(Context context, boolean picture) {
        final SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd-HH:mm:ss", Locale.CHINA);
        final String date = format.format(new Date());
        final File file = new File(getDirPath(context),
                date + (picture ? GlobalConfig.PICTURE_POSTFIX : GlobalConfig.VIDEO_POSTFIX));
        return file.getAbsolutePath();
    }

    public String getDirPath(Context context) {
        return GlobalConfig.getRootSavePath(context, mDeviceName);
    }

    private void dispose(Disposable d) {
        if (d != null) {
            d.dispose();
        }
    }

    private DeviceCtrlApi getCtrlApi() {
        return ApiManager.getDeviceCtrlApi(mDeviceIp);
    }

    private ObservableStart getTcpObserve() {
        return ObservableStart.getObserveStart(mDeviceIp,mCtrlView);
    }

    public void destroy() {
        mDisposableSet.dispose();
        ApiManager.release();
    }

}
