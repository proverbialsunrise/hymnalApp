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
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class Hymn extends Activity{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.hymn);
        
        Bundle extras = getIntent().getExtras();
        String hymnalName = extras.getString("HymnalName");
        String hymnName = extras.getString("HymnName");
        setTitle(hymnName);

        ScrollView mainScrollView = (ScrollView) findViewById(R.id.hymn_scrollview);
        LinearLayout ll = (LinearLayout) mainScrollView.findViewById(R.id.next_level);
        ll.setPadding(0, 0, 0, 0);
        //ll.setBackgroundColor(0xffffffff);//TODO: keep??
        
        if ( Environment.MEDIA_MOUNTED.equals(Environment.MEDIA_MOUNTED) ||
             Environment.MEDIA_MOUNTED.equals(Environment.MEDIA_MOUNTED_READ_ONLY)	){
        	//good
        }else{
        	Log.e ( "hymnal", "external storage not there" );
        	return;
        }
        
        File hymnBaseDir = null;
        File externalStorage = Environment.getExternalStorageDirectory();
        File[] files = externalStorage.listFiles();
        if ( files == null ){
        	Log.e ( "hymnal", "external storage has no files" );
        	setTitle("error occurred");
        	return;
        }
        for ( int i = 0; i < files.length; ++i ){
        	if ( files[i].getName().equals("hymnalapp")){
        		hymnBaseDir = files[i];
        		break;
        	}
        }

        //TODO: query the database for all this info
        String hymnalDirName = hymnBaseDir + "/" + hymnalName.replace(":", "");
        File hymnalDir = new File ( hymnalDirName );
        files = hymnalDir.listFiles();
        if ( files == null ){
        	Log.e ( "hymnal", "failed getting hymnal dir" );
        	setTitle("error occurred");
        	return;
        }
        int hymnNumber = Integer.parseInt ( hymnName.substring(0, 2) );
        String hymn = hymnalDirName + "/";
        for ( int i=0; i < files.length; ++i ){
            int tmpNumber = Integer.parseInt ( files[i].getName().substring(0, 3) );
        	if ( tmpNumber == hymnNumber ){
        		hymn += files[i].getName();
        		break;
        	}
        }

        Vector<String> pieces = getHymnPieces(hymn);
        TextView filler = new TextView(getApplicationContext());
        filler.setHeight(5);
        filler.setBackgroundColor(0xffffffff);
        ll.addView( filler );
        for ( int i = 0; i < pieces.size(); ++i ){
        	ImageView image = new ImageView(getApplicationContext());
        	String path = hymn + "/" + pieces.get(i);
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
    
    //TODO: jni database access implementation
    private Vector<String> getHymnPieces( String hymn ){
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
