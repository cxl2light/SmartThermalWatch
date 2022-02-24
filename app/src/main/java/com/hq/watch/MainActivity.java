package com.hq.watch;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.listener.OnItemClickListener;
import com.chad.library.adapter.base.viewholder.BaseViewHolder;
import com.hq.watch.adapter.DevicePopupListAdapter;
import com.hq.watch.base.BaseActivity;
import com.hq.watch.base.CommonConst;
import com.hq.watch.base.GlobalConst;
import com.hq.watch.base.bean.DeviceBaseInfo;
import com.hq.watch.device.ControlDeviceIJKActivity;
import com.hq.watch.device.DeviceScanPresenter;
import com.hq.watch.device.IDeviceScanView;
import com.hq.watch.dialog.CommonConfirmDialog;
import com.hq.watch.utils.DensityUtil;
import com.hq.watch.utils.NetWorkUtil;
import com.hq.watch.utils.SpUtils;
import com.hq.watch.utils.ToastUtil;
import com.hq.watch.view.LoadingView;
import com.hq.watch.widget.DeviceWidget;
import com.hq.watch.widget.SpaceItemDecoration;

import java.util.ArrayList;

public class MainActivity extends BaseActivity implements View.OnClickListener , IDeviceScanView {

    private String TAG = "MainActivity";
    private LoadingView loadingView;

    private TextView textBtn;
    private TextView tvDeviceName;
    private RelativeLayout rlDeviceName;

    private DeviceScanPresenter mDeviceScanPresenter;

    private BaseQuickAdapter<DeviceBaseInfo, BaseViewHolder> deviceAdapter;
    private RecyclerView deviceRecyclerList;

    ArrayList<DeviceBaseInfo> mDeviceList = new ArrayList<>();

    private Boolean scanSuccess = false;

    private int[] intText = {R.string.scan_device, R.string.connect_device};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mDeviceScanPresenter = new DeviceScanPresenter(this);
        setContentView(R.layout.activity_main);
        initView();
    }

    private void initView() {

        loadingView = findViewById(R.id.view_loading);
        textBtn = findViewById(R.id.text_btn);
        textBtn.setOnClickListener(this);
        rlDeviceName = findViewById(R.id.rlDeviceName);
        rlDeviceName.getLayoutParams().width = CommonConst.getCommonWidgetWidth(mActivity);
        rlDeviceName.setOnClickListener(this);
        tvDeviceName = findViewById(R.id.tvDeviceName);

        deviceRecyclerList = findViewById(R.id.device_recycler_list);

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.text_btn:
                if (scanSuccess){
                    loadingView.setVisibility(View.VISIBLE);
                    new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            SpUtils.saveBean2Sp(mActivity,preSelectedDeviceInfo, SpUtils.SP_DEVICE_INFO);
                            ControlDeviceIJKActivity.startActivity(MainActivity.this);
                        }
                    },2000);
                }
                else {
                    initScanStart();
                }
                textBtn.setEnabled(false);

                break;
            case R.id.rlDeviceName:
                if (mDeviceList.size() < 1){
                    ToastUtil.toast("获取设备信息有误，请返回重试");
                } else {
                    showPopupWindow();
                }
                break;
            default:
                break;
        }

    }

    @Override
    protected void onPause() {
        super.onPause();
        loadingView.setVisibility(View.GONE);
    }

    private void initScanStart() {
        loadingView.setVisibility(View.VISIBLE);

        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                scanDevice();
            }
        },2000);
    }

    private void scanDevice() {
        final boolean wifi = NetWorkUtil.isWifiEnabled(mActivity);
        Log.d(TAG,"curIpZero=" + wifi);
        if (wifi) {
            // WiFi情况下
            String curIp = NetWorkUtil.getWifiIp(mActivity.getApplicationContext());
            Log.d(TAG,"curIpOne=" + curIp);
            if (mDeviceScanPresenter.isValidateIp(curIp)) {
                mDeviceScanPresenter.scanWifiDevice(curIp);
            } else {
                curIp = NetWorkUtil.getApHostIp();
                Log.d(TAG,"curIpTwo=" + curIp);
                if (mDeviceScanPresenter.isValidateIp(curIp)) {
                    mDeviceScanPresenter.scanWifiDevice(curIp);
                }
            }
        } else {
            final boolean ap = NetWorkUtil.isApOn(mActivity);
            Log.d(TAG,"curIpThree=" + ap);
            if (ap) {
                requestReadPermission();
                return;
            }
            if (GlobalConst.isDebug()) {
                ToastUtil.toast(getString(R.string.tip_open_wi_fi_hotspots));
            }
            final CommonConfirmDialog dialog = new CommonConfirmDialog(mActivity);
            dialog.setContent(getString(R.string.tip_open_net));
            dialog.show();
        }
    }

    private static final int REQUEST_CODE_PERMISSION_READ_STORAGE = 0xff10;

    private void requestReadPermission() {
        final int result = ContextCompat.checkSelfPermission(mActivity, Manifest.permission.READ_EXTERNAL_STORAGE);
        if (result == PackageManager.PERMISSION_GRANTED) {
            mDeviceScanPresenter.scanApDeviceList();
            return;
        }
        mActivity.requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, REQUEST_CODE_PERMISSION_READ_STORAGE);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode != REQUEST_CODE_PERMISSION_READ_STORAGE) {
            return;
        }
        mDeviceScanPresenter.scanApDeviceList();
    }

    /**
     * 选择设备pop
     */
    private void showPopupWindow() {
        View popupView = LayoutInflater.from(this).inflate(R.layout.popupwindows_recycleview_base,null);
        PopupWindow popupWindow = new PopupWindow(mActivity);
        popupWindow.setContentView(popupView);
        popupWindow.setWidth(ViewGroup.LayoutParams.WRAP_CONTENT);
        popupWindow.setHeight(ViewGroup.LayoutParams.WRAP_CONTENT);
        popupWindow.setBackgroundDrawable(new ColorDrawable(getColor(android.R.color.transparent)));    //要为popWindow设置一个背景才有效
        popupWindow.setFocusable(true);
        popupWindow.showAsDropDown(rlDeviceName);
        popupView.getLayoutParams().width = CommonConst.getCommonWidgetWidth(mActivity);
        RecyclerView rvPopup = popupView.findViewById(R.id.rv_popup);
        DevicePopupListAdapter mPopAdapter = new DevicePopupListAdapter();
        rvPopup.setLayoutManager(new GridLayoutManager(mActivity, 1, RecyclerView.VERTICAL, false));
        rvPopup.setAdapter(mPopAdapter);
        mPopAdapter.setList(mDeviceList);
        mPopAdapter.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(@NonNull BaseQuickAdapter<?, ?> adapter, @NonNull View view, int position) {
                preSelectedDeviceInfo = mDeviceList.get(position);
                tvDeviceName.setText(mDeviceList.get(position).getDevName());
                popupWindow.dismiss();
            }
        });
    }

    /**
     * 设备列表
     */
    private void initRecyclerList() {
        deviceRecyclerList.setLayoutManager(new LinearLayoutManager(mActivity));
        deviceRecyclerList.setNestedScrollingEnabled(false);
        deviceAdapter = new BaseQuickAdapter<DeviceBaseInfo, BaseViewHolder>(-1, null) {

            @NonNull
            @Override
            protected BaseViewHolder onCreateDefViewHolder(@NonNull ViewGroup parent, int viewType) {
                final DeviceWidget widget = new DeviceWidget(mActivity);
                final ViewGroup.LayoutParams layoutParams = new ViewGroup.LayoutParams(CommonConst.getCommonWidgetWidth(mActivity),
                        ViewGroup.LayoutParams.WRAP_CONTENT);
                widget.setLayoutParams(layoutParams);
                widget.setOnSelectListener(mOnSelectListener);
                return createBaseViewHolder(widget);
            }

            @Override
            protected void convert(@NonNull BaseViewHolder holder, DeviceBaseInfo d) {
                final DeviceWidget textView = (DeviceWidget) holder.itemView;
                textView.setData(d);
            }
        };
        deviceRecyclerList.setAdapter(deviceAdapter);
        deviceRecyclerList.addItemDecoration(new SpaceItemDecoration(DensityUtil.dpToPx(mActivity, 10)));
    }

    @Override
    public void showDeviceList(@NonNull ArrayList<DeviceBaseInfo> deviceList) {
        for (DeviceBaseInfo info:deviceList){
            Log.d("ZeOne","=" + info.toString());
        }
        Log.d("ZeOne","=" + deviceList.size());
        //去除重复的
        mDeviceList.clear();
        mDeviceList.addAll(deviceList);
        Log.d("ZeOne","=" + deviceList.size());

        boolean isEmpty = mDeviceList.isEmpty();
        if (!isEmpty){
            preSelectedDeviceInfo = mDeviceList.get(0);

            loadingView.setVisibility(View.GONE);

            textBtn.setText(getResources().getString(intText[1]));
            scanSuccess = true;

            rlDeviceName.setVisibility(View.VISIBLE);
            tvDeviceName.setText(mDeviceList.get(0).getDevName());
        } else {
            rlDeviceName.setVisibility(View.GONE);
            textBtn.setText(getResources().getString(intText[0]));
            scanSuccess = false;
        }
    }

    @Override
    public void showProgress(int progress) {

    }

    @Override
    public void scanFinished() {
        if (mDeviceList.isEmpty()) {
            loadingView.setVisibility(View.GONE);
            scanSuccess = false;
        }
        textBtn.setEnabled(true);
    }

    @Override
    public void scanTip(String tip) {

    }

    private DeviceBaseInfo preSelectedDeviceInfo;

    private final DeviceWidget.OnSelectListener mOnSelectListener = new DeviceWidget.OnSelectListener() {
        @Override
        public void onSelect(@NonNull View v, @NonNull DeviceBaseInfo data) {
            if (preSelectedDeviceInfo != null) {
                preSelectedDeviceInfo.setSelected(false);
            }
            preSelectedDeviceInfo = data;
            preSelectedDeviceInfo.setSelected(true);
            deviceAdapter.notifyDataSetChanged();
            textBtn.setEnabled(true);
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mDeviceScanPresenter.destroy();
    }
}