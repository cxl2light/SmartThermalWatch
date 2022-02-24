package com.hq.watch.base.bean;

/**
 * Created on 2020/6/6
 * author :
 * desc :
 */
public class ValuePackage {
    // 锐度
    public static final int TYPE_ACUITY = 0;
    // 对比度
    public static final int TYPE_CONTRAST_RATIO = 1;
    // 亮度
    public static final int TYPE_BRIGHTNESS = 2;
    // 降噪
    public static final int TYPE_NOISE_REDUCTION = 3;
    // 分划
    public static final int TYPE_POLARIZATION = 4;
    // x
    public static final int TYPE_X = 5;
    // y
    public static final int TYPE_Y = 6;
    // 变倍
    public static final int TYPE_ZOOM = 7;

    private final int type;
    private final int min;
    private final int max;
    private int currentValue;
    private final OnValueChange onValueChange;
    private String minStr;
    private String maxStr;

    public ValuePackage(int type, int min, int max) {
        this.type = type;
        this.min = min;
        this.max = max;
        this.onValueChange = value -> currentValue = value;
    }

    public int getMin() {
        return min;
    }

    public int getMax() {
        return max;
    }

    public OnValueChange getOnValueChange() {
        return onValueChange;
    }

    public int getCurrentValue() {
        return currentValue;
    }

    public void setCurrentValue(int currentValue) {
        this.currentValue = currentValue;
    }

    public int getType() {
        return type;
    }

    public void setMinMaxStr(String minStr, String maxStr) {
        this.minStr = minStr;
        this.maxStr = maxStr;
    }

    public String getMinStr() {
        return minStr;
    }

    public void setMinStr(String minStr) {
        this.minStr = minStr;
    }

    public String getMaxStr() {
        return maxStr;
    }

    public void setMaxStr(String maxStr) {
        this.maxStr = maxStr;
    }
}
