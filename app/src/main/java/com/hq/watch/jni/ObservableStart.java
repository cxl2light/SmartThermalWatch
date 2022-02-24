package com.hq.watch.jni;


import android.text.TextUtils;
import android.util.Log;


import com.google.gson.Gson;
import com.hq.watch.MyApp;
import com.hq.watch.base.bean.DeviceBaseInfo;
import com.hq.watch.base.bean.DeviceConfig;
import com.hq.watch.base.bean.QuickBean;
import com.hq.watch.device.IDeviceCtrlView;
import com.hq.watch.utils.RxBus;
import com.hq.watch.utils.RxUtil;
import com.hq.watch.utils.SpUtils;

import io.reactivex.Observable;
import io.reactivex.ObservableEmitter;
import io.reactivex.ObservableOnSubscribe;
import io.reactivex.annotations.NonNull;

public class ObservableStart {

    private static String mIp = "";
    ParamsJni paramsJni = new ParamsJni();

    private static IDeviceCtrlView mCtrlView;
    private DeviceConfig mDeviceConfig;

     public Observable<Integer> observableStart(int start){
        return Observable.create(new ObservableOnSubscribe<Integer>() {
             @Override
             public void subscribe(@NonNull ObservableEmitter<Integer> emitter) throws Exception {
                 paramsJni.initDevice(start);
             }
         });
     }

    private static volatile ObservableStart observeStart;

    public static ObservableStart getObserveStart() {

        if (TextUtils.isEmpty(mIp)){
            DeviceBaseInfo deviceBaseInfo = (DeviceBaseInfo)SpUtils.getBeanFromSp(MyApp.getAppContext(),SpUtils.SP_DEVICE_INFO);
            if (deviceBaseInfo != null){
                mIp = deviceBaseInfo.getIp();
                Log.d("ZeIp=","start=" + mIp);
            }
        }

        if (observeStart == null) {
            synchronized (ObservableStart.class) {
                if (observeStart == null) {
                    observeStart = new ObservableStart();
                }
            }
        }
        return observeStart;
    }

    public static ObservableStart getObserveStart(String ip, IDeviceCtrlView ctrlView) {
        mIp = ip;
        mCtrlView = ctrlView;
        if (observeStart == null) {
            synchronized (ObservableStart.class) {
                if (observeStart == null) {
                    observeStart = new ObservableStart();
                }
            }
        }
        return observeStart;
    }

    /**
     * 创建通道，注册回调方法
     * @return
     */
    public Observable<Integer> initBase(){
        return Observable.create(new ObservableOnSubscribe<Integer>() {
            @Override
            public void subscribe(@NonNull ObservableEmitter<Integer> emitter) throws Exception {
//                mIp = "天马行空";
                Log.d("ZeOneBase1","mIp=" + mIp);
                paramsJni.initDeviceChannel(mIp, new ParamsJni.FunGetJsonDevConfig() {
                    @Override
                    public void onDeviceConfig(int channel, String json) {
                        Log.d("ZeTwo", "channel=" + channel + ",mIp=" + mIp + ",json" + json);
                        try {
                            mDeviceConfig = new Gson().fromJson(json, DeviceConfig.class);
                        } catch (Exception e) {
                            Log.d("ZeErrorJson", "channel=" + channel + ",json" + json);
                            return;
                        }
                        if (mCtrlView != null) {
                            mCtrlView.showDeviceConfig(mDeviceConfig);
                        }
                    }
                }, new ParamsJni.FunUpgrade() {
                    @Override
                    public void onUpgrade(int channelNo, int nUpgrade) {
                        Log.d("ZeUpgrade", "channel=" + channelNo + ",nUpgrade" + nUpgrade);
                        RxBus.getInstance().post(new QuickBean(channelNo, nUpgrade));
                    }
                }, new ParamsJni.FunGetJsonAnalyseResult() {
                    @Override
                    public void onAnalyseResult(int channelNo, String pJson) {
                        Log.d("ZeTwo=", "channelNo=" + channelNo + ",mIp=" + mIp + ",pJson" + pJson);
                    }
                });
            }
        });
    }

    /**
     * 获取设备控制实时数据
     * @return
     */
    public Observable<Integer> initGetDeviceConfig(){
        RxUtil.apply(ObservableStart.getObserveStart().initBase()).subscribe();
        return Observable.create(new ObservableOnSubscribe<Integer>() {
            @Override
            public void subscribe(@NonNull ObservableEmitter<Integer> emitter) throws Exception {
                int setResult = paramsJni.initGetDeviceConfig();
                Log.d("ZeOne=","initSetValue=" + "value" + ",setResult=" + setResult);
            }
        });
    }

    /**
     * 上传升级文件
     * @param filePath
     * @return
     */
    public Observable<Integer> initUpgradeFile(String filePath, ParamsJni.FunUpgradeFile funUpgradeFile){
        Log.d("ZeIp=","mIp=" + mIp);
        return Observable.create(new ObservableOnSubscribe<Integer>() {
            @Override
            public void subscribe(@NonNull ObservableEmitter<Integer> emitter) throws Exception {
                int setResult = paramsJni.initUpgradeFile(filePath,funUpgradeFile);
                Log.d("ZeOne=","filePath=" + filePath + ",setResult=" + setResult);
                emitter.onNext(setResult);
            }
        });
    }

    //测试JNI读写文件
    public Observable<Integer> initUpgradeFile2(String filePath,ParamsJni.FunUpgradeFile funUpgradeFile){
        return Observable.create(new ObservableOnSubscribe<Integer>() {
            @Override
            public void subscribe(@NonNull ObservableEmitter<Integer> emitter) throws Exception {
                int setResult = paramsJni.initUpgradeFile2(filePath,filePath + "1606");
                Log.d("ZeOne=","filePath=" + filePath + ",setResult=" + setResult);
            }
        });
    }

}
