package com.example.hymnal;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class HymnList extends ListActivity{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Bundle extras = getIntent().getExtras();
        final String hymnalName = extras.getString("HymnalName");
        setTitle(hymnalName);
        
        final Hymn[] hymns = getHymns ( 1 );//TODO: magic number
        final String[] hymnStrings = getHymnStrings ( hymns );
        
        setListAdapter(new ArrayAdapter<String>(this, R.layout.list_item, hymnStrings));

        ListView lv = getListView();
        lv.setTextFilterEnabled(true);

        lv.setOnItemClickListener(new OnItemClickListener() {
          public void onItemClick(AdapterView<?> parent, View view,
              int position, long id) {
	    	   Intent i = new Intent(HymnList.this, HymnView.class);
	    	   //TODO: pass the hymnID and a display settings parameter
	    	   i.putExtra ( "HymnName", ((TextView)view).getText() );
	    	   i.putExtra ( "HymnalName", hymnalName );
	    	   i.putExtra ( "HymnId", hymns[position].hymnId );//TODO: position == array index??
	    	   startActivity(i);
          }
        });
    }
	
	public native Hymn[] listHymns( int hymnalId );
	static {
		System.loadLibrary ( "Database-Jni" );
	}
	
	private String[] getHymnStrings ( Hymn[] hymns ){
    	String[] s = new String[hymns.length];
    	for ( int i = 0; i < hymns.length; ++i ){
    		s[i] = hymns[i].hymnNumber + " - " + hymns[i].hymnName;
    		Log.d ( "hymnal", "s[" + i + "]: " + s[i] );
    	}
		return s;
	}

    private Hymn[] getHymns ( int hymnalId ){
    	Hymn[] hymns = listHymns ( hymnalId );
    	return hymns;
    }
}