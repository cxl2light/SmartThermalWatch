package com.hq.watch.device;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.hq.watch.R;
import com.hq.watch.base.BaseActivity;
import com.hq.watch.base.bean.DeviceBaseInfo;
import com.hq.watch.base.bean.DeviceConfig;
import com.hq.watch.jni.ObservableStart;
import com.hq.watch.utils.ExceptionToTip;
import com.hq.watch.utils.RxUtil;
import com.hq.watch.utils.ScreenUtils;
import com.hq.watch.utils.SpUtils;
import com.hq.watch.utils.ToastUtil;

import java.util.ArrayList;

import tv.danmaku.ijk.media.player.IjkMediaPlayer;
import tv.danmaku.ijk.media.player.widget.IRenderView;
import tv.danmaku.ijk.media.player.widget.TextureRenderView;

/**
 * @author Administrator
 * @date 2022/2/23 0023 11:00
 */
public class ControlDeviceIJKActivity extends BaseActivity implements IDeviceCtrlView, View.OnClickListener {

    private TextureRenderView mPlayerView;
    private IjkMediaPlayer mIjkMediaPlayer;
    private View loadingView;

    Thread mThread;
    Boolean isDestroy = false;

    String deviceIp = "";

    private DeviceCtrlIJKPresenter mCtrlPresenter;

    long delay = 2000;
    Handler mHandler = new Handler(Looper.getMainLooper()){
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case 1:
                    mCtrlPresenter.getDeviceConfig();
                    break;
            }
        }
    };

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_control_device_ijk);
        initView();
    }

    private void initView() {
        DeviceBaseInfo preSelectedDeviceInfo = SpUtils.getBeanFromSp(this,SpUtils.SP_DEVICE_INFO);
        if (preSelectedDeviceInfo == null) {
            ToastUtil.toast(this.getString(R.string.please_select_connect_device));
            return;
        }

        mPlayerView = findViewById(R.id.player_texture_view);
        loadingView = findViewById(R.id.view_loading);

        ConstraintLayout clVideo = findViewById(R.id.clVideo);
        clVideo.setLayoutParams(ScreenUtils.getVideoLayoutParams(this));

        final String rtspUrl = "rtsp://" + preSelectedDeviceInfo.getIp() + ":554/mainstream";
        deviceIp = preSelectedDeviceInfo.getIp();
        final String deviceName = preSelectedDeviceInfo.getDevName();
        if (TextUtils.isEmpty(rtspUrl) || TextUtils.isEmpty(deviceIp)) {
            ToastUtil.toast("设备地址或IP为空");
            return;
        }
        mCtrlPresenter = new DeviceCtrlIJKPresenter(this, deviceIp, rtspUrl, deviceName);
        Log.d("getDeviceCurrent","deviceIp=" + deviceIp + ",rtspUrl=" + rtspUrl + ",deviceName=" + deviceName);
        mCtrlPresenter.getDeviceConfig();

        if (mThread == null){
            mThread = new Thread(new MyRunnable());
            mThread.start();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        RxUtil.apply(ObservableStart.getObserveStart(deviceIp,this).initBase()).subscribe();
        if (mIjkMediaPlayer == null) {
            initPlayer();
        }
    }

    private void initPlayer() {
        Log.d("ZePlayer","mIjkMediaPlayer=" + mIjkMediaPlayer);
        mIjkMediaPlayer = new IjkMediaPlayer();
        final IjkMediaPlayer mediaPlayer = mIjkMediaPlayer;

        setPlayerParams(mediaPlayer);
        //mediaPlayer准备工作-------回调,onPrepared
        mediaPlayer.setOnPreparedListener(mp -> {
            loadingView.setVisibility(View.GONE);
            mp.start();
        });
        //MediaPlayer完成---------回调,onCompletion
        mediaPlayer.setOnCompletionListener(mp -> loadingView.setVisibility(View.GONE));

        try {
            mediaPlayer.setDataSource(mCtrlPresenter.getRtspUrl());
        } catch (Throwable e) {
            ToastUtil.toast(ExceptionToTip.toTip(e));
        }
        initPlayerView();
    }

    private void initPlayerView() {
        mPlayerView.removeRenderCallback(renderCallback);
        mPlayerView.setOnClickListener(this);
        mPlayerView.addRenderCallback(renderCallback);
    }

    private final IRenderView.IRenderCallback renderCallback = new IRenderView.IRenderCallback() {
        @Override
        public void onSurfaceCreated(@NonNull IRenderView.ISurfaceHolder holder, int width, int height) {
            holder.bindToMediaPlayer(mIjkMediaPlayer);
            //开启异步准备
            try {
                mIjkMediaPlayer.prepareAsync();
            } catch (Throwable e) {
                ToastUtil.toast(ExceptionToTip.toTip(e));
            }
        }

        @Override
        public void onSurfaceChanged(@NonNull IRenderView.ISurfaceHolder holder, int format, int width, int height) {

        }

        @Override
        public void onSurfaceDestroyed(@NonNull IRenderView.ISurfaceHolder holder) {

        }
    };


    @Override
    public void onClick(View v) {

    }

    public static void startActivity(Context context) {
        if (context == null) {
            return;
        }
        final Intent intent = new Intent(context, ControlDeviceIJKActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        isDestroy = true;
        release();
    }

    private void release() {
        if (null == mIjkMediaPlayer) {
            return;
        }
        try {
            mIjkMediaPlayer.stop();
            mIjkMediaPlayer.release();
            mIjkMediaPlayer = null;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * https://blog.csdn.net/u013270727/article/details/83900062
     * @param ijkMediaPlayer
     */
    private void setPlayerParams(IjkMediaPlayer ijkMediaPlayer){
        // 支持硬解 1：开启 O:关闭
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "mediacodec-hevc", 1);
        // 设置播放前的探测时间 1,达到首屏秒开效果
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "analyzeduration", 1);
        /**
         * 播放延时的解决方案
         */
        // 如果是rtsp协议，可以优先用tcp(默认是用udp)
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "rtsp_transport", "tcp");
        // 设置播放前的最大探测时间 （100未测试是否是最佳值）
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "analyzemaxduration", 50L);
        // 每处理一个packet之后刷新io上下文
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "flush_packets", 1L);
        // 需要准备好后自动播放
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "start-on-prepared", 1);
        // 不额外优化（使能非规范兼容优化，默认值0 ）
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "fast", 1);
        // 是否开启预缓冲，一般直播项目会开启，达到秒开的效果，不过带来了播放丢帧卡顿的体验
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "packet-buffering",  0);
        // 自动旋屏
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "mediacodec-auto-rotate", 0);
        // 处理分辨率变化
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "mediacodec-handle-resolution-change", 0);
        // 最大缓冲大小,单位kb
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "max-buffer-size", 0);
        // 默认最小帧数2
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "min-frames", 2);
        // 最大缓存时长
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER,  "max_cached_duration", 3); //300
        // 是否限制输入缓存数
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER,  "infbuf", 1);
        // 缩短播放的rtmp视频延迟在1s内
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "fflags", "nobuffer");
        // 播放前的探测Size，默认是1M, 改小一点会出画面更快
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "probesize", 100); //1024L)
        // 播放重连次数
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER,"reconnect",5);
        //
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_FORMAT, "http-detect-range-support", 0);
        // 设置是否开启环路过滤: 0开启，画面质量高，解码开销大，48关闭，画面质量差点，解码开销小
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_CODEC, "skip_loop_filter", 48L);
        // 跳过帧 ？？
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_CODEC, "skip_frame", 0);
        // 视频帧处理不过来的时候丢弃一些帧达到同步的效果
        ijkMediaPlayer.setOption(IjkMediaPlayer.OPT_CATEGORY_PLAYER, "framedrop", 1);

    }

    @Override
    public void showDeviceConfig(@NonNull DeviceConfig deviceConfig) {

    }

    public class MyRunnable implements Runnable {
        @Override
        public void run() {
            // TODO Auto-generated method stub
            while (!isDestroy) {
                try {
                    Message message = new Message();
                    message.what = 1;
                    mHandler.sendMessage(message);// 发送消息
                    Thread.sleep(delay);// 线程暂停10秒，单位毫秒
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
    }
}
