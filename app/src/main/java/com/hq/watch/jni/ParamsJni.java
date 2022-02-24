package com.hq.watch.jni;

/**
 * Created by Pipi on 2017/8/2.
 */

public class ParamsJni {

//    public native int intMethod(int n);

    public native int initDevice(int n);

    public native void initDeviceChannel(String ip, FunGetJsonDevConfig listener,FunUpgrade funUpgrade, FunGetJsonAnalyseResult analyseResult);

    public native int initSetX(int n);
    public native int initSetY(int n);
    public native int initSetSharpness(int n);
    public native int initSetContrast(int n);
    public native int initSetBrightness(int n);
    public native int initSetNoiseReduction(int n);
    public native int initSetReticle(int n);
    public native int initSetZoom(int n);
    public native int initSetPalette(int n);
    public native int initSetDistanceMeasurement(int n);
    public native int initSetTrack(int n);
    public native int initSetPip(int n);
    public native int initSetGPS(int n);

    public native int initGetDeviceConfig();
    public native int initGetAnalyseResult();

    public native int initReboot();
    public native int initDisLink();

    public native int initUpgradeFile(String filePath,FunUpgradeFile funUpgradeFile);

    public native int initUpgradeFile2(String filePath,String filePathOne);

    /**
     * JNI call Java object public function
     */
    private String deviceConfig = "female";
    public void setDeviceConfig(String deviceConfig) {
        this.deviceConfig = deviceConfig;
    }

    public String getDeviceConfig(){
        return deviceConfig;
    }

    /**
     *  https://www.jianshu.com/p/e576c7e1c403
     *  get device info
     */
    public interface FunGetJsonDevConfig {
        void onDeviceConfig(int channelNo, String pJson);
    }

    /**
     * get ai info
     */
    public interface FunGetJsonAnalyseResult {
        void onAnalyseResult(int channelNo, String pJson);
    }

    public interface FunUpgrade {
        void onUpgrade(int channelNo, int nUpgrade);
    }

    /**
     * int CallbackGetUpgradeProgress()
     */
    public interface FunUpgradeFile {
        void onUpgradeFile(int channelNo,float nProgress, int endFlag);
    }

//    public native String stringMethod(String name);

//    public native int intArrayMethod(int[] arr);
//
//
//    public native Person objectMethod(Person person);
//
//    public native ArrayList<Person> personArrayListMethod(ArrayList<Person> persons);


}
