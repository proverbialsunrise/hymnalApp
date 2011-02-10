package com.example.hymnal;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.Gallery;

public class HymnGallery extends Gallery {
	public HymnGallery(Context context) {
		super(context);
	}
	public HymnGallery(Context c, AttributeSet as ){
		super(c, as);
	}
	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX,
			float distanceY) {
		// TODO Auto-generated method stub
		return super.onScroll(e1, e2, distanceX, distanceY);
	}
	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
			float velocityY) {
    	int velocity = (int)(getWidth() * 1.2);
    	Log.e ( "hymnal", "velocity: " + velocity );
		if ( velocityX > 10 ){
			//setSelection(getSelectedItemPosition()-1);
			//scrollTo(0, 0);
			//return true;
			velocityX = velocity;
		}else if ( velocityX < -10 ){
			velocityX = -velocity;
		}
		
		return super.onFling(e1, e2, velocityX, velocityY);
	}
}
