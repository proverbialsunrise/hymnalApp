package com.example.hymnal;

import android.app.ListActivity;
import android.app.SearchManager;
import android.content.Intent;
//import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.view.*;
import android.widget.ArrayAdapter;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;

public class Hymnal extends ListActivity{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Intent intent = getIntent();
        if (Intent.ACTION_SEARCH.equals(intent.getAction())) {
            // handles a search query
            String query = intent.getStringExtra(SearchManager.QUERY);
            Log.e ( "hymnal", "got query: " + query );
            //showResults(query);
            return;
        }

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
	
	public native String[] listHymns();
	static {
		System.loadLibrary ( "Database-Jni" );
	}

    private String[] getHymnals(){
    	String[] a = listHymns();
    	//TODO: don't hardcode path
    	//SQLiteDatabase db = openDatabase ( "/sdcard/hymnalapp/database",  );
    	Log.e("Hymnal", a[0]);
    	return a;
    }
}
