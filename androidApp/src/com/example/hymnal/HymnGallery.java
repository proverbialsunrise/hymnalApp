package com.example.hymnal;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.widget.Gallery;

public class HymnGallery extends Gallery {
	float mScrollDistance;
	public boolean mIsScrolling = false;
	public int mPositionSelected = 0;

	public HymnGallery(Context context) {
		super(context);
	}
	public HymnGallery(Context c, AttributeSet as ){
		super(c, as);
	}
	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX,
			float distanceY) {
		mIsScrolling = true;
		mScrollDistance = getSelectedView().getLeft();
		return super.onScroll(e1, e2, distanceX, distanceY);
	}
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if ( event.getAction() == MotionEvent.ACTION_UP ){
			mIsScrolling = false;
		}
		else if ( event.getAction() == MotionEvent.ACTION_CANCEL ){
			mScrollDistance = 0;
		}
		return super.onTouchEvent(event);
	}
	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
			float velocityY) {
		if ( mPositionSelected != getSelectedItemPosition() ){
			mPositionSelected = getSelectedItemPosition();
			return true;
		}
		mIsScrolling = false;
    	int velocity = (int)(getWidth() * 1.6);
		if ( velocityX > 10 ){
			velocityX = velocity - 3*mScrollDistance;
		}else if ( velocityX < -10 ){
			velocityX = -velocity - 3*mScrollDistance;
		}
		
		return super.onFling(e1, e2, velocityX, velocityY);
	}
}
