package com.hq.watch.base.bean;

import java.io.Serializable;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public class DeviceBaseInfo implements Serializable {
    /**
     * softVer : 1.0.00 Nov 16 2020 17:27:22
     * hardware : ZS-T400
     * devName : ZS-T400-00
     */

    private String softVer;
    private String hardware;
    private String devName;

    private String ip;
    private boolean selected;

    public String getSoftVer() {
        return softVer;
    }

    public void setSoftVer(String softVer) {
        this.softVer = softVer;
    }

    public String getHardware() {
        return hardware;
    }

    public void setHardware(String hardware) {
        this.hardware = hardware;
    }

    public String getDevName() {
        return devName;
    }

    public void setDevName(String devName) {
        this.devName = devName;
    }

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public boolean isSelected() {
        return selected;
    }

    public void setSelected(boolean selected) {
        this.selected = selected;
    }

    @Override
    public String toString() {
        return "DeviceBaseInfo{" +
                "softVer='" + softVer + '\'' +
                ", hardware='" + hardware + '\'' +
                ", devName='" + devName + '\'' +
                ", ip='" + ip + '\'' +
                ", selected=" + selected +
                '}';
    }
}
