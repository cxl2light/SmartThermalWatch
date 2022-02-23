package com.hq.watch.widget;

import android.content.Context;
import android.content.res.ColorStateList;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.StateListDrawable;
import android.util.AttributeSet;
import android.view.Gravity;

import androidx.annotation.ColorInt;
import androidx.annotation.ColorRes;
import androidx.appcompat.widget.AppCompatButton;

import com.hq.watch.R;

import java.util.Arrays;

/**
 * Created on 2020/4/5.
 * author :
 * desc : 有选择器的按钮，圆角矩形的圆角半径可以随意指定，也可以半圆样式
 */
public class WidgetSelectorBtn extends AppCompatButton {
    private static final int LEFT = 1;
    private static final int RIGHT = 2;

    private final int[] normal_color = new int[2];
    private final int[] pressed_color = {0xfff38b53, 0xfff38b53};
    private final int[] disable_color = {0xFFb6b6b6, 0xFFb6b6b6};
    private final int[] text_color = {0xFF333333, 0xFF333333, 0xFFFFFFFF, 0xFFFFFFFF};

    private GradientDrawable gradientDrawableNormal;
    private GradientDrawable gradientDrawablePressed;
    private GradientDrawable drawableDisable;
    private int strokeWidth = 0;
    private int strokeColorNormal = 0xffb6b6b6;
    private int strokeColorPress = 0xffb6b6b6;
    private int strokeColorDisable = 0xffb6b6b6;
    private boolean halfCorner = false;//是否是半圆
    private int halfCornerOneSide = -1;//某一边是否是半圆（左边或者右边）
    private int cornersRadius = 0;//圆角半径
    private boolean sameWidthHeight = false;
    private ColorStateList textColorStateList;

    public WidgetSelectorBtn(Context context) {
        this(context, null);
    }

    public WidgetSelectorBtn(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public WidgetSelectorBtn(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        final Resources resources = getResources();
        final int normalColor = resources.getColor(R.color.default_btn_color_normal, null);
        normal_color[0] = normalColor;
        normal_color[1] = normalColor;

        final int pressColor = resources.getColor(R.color.default_btn_color_press, null);
        pressed_color[0] = pressColor;
        pressed_color[1] = pressColor;

        final int disableColor = resources.getColor(R.color.default_btn_color_disable, null);
        disable_color[0] = disableColor;
        disable_color[1] = disableColor;

        text_color[3] = resources.getColor(R.color.default_btn_text_color, null);

        init(attrs);
    }

    private void init(AttributeSet attrs) {
        setGravity(Gravity.CENTER);
        boolean noBackground = false;
        final float[] cornerRadius = new float[8];
        if (attrs != null) {
            TypedArray array = getContext().obtainStyledAttributes(attrs, R.styleable.WidgetSelectorBtn);
            strokeWidth = (int) array.getDimension(R.styleable.WidgetSelectorBtn_widget_selector_btn_stroke_width, 0);
            strokeColorNormal = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_stroke_color, strokeColorNormal);
            strokeColorPress = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_stroke_color_press, strokeColorNormal);
            strokeColorDisable = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_stroke_color_disable, strokeColorNormal);
            noBackground = array.getBoolean(R.styleable.WidgetSelectorBtn_widget_selector_btn_no_background, noBackground);

            normal_color[0] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_normal_start_color, normal_color[0]);
            normal_color[1] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_normal_end_color, normal_color[0]);

            pressed_color[0] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_press_start_color, pressed_color[0]);
            pressed_color[1] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_press_end_color, pressed_color[0]);

            disable_color[0] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_disable_start_color, disable_color[0]);
            disable_color[1] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_disable_end_color, disable_color[0]);

            sameWidthHeight = array.getBoolean(R.styleable.WidgetSelectorBtn_widget_selector_btn_same_w_h, sameWidthHeight);
            halfCorner = array.getBoolean(R.styleable.WidgetSelectorBtn_widget_selector_btn_corners_half_circle, false);
            if (!halfCorner) {
                halfCornerOneSide = array.getInt(R.styleable.WidgetSelectorBtn_widget_selector_btn_corners_half_circle_one_side, -1);
                if (halfCornerOneSide != LEFT && halfCornerOneSide != RIGHT) {
                    cornersRadius = (int) array.getDimension(R.styleable.WidgetSelectorBtn_widget_selector_btn_corners_radius, 0);
                    cornerRadius[0] = cornerRadius[1] = (int) array.getDimension(R.styleable.WidgetSelectorBtn_widget_selector_btn_corners_radius_left_top, 0);
                    cornerRadius[2] = cornerRadius[3] = (int) array.getDimension(R.styleable.WidgetSelectorBtn_widget_selector_btn_corners_radius_right_top, 0);
                    cornerRadius[4] = cornerRadius[5] = (int) array.getDimension(R.styleable.WidgetSelectorBtn_widget_selector_btn_corners_radius_right_bottom, 0);
                    cornerRadius[6] = cornerRadius[7] = (int) array.getDimension(R.styleable.WidgetSelectorBtn_widget_selector_btn_corners_radius_left_bottom, 0);
                }
            }

            textColorStateList = array.getColorStateList(R.styleable.WidgetSelectorBtn_widget_selector_btn_text_color);
            text_color[3] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_text_color_normal, text_color[3]);
            text_color[2] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_text_color_press, text_color[3]);
            text_color[1] = text_color[2];
            text_color[0] = array.getColor(R.styleable.WidgetSelectorBtn_widget_selector_btn_text_color_disable, text_color[3]);
            array.recycle();
        }
        gradientDrawableNormal = new GradientDrawable(
                GradientDrawable.Orientation.LEFT_RIGHT, normal_color);
        gradientDrawableNormal.setShape(GradientDrawable.RECTANGLE);
        gradientDrawablePressed = new GradientDrawable(
                GradientDrawable.Orientation.LEFT_RIGHT, pressed_color);
        gradientDrawablePressed.setShape(GradientDrawable.RECTANGLE);
        drawableDisable = new GradientDrawable(
                GradientDrawable.Orientation.LEFT_RIGHT, disable_color);
        drawableDisable.setShape(GradientDrawable.RECTANGLE);

        if (!halfCorner && halfCornerOneSide != LEFT && halfCornerOneSide != RIGHT && cornersRadius > 0) {
            gradientDrawableNormal.setCornerRadius(cornersRadius);
            gradientDrawablePressed.setCornerRadius(cornersRadius);
            drawableDisable.setCornerRadius(cornersRadius);
        } else {
            gradientDrawableNormal.setCornerRadii(cornerRadius);
            gradientDrawablePressed.setCornerRadii(cornerRadius);
            drawableDisable.setCornerRadii(cornerRadius);
        }

        if (strokeWidth > 0) {
            gradientDrawableNormal.setStroke(strokeWidth, strokeColorNormal);
            gradientDrawablePressed.setStroke(strokeWidth, strokeColorPress);
            drawableDisable.setStroke(strokeWidth, strokeColorDisable);
        }

        if (!noBackground) {
            setBackground();
        }
        setTextColor();
    }

    private void setTextColor() {
        if (textColorStateList == null) {
            final int[][] states = new int[4][];
            states[0] = new int[]{-android.R.attr.state_enabled};
            states[1] = new int[]{android.R.attr.state_selected};
            states[2] = new int[]{android.R.attr.state_pressed};
            states[3] = new int[]{};
            textColorStateList = new ColorStateList(states, text_color);
        }
        setTextColor(textColorStateList);
    }

    private void setBackground() {
        final StateListDrawable stateListDrawable = new StateListDrawable();
        stateListDrawable.addState(new int[]{-android.R.attr.state_enabled}, drawableDisable);
        stateListDrawable.addState(new int[]{android.R.attr.state_pressed}, gradientDrawablePressed);
        stateListDrawable.addState(new int[]{android.R.attr.state_selected}, gradientDrawablePressed);
        stateListDrawable.addState(new int[]{android.R.attr.state_hovered}, gradientDrawablePressed);
        stateListDrawable.addState(new int[]{}, gradientDrawableNormal);
        setBackground(stateListDrawable);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        if (!sameWidthHeight) {
            return;
        }
        /*if (getGravity() == Gravity.CENTER) {
            if (getMeasuredWidth() > getMeasuredHeight()) {
                final int padding = (getMeasuredWidth() - getMeasuredHeight()) / 2;
                setPadding(getPaddingLeft(), getPaddingTop() + padding, getPaddingRight(), getPaddingBottom() + padding);
            } else {
                final int padding = (getMeasuredHeight() - getMeasuredWidth()) / 2;
                setPadding(getPaddingLeft() + padding, getPaddingTop(), getPaddingRight() + padding, getPaddingBottom());
            }
            return;
        }*/
        final int result = Math.max(getMeasuredWidth(), getMeasuredHeight());
        setMeasuredDimension(result, result);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        if (!halfCorner && (halfCornerOneSide != RIGHT && halfCornerOneSide != LEFT)) {
            return;
        }
        final float[] cornerRadius = new float[8];
        final float radius = getMeasuredHeight() / 2.0f;
        if (halfCorner) {
            Arrays.fill(cornerRadius, radius);
        } else {
            if (halfCornerOneSide == RIGHT) {
                for (int i = 2; i < 6; i++) {
                    cornerRadius[i] = radius;
                }
            } else if (halfCornerOneSide == LEFT) {
                cornerRadius[0] = cornerRadius[1] = radius;
                cornerRadius[6] = cornerRadius[7] = radius;
            }
        }
        gradientDrawableNormal.setCornerRadii(cornerRadius);
        gradientDrawablePressed.setCornerRadii(cornerRadius);
        drawableDisable.setCornerRadii(cornerRadius);
    }

    public void setDisableStrokeWidth(int strokeWidth) {
        if (drawableDisable != null) {
            drawableDisable.setStroke(strokeWidth, strokeColorDisable);
        }
    }

    public void setNormalColor(@ColorRes int startColor, @ColorRes int endColor) {
        setNormalColorInt(getColor(startColor), getColor(endColor));
    }

    public void setNormalColorInt(@ColorInt int startColor, @ColorInt int endColor) {
        normal_color[0] = startColor;
        normal_color[1] = endColor;
        if (gradientDrawableNormal != null) {
            gradientDrawableNormal.setColors(normal_color);
        }
        setBackground();
    }

    public void setStrokeNormalColorInt(@ColorInt int color) {
        if (gradientDrawableNormal != null) {
            strokeColorNormal = color;
            gradientDrawableNormal.setStroke(strokeWidth, strokeColorNormal);
        }
        setBackground();
    }

    public void setPressColor(@ColorRes int startColor, @ColorRes int endColor) {
        setPressColorInt(getColor(startColor), getColor(endColor));
    }

    public void setPressColorInt(@ColorInt int startColor, @ColorInt int endColor) {
        pressed_color[0] = startColor;
        pressed_color[1] = endColor;
        if (gradientDrawablePressed != null) {
            gradientDrawablePressed.setColors(pressed_color);
        }
        setBackground();
    }

    public void setStrokePressColorInt(@ColorInt int color) {
        if (gradientDrawablePressed != null) {
            strokeColorPress = color;
            gradientDrawablePressed.setStroke(strokeWidth, strokeColorPress);
        }
        setBackground();
    }

    public void setDisableColor(@ColorRes int startColor, @ColorRes int endColor) {
        setDisableColorInt(getColor(startColor), getColor(endColor));
    }

    public void setDisableColorInt(@ColorInt int startColor, @ColorInt int endColor) {
        disable_color[0] = startColor;
        disable_color[1] = endColor;
        if (drawableDisable != null) {
            drawableDisable.setColors(disable_color);
        }
        setBackground();
    }

    /**
     * 获取颜色值
     */
    private int getColor(@ColorRes int srcId) {
        return getResources().getColor(srcId);
    }

    public void setCornersRadius(int cornersRadius) {
        if (!halfCorner && halfCornerOneSide != LEFT && halfCornerOneSide != RIGHT && cornersRadius > 0) {
            gradientDrawableNormal.setCornerRadius(cornersRadius);
            gradientDrawablePressed.setCornerRadius(cornersRadius);
            drawableDisable.setCornerRadius(cornersRadius);
        }

        setBackground();
    }

    public void setStrokeWidth(int strokeWidth) {
        if (strokeWidth > 0) {
            gradientDrawableNormal.setStroke(strokeWidth, strokeColorNormal);
            gradientDrawablePressed.setStroke(strokeWidth, strokeColorPress);
            drawableDisable.setStroke(strokeWidth, strokeColorDisable);
        }
    }
}
