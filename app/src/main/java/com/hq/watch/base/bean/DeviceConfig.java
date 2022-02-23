package com.hq.watch.base.bean;

/**
 * Created on 2020/11/21
 * author :
 * desc :
 */
public class DeviceConfig {
    private int palette;
    private int zoom;
    private int sharpness;
    private int contrast;
    private int brightness;
    private int noiseReduction;
    private int reticle;
    private int x;
    private int y;
    private int trackEn;
    private int gpsEn;
    private int pipEnable;
    private int distanceEn;

    public boolean getPipEnable() {
        return pipEnable == 1;
    }

    public void setPipEnable(int pipEnable) {
        this.pipEnable = pipEnable;
    }

    public int getPalette() {
        return palette;
    }

    public void setPalette(int palette) {
        this.palette = palette;
    }

    public int getZoom() {
        return zoom;
    }

    public void setZoom(int zoom) {
        this.zoom = zoom;
    }

    public int getSharpness() {
        return sharpness;
    }

    public void setSharpness(int sharpness) {
        this.sharpness = sharpness;
    }

    public int getContrast() {
        return contrast;
    }

    public void setContrast(int contrast) {
        this.contrast = contrast;
    }

    public int getBrightness() {
        return brightness;
    }

    public void setBrightness(int brightness) {
        this.brightness = brightness;
    }

    public int getNoiseReduction() {
        return noiseReduction;
    }

    public void setNoiseReduction(int noiseReduction) {
        this.noiseReduction = noiseReduction;
    }

    public int getReticle() {
        return reticle;
    }

    public void setReticle(int reticle) {
        this.reticle = reticle;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public boolean getTrackEn() {
        return trackEn == 1;
    }

    public void setTrackEn(int trackEn) {
        this.trackEn = trackEn;
    }

    public boolean getGpsEn() {
        return gpsEn == 1;
    }

    public void setGpsEn(int gpsEn) {
        this.gpsEn = gpsEn;
    }

    public boolean getDistanceEn() {
        return distanceEn == 1;
    }

    public void setDistanceEn(int distanceEn) {
        this.distanceEn = distanceEn;
    }

    @Override
    public String toString() {
        return "DeviceConfig{" +
                "palette=" + palette +
                ", zoom=" + zoom +
                ", sharpness=" + sharpness +
                ", contrast=" + contrast +
                ", brightness=" + brightness +
                ", noiseReduction=" + noiseReduction +
                ", reticle=" + reticle +
                ", x=" + x +
                ", y=" + y +
                ", trackEn=" + trackEn +
                ", gpsEn=" + gpsEn +
                ", distanceEn=" + distanceEn +
                '}';
    }
}
