package com.example.hymnal;

import java.io.File;
import java.util.Iterator;
import java.util.Map;
import java.util.SortedSet;
import java.util.TreeSet;
import java.util.Vector;

import android.app.Activity;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Gallery;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.BaseAdapter;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class HymnView extends Activity {
	int currentVerse = 0;
	int hymnId = -1;
	int numVerses = 0;
	static int part = 0;
	File hymnalBaseDir = null;
	VerseAdapter va = null;

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
        	Log.e ( "hymnal", "failed getting hymnal dir: '" + hymnalDirName + "'" );
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

        Gallery gallery = (Gallery) findViewById(R.id.hymn_gallery);
        va = new VerseAdapter(this);
        gallery.setAdapter ( va );
        gallery.setBackgroundColor(0xffffffff);
    }
    
    @Override
    /*
     * Individually free the memory for each image
     */
    public void onPause(){
    	super.onPause();
    	Gallery gallery = (Gallery) findViewById(R.id.hymn_gallery);
    	for ( int i = 0; i < gallery.getChildCount(); ++i ){
    		ScrollView gfsv = (ScrollView) gallery.getChildAt(i);
    		LinearLayout ll = (LinearLayout) gfsv.getChildAt(0);
    		for ( int j = 0; j < ll.getChildCount(); ++j ){
    			View iv = ll.getChildAt(j);
    			if ( iv instanceof ImageView )
    				((ImageView)iv).setImageBitmap(null);
    		}
    	}
    	System.gc();
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
    	Gallery gallery = (Gallery) findViewById(R.id.hymn_gallery);
    	int position = gallery.getFirstVisiblePosition();
    	int last = gallery.getLastVisiblePosition();
    	while ( position <= last ){
	    	ScrollView sv = (ScrollView) gallery.getChildAt(position);
	    	if ( sv == null ){
	    		++position;
	    		continue;
	    	}
	    	sv.removeAllViews();
	    	va.fillScrollView(sv, position + 1);
	    	++position;
    	}
    	return true;
    }
    
    class VerseAdapter extends BaseAdapter {
    	private Context mContext;
    	int mGalleryItemBackground;
    	Map<Integer, View> views;
    	
    	public VerseAdapter ( Context c ){
    		mContext = c;
            TypedArray a = obtainStyledAttributes(R.styleable.HelloGallery);
            mGalleryItemBackground = a.getResourceId(
                    R.styleable.HelloGallery_android_galleryItemBackground, 0);
            a.recycle();
    	}
    	public int getCount() {
    		return numVerses;
    	}
    	public Object getItem( int position ){
    		return position;
    	}
    	public long getItemId ( int position ){
    		return position;
    	}
    	public View getView ( int position, View convertView, ViewGroup parent ){
            GalleryFriendlyScrollView sv = new GalleryFriendlyScrollView(mContext);
    		fillScrollView(sv, position+1);
            return sv;
    	}
    	public void fillScrollView ( ScrollView sv, int verse ){
            Vector<String> pieces = getHymnPieces(hymnId, part, verse); //no verse zero
    		LinearLayout ll = new LinearLayout(mContext);
    		ll.setOrientation(LinearLayout.VERTICAL);
    		TextView filler = new TextView(mContext);
            filler.setHeight(5);
            filler.setBackgroundColor(0xffffffff);
            ll.addView( filler );
        	DisplayMetrics dm = new DisplayMetrics();
        	getWindowManager().getDefaultDisplay().getMetrics(dm);
        	int width = dm.widthPixels;
            for ( int i = 0; i < pieces.size(); ++i ){
            	ImageView image = new ImageView(mContext);
            	String path = hymnalBaseDir + "/" + pieces.get(i);
            	//Log.e ( "hymnal", "path: " + path );
            	Bitmap bm = null;
            	try{
            		bm = BitmapFactory.decodeFile(path);
            	} catch ( Exception ex ){
            		Log.e ( "hymnal", "error: " + ex );
            	}
            	image.setImageBitmap(bm);
            	image.setScaleType(ScaleType.FIT_XY);
            	int height = width * bm.getHeight() / bm.getWidth();
            	image.setLayoutParams(new LinearLayout.LayoutParams(width, height));
            	if ( pieces.get(i).contains("bass") ){
            		image.setPadding(0, 0, 0, 5);
            	}else{
            		image.setPadding(0, 0, 0, 0);
            	}
            	ll.addView(image);
            }
            ll.setBackgroundColor(0xffffffff);
            sv.addView(ll);
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

    private Vector<String> getHymnPieces ( int hymnId, int partSpecifier, int verse ){
    	if ( Hymnal.EMULATOR ){
    		return getHymnPieces_noJni(hymnId, partSpecifier, verse);
    	}
    	String[] pieces = getSections ( hymnId, verse, partSpecifier );
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
    private Vector<String> getHymnPieces_noJni ( int hymnId, int partSpecifier, int verse ){
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
