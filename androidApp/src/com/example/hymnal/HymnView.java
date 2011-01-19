package com.example.hymnal;

import java.io.File;
import java.util.Iterator;
import java.util.SortedSet;
import java.util.TreeSet;
import java.util.Vector;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class HymnView extends Activity {
	int currentVerse = 0;
	int hymnId = -1;
	int numVerses = 0;
	static int part = 0;
	File hymnalBaseDir = null;

	public final int ALLPARTS = 0;
	public final int TREBLE = 1;
	public final int BASS = 2;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.hymn);
        
        Bundle extras = getIntent().getExtras();
        String hymnalName = extras.getString("HymnalName");
        String hymnName = extras.getString("HymnName");
        hymnId = extras.getInt("HymnId");
        Log.d ( "hymnal", "hymnId: " + hymnId );
        setTitle(hymnName);

        ScrollView mainScrollView = (ScrollView) findViewById(R.id.hymn_scrollview);
        LinearLayout ll = (LinearLayout) mainScrollView.findViewById(R.id.next_level);
        ll.setBackgroundColor(0xffffffff);
        
        if ( !Environment.MEDIA_MOUNTED.equals(Environment.MEDIA_MOUNTED) &&
             !Environment.MEDIA_MOUNTED.equals(Environment.MEDIA_MOUNTED_READ_ONLY)	){
        	Log.e ( "hymnal", "external storage not there" );
        	return;
        }

        File externalStorage = Environment.getExternalStorageDirectory();
        File[] files = externalStorage.listFiles();
        if ( files == null ){
        	Log.e ( "hymnal", "external storage has no files" );
        	setTitle("error occurred");
        	return;
        }
        for ( int i = 0; i < files.length; ++i ){
        	if ( files[i].getName().equals("hymnalapp")){
        		hymnalBaseDir = files[i];
        		break;
        	}
        }

        String hymnalDirName = hymnalBaseDir + "/" + hymnalName.replace(":", "");
        File hymnalDir = new File ( hymnalDirName );
        files = hymnalDir.listFiles();
        if ( files == null ){
        	Log.e ( "hymnal", "failed getting hymnal dir" );
        	setTitle("error occurred");
        	return;
        }

        currentVerse = 1;
        if ( !Hymnal.EMULATOR )
        	numVerses = getNumVerses(hymnId);
        else
        	numVerses = 4;
        if ( numVerses == -1 )
        	numVerses = 1;
        showPage();
		
		gestureDetector = new GestureDetector(new MyGestureDetector());
        gestureListener = new View.OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                return gestureDetector.onTouchEvent(event);
            }
        };
        mainScrollView.setOnTouchListener(gestureListener);
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	menu.add(Menu.NONE, ALLPARTS, Menu.NONE, "All");
    	menu.add(Menu.NONE, TREBLE, Menu.NONE, "Treble");
    	menu.add(Menu.NONE, BASS, Menu.NONE, "Bass");
    	return super.onCreateOptionsMenu(menu);
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	part = item.getItemId();
    	clearPage();
    	showPage();
    	return true;
    }
    
    private static final int SWIPE_MIN_DISTANCE = 120;
    private static final int SWIPE_MAX_OFF_PATH = 250;
    private static final int SWIPE_THRESHOLD_VELOCITY = 200;
    private GestureDetector gestureDetector;
    View.OnTouchListener gestureListener;
    class MyGestureDetector extends SimpleOnGestureListener {
        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
            try {
                if (Math.abs(e1.getY() - e2.getY()) > SWIPE_MAX_OFF_PATH)
                    return false;
                // right to left swipe
                if(e1.getX() - e2.getX() > SWIPE_MIN_DISTANCE && Math.abs(velocityX) > SWIPE_THRESHOLD_VELOCITY) {
                	changeVerse ( true );
                }  else if (e2.getX() - e1.getX() > SWIPE_MIN_DISTANCE && Math.abs(velocityX) > SWIPE_THRESHOLD_VELOCITY) {
                	changeVerse ( false );
                }
                return true;
            } catch (Exception e) {
                // nothing
            }
            return false;
        }
    }
    
    private final void changeVerse ( boolean advance ){
    	if ( advance ){
    		if ( currentVerse == numVerses ){
    	        ScrollView mainScrollView = (ScrollView) findViewById(R.id.hymn_scrollview);
    	        mainScrollView.scrollTo(0, 0);
    			return;
    		}
    		++currentVerse;
    	}else{
    		if ( currentVerse == 0 )
    			return;
    		--currentVerse;
    	}
		clearPage();
		showPage();
    }
    
    private void clearPage(){
        ScrollView mainScrollView = (ScrollView) findViewById(R.id.hymn_scrollview);
        mainScrollView.scrollTo(0, 0);
        LinearLayout ll = (LinearLayout) mainScrollView.findViewById(R.id.next_level);
        ll.removeAllViews();
    }
    
    private void showPage(){
        Vector<String> pieces = getHymnPieces(hymnId, part);
        ScrollView mainScrollView = (ScrollView) findViewById(R.id.hymn_scrollview);
        LinearLayout ll = (LinearLayout) mainScrollView.findViewById(R.id.next_level);
        TextView filler = new TextView(getApplicationContext());
        filler.setHeight(5);
        filler.setBackgroundColor(0xffffffff);
        ll.addView( filler );
        for ( int i = 0; i < pieces.size(); ++i ){
        	ImageView image = new ImageView(getApplicationContext());
        	String path = hymnalBaseDir + "/" + pieces.get(i);
        	//Log.e ( "hymnal", "path: " + path );
        	Bitmap bm = BitmapFactory.decodeFile(path);
        	image.setImageBitmap(bm);
        	image.setScaleType(ScaleType.FIT_XY);
        	DisplayMetrics dm = new DisplayMetrics();
        	getWindowManager().getDefaultDisplay().getMetrics(dm);
        	int width = dm.widthPixels;
        	int height = width * bm.getHeight() / bm.getWidth();
        	image.setLayoutParams(new LinearLayout.LayoutParams(width, height));
        	if ( pieces.get(i).contains("bass") ){
        		image.setPadding(0, 0, 0, 5);
        	}else{
        		image.setPadding(0, 0, 0, 0);
        	}
        	ll.addView(image);
        }
    	
    }
    
    //public native String[] getLyricSections ( int hymnId, int partSpecifier );
    public native String[] getMusicSections ( int hymnId, int partSpecifier );
    public native String[] getSections ( int hymnId, int verse, int partSpecifier );
    public native int getNumVerses ( int hymnId );
	static {
		if ( !Hymnal.EMULATOR )
			System.loadLibrary ( "Database-Jni" );
	}

    private Vector<String> getHymnPieces ( int hymnId, int partSpecifier ){
    	if ( Hymnal.EMULATOR ){
    		return getHymnPieces_noJni(hymnId, partSpecifier);
    	}
    	String[] pieces = getSections ( hymnId, currentVerse, partSpecifier );
    	SortedSet<String> sortedSet = new TreeSet<String> ( );
    	for ( int i = 0; i < pieces.length; ++i ){
    		sortedSet.add(pieces[i]);
    	}
    	
    	Vector<String> v = new Vector<String>();
    	Iterator<String> it = sortedSet.iterator();
    	while ( it.hasNext() ){
    		v.add(it.next());
    	}
    	return v;
    }
    
    //just a hack to display one particular hymn without using the database
    private Vector<String> getHymnPieces_noJni ( int hymnId, int partSpecifier ){
    	Vector<String> ret = new Vector<String>();
    	File dir = new File("/sdcard/hymnalapp/Hymnal A Worship Book/043 My faith has foundmus/");
    	String basePath = "Hymnal A Worship Book/043 My faith has foundmus/";
    	String list[] = dir.list();
    	SortedSet<String> sortedSet = new TreeSet<String>();
    	
    	for ( int i = 0; i < list.length; ++i ){
    		if ( list[i].startsWith(".") || list[i].contains ( "title" ) )
    			continue;
    		sortedSet.add(list[i]);
    	}
    	
    	int verse = currentVerse;
    	String clef = "png";
    	if ( partSpecifier == 1 )
    		clef = "treble";
    	else if ( partSpecifier == 2 )
    		clef = "bass";
 
    	Iterator<String> it = sortedSet.iterator();
    	while ( it.hasNext() ){
    		String s = it.next();
    		//Log.e ( "hymnal", "piece: " + basePath + "/" + s );
    		if (   ( s.contains(clef) && !s.contains("text") ) || 
    			   s.contains("text" + verse ) ){
    			ret.add(basePath + "/" + s);
    		}
    	}
    	return ret;
    }

    /// @deprecated
    /// Keeping it around for now because it has logic to 
    /// display all the verses on the same page
    private Vector<String> getAllHymnPieces_noJni( String hymn ){
    	File dir = new File(hymn);
    	String list[] = dir.list();
    	SortedSet<String> sortedSet = new TreeSet<String>();
    	Vector<String> ret = new Vector<String>();
    	String clef = "bass";
    	
    	for ( int i = 0; i < list.length; ++i ){
    		if ( list[i].startsWith(".") || list[i].contains ( "title" ) )
    			continue;
    		sortedSet.add(list[i]);
    		//Log.e("HymnalApp", "added: " + list[i]);
    	}
    	
    	int verse = 1;
    	while ( true ){
        	Iterator<String> it = sortedSet.iterator();
        	boolean hasVerse = false;
	    	while ( it.hasNext() ){
	    		String s = it.next();
	    		if ( s.contains("text" + verse) ){
	    			hasVerse = true;
	    			break;
	    		}
	    	}
	    	if ( !hasVerse )
	    		break;
	    	it = sortedSet.iterator();
	    	while ( it.hasNext() ){
	    		String s = it.next();
	    		if ( s.contains(clef) || 
	    			 ( s.contains("text" + verse ) )||
	    		     ( s.contains("title") && verse == 1 ) ){
	    			ret.add(s);
	    		}
	    	}
	    	++verse;
    	}
    	return ret;
    }
}
