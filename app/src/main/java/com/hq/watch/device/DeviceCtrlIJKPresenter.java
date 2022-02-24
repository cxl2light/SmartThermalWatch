package com.hq.watch.device;

import android.content.Context;
import android.graphics.Bitmap;


import com.hq.watch.utils.ExceptionToTip;
import com.hq.watch.utils.ToastUtil;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created on 2020/12/2
 * author :
 * desc : 设备kongzhi
 */
public class DeviceCtrlIJKPresenter extends DeviceCtrlPresenter {


    public DeviceCtrlIJKPresenter(IDeviceCtrlView ctrlView, String deviceIp,
                                  String rtspUrl, String deviceName) {
        super(ctrlView, deviceIp, rtspUrl, deviceName);
    }

    public boolean takePicture(Context context, Bitmap bm) {
        if (context == null || bm == null) {
            return false;
        }
        final File picPath = new File(generateFile(context, true));
        try {
            if (!picPath.exists()) {
                picPath.createNewFile();
            }
        } catch (IOException e) {
            ToastUtil.toast(ExceptionToTip.toTip(e));
            return false;
        }
        try {
            FileOutputStream foStream = new FileOutputStream(picPath);
            bm.compress(Bitmap.CompressFormat.JPEG, 100, foStream);
            foStream.flush();
            foStream.close();
        } catch (IOException e) {
            ToastUtil.toast(ExceptionToTip.toTip(e));
            return false;
        }
        refreshGallery(picPath.getAbsolutePath());
        return true;
    }

}
