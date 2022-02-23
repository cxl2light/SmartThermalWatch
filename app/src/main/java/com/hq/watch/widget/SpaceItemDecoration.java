package com.hq.watch.widget;

import android.graphics.Rect;
import android.view.View;

import androidx.recyclerview.widget.RecyclerView;

/**
 * Created by 何清 on 2017/8/2.
 * Function：RecyclerView 的每一个条目之间的间距
 */

public class SpaceItemDecoration extends RecyclerView.ItemDecoration {

    private final int topSpace;
    private int bottomSpace = 0;
    private int rightSpace = 0;
    private int leftSpace = 0;

    public SpaceItemDecoration(int topSpace) {
        this.topSpace = topSpace;
    }

    public SpaceItemDecoration(int topSpace, int bottomSpace) {
        this.topSpace = topSpace;
        this.bottomSpace = bottomSpace;
    }

    public SpaceItemDecoration(int topSpace, int bottomSpace, int rightSpace, int leftSpace) {
        this.topSpace = topSpace;
        this.bottomSpace = bottomSpace;
        this.rightSpace = rightSpace;
        this.leftSpace = leftSpace;
    }

    @Override
    public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
        outRect.top = topSpace;
        outRect.right = rightSpace;
        outRect.left = leftSpace;
        if (parent.getChildAdapterPosition(view) == state.getItemCount() - 1) {
            outRect.bottom = bottomSpace;
        }
    }

}
