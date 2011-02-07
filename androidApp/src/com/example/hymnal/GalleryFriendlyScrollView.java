package com.example.hymnal;

import android.content.Context;
import android.view.MotionEvent;
import android.widget.Gallery;
import android.widget.ScrollView;

public class GalleryFriendlyScrollView extends ScrollView{

    private static int NONE = -1;
    private static int DOSCROLLVIEW = 0;
    private static int DOGALLERY = 1;

    private float lastx = 0;
    private float lasty = 0;
    private float firstx = 0;
    private float firsty = 0;
    private float lastRawx = 0;
    private float lastRawy = 0;
    private int gestureRecipient = NONE;
    private boolean donewithclick = true;
    private Gallery parent = null;
    private boolean firstclick = true;

    public GalleryFriendlyScrollView(Context context) {
        super(context);
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
		super.onTouchEvent(ev);

        boolean retthis = true;
        //Indicating a fresh click
        if(donewithclick){
            firstx = ev.getX();
            firsty = ev.getY();
            lastx = firstx;
            lasty = firsty;
            lastRawx = ev.getRawX();
            lastRawy = ev.getRawY();
            donewithclick = false;
            firstclick = true;
        }
        //We don't know where these gesture events are supposed to go to. 
        //We have movement on the x and/or why axes, so we can determine where they should go now.
        if((gestureRecipient == NONE) && (lastx != ev.getX() || lasty != ev.getY())){
            //Determine whether there's more movement vertically or horizontally
            float xdiff = ev.getX() - lastx;
            float ydiff = ev.getY() - lasty;
            if(xdiff < 0)xdiff = xdiff * -1;
            if(ydiff < 0)ydiff = ydiff * -1;
            if((xdiff) > (ydiff)){
                gestureRecipient = DOGALLERY;
            } else {
                gestureRecipient = DOSCROLLVIEW;
            }
        }
        if(gestureRecipient == DOGALLERY){
            if(!firstclick){
                //After you drag the screen left or right a bit, the baseline for where it calculates
                //x and y from changes, so we need to adjust the offset to our original baseline
                float offsetx = (((ev.getX() - lastx) - (ev.getRawX() - lastRawx)) * -1);
                float offsety = (((ev.getY() - lasty) - (ev.getRawY() - lastRawy)) * -1);
                ev.offsetLocation(offsetx, offsety);
            }
            retthis = getGallery().onTouchEvent(ev);
            firstclick = false;
        } else if(gestureRecipient == DOSCROLLVIEW){
            retthis = super.onTouchEvent(ev);
        }
        if(ev.getAction() == MotionEvent.ACTION_UP){
            //User's finger has been lifted
            if(((firstx == ev.getX()) && (firsty == ev.getY()))){
                //Since there isn't any movement in either direction, it's a click
                getGallery().onSingleTapUp(ev);
                super.onTouchEvent(ev);
            }
            donewithclick = true;
            gestureRecipient = NONE;
        }
        //And record our coordinate data
        lastx = ev.getX();
        lasty = ev.getY();
        lastRawx = ev.getRawX();
        lastRawy = ev.getRawY();
        return retthis;
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
        getGallery().onTouchEvent(ev);
        return super.onInterceptTouchEvent(ev);
    }

    private Gallery getGallery(){
        //Gets the gallery, in this case assuming the ScrollView is the direct child in the gallery.
        //Adjust as needed
        if(parent == null){
            parent = (Gallery) this.getParent();
        }
        return parent;
    }
}
