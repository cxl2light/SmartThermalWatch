package com.hq.watch.base.bean;

public class QuickBean {
    private int intOne;
    private int intTwo;
    private int intThree;
    private boolean checkOne;
    private boolean checkTwo;
    private String strOne;
    private String strTwo;

    public QuickBean(int intOne, int intTwo) {
        this.intOne = intOne;
        this.intTwo = intTwo;
    }

    public QuickBean(String strOne, String strTwo) {
        this.strOne = strOne;
        this.strTwo = strTwo;
    }

    public QuickBean(int intOne, String strOne) {
        this.intOne = intOne;
        this.strOne = strOne;
    }

    public QuickBean(boolean checkOne, int intOne, String strOne) {
        this.checkOne = checkOne;
        this.intOne = intOne;
        this.strOne = strOne;
    }

    public QuickBean(boolean checkOne, int intOne, int intTwo) {
        this.checkOne = checkOne;
        this.intOne = intOne;
        this.intTwo = intTwo;
    }

    public QuickBean(int intOne, int intTwo, int intThree) {
        this.intOne = intOne;
        this.intTwo = intTwo;
        this.intThree = intThree;
    }

    public QuickBean(boolean checkOne, boolean checkTwo, int intOne, String strOne) {
        this.checkOne = checkOne;
        this.checkTwo = checkTwo;
        this.intOne = intOne;
        this.strOne = strOne;
    }

    public QuickBean(boolean checkOne, boolean checkTwo, int intOne, int intTwo) {
        this.checkOne = checkOne;
        this.checkTwo = checkTwo;
        this.intOne = intOne;
        this.intTwo = intTwo;
    }

    public int getIntTwo() {
        return intTwo;
    }

    public void setIntTwo(int intTwo) {
        this.intTwo = intTwo;
    }

    public int getIntThree() {
        return intThree;
    }

    public void setIntThree(int intThree) {
        this.intThree = intThree;
    }

    public boolean isCheckOne() {
        return checkOne;
    }

    public void setCheckOne(boolean checkOne) {
        this.checkOne = checkOne;
    }

    public boolean isCheckTwo() {
        return checkTwo;
    }

    public void setCheckTwo(boolean checkTwo) {
        this.checkTwo = checkTwo;
    }

    public String getStrOne() {
        return strOne;
    }

    public int getIntOne() {
        return intOne;
    }

    public void setIntOne(int intOne) {
        this.intOne = intOne;
    }

    public void setStrOne(String strOne) {
        this.strOne = strOne;
    }

    public String getStrTwo() {
        return strTwo;
    }

    public void setStrTwo(String strTwo) {
        this.strTwo = strTwo;
    }
}
