package com.example.hymnal;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
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
        
        final String[] hymns = getHymns();
        
        setListAdapter(new ArrayAdapter<String>(this, R.layout.list_item, hymns));

        ListView lv = getListView();
        lv.setTextFilterEnabled(true);

        lv.setOnItemClickListener(new OnItemClickListener() {
          public void onItemClick(AdapterView<?> parent, View view,
              int position, long id) {
	    	   Intent i = new Intent(HymnList.this, Hymn.class);
	    	   //TODO: pass the hymnID and a display settings parameter
	    	   i.putExtra ( "HymnName", ((TextView)view).getText() );
	    	   i.putExtra ( "HymnalName", hymnalName );
	    	   startActivity(i);
          }
        });
    }

    private String[] getHymns(){
    	String[] a = {"43 - My Faith Has Found", "44 - The Love Of God"};
    	return a;
    }
}