package com.example.hymnal;

import android.app.ListActivity;
import android.app.SearchManager;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class SearchActivity extends ListActivity {

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Intent intent = getIntent();
        Log.e("hymnal", "search activity activated");
        if (Intent.ACTION_SEARCH.equals(intent.getAction())) {
            String query = intent.getStringExtra(SearchManager.QUERY);
            setTitle("Search: \"" + query + "\"");
            Log.e("hymnal", "got query: " + query);
            if ( Hymnal.EMULATOR )
            	return;

            final Hymn[] results = search(1, query);//TODO: magic number
            String[] displayStrings = new String[results.length];
            for ( int i = 0; i < results.length; ++i ){
            	displayStrings[i] = results[i].hymnName;
            }
            setListAdapter(new ArrayAdapter<String>(this, R.layout.list_item, displayStrings));
            
            getListView().setOnItemClickListener(new OnItemClickListener() {
                public void onItemClick(AdapterView<?> parent, View view,
                        int position, long id) {
      	    	   Intent i = new Intent(SearchActivity.this, HymnView.class);
      	    	   i.putExtra ( "HymnName", ((TextView)view).getText() );
      	    	   i.putExtra ( "HymnalName", "Hymnal: A Worship Book" );//TODO: magic string
      	    	   i.putExtra ( "HymnId", results[position].hymnId );
      	    	   startActivity(i);
                }
            });
        }
    }
    
    public native Hymn[] search(int hymnalId, String query);
	static {
    	if ( !Hymnal.EMULATOR )
			System.loadLibrary ( "Database-Jni" );
	}

}
