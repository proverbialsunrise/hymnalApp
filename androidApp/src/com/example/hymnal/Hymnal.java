package com.example.hymnal;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.*;
import android.widget.ArrayAdapter;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;

public class Hymnal extends ListActivity{
	static boolean EMULATOR = true;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final String[] hymnals = getHymnals();
        setListAdapter(new ArrayAdapter<String>(this, R.layout.list_item, hymnals));

        ListView lv = getListView();
        lv.setTextFilterEnabled ( true );

        lv.setOnItemClickListener(new OnItemClickListener() {
          public void onItemClick (	AdapterView<?> parent, View view,
        		  					int position, long id ) {
	    	   Intent i = new Intent(Hymnal.this, HymnList.class);
	    	   i.putExtra ( "HymnalName", ((TextView)view).getText() );
	    	   startActivity(i);
          }
        });
    }
	
	public native String[] listHymnals();
	static {
    	if ( "sdk".equals( Build.PRODUCT ) )
			EMULATOR = true;
    	else
    		Log.e ( "hymnal", "Product: " + Build.PRODUCT );
		if ( !EMULATOR )
			System.loadLibrary ( "Database-Jni" );
	}

    private String[] getHymnals(){
    	String[] a = null;
    	if( !EMULATOR ){
    		a = listHymnals();
    	}else{
    		a = new String[]{"Hymnal: A Worship Book"};
		}
    	Log.e("Hymnal", a[0]);
    	return a;
    }
}
