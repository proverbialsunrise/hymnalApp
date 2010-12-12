/**
 * Receives query events as the user inputs text into the
 * search box.
 */

package com.example.hymnal;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.util.Log;

public class SearchProvider extends ContentProvider {

	public boolean onCreate() {
	    Log.i ( "hymnal", "SearchProvider created" );
		return true;
	}

	@Override
	public Cursor query(Uri uri, String[] projection, String selection,
			String[] selectionArgs, String sortOrder) {
		if ( projection == null || selectionArgs == null ){
			Log.e("hymnal", "null projection");
		}else{
			for ( int i = 0; i < projection.length; ++i ){
				Log.e("hymnal", "projection: " + projection[i]);
			}
		}
		
		if ( selectionArgs == null ){
			Log.e("hymnal", "null selectionArgs" );
		}else{
			for ( int i = 0; i < selectionArgs.length; ++i ){
				Log.e("hymnal", "selectionArg: " + selectionArgs[i]);
			}
		}
		Log.e("hymnal", "selection: '" + selection + "'");
		Log.e("hymnal", "uri: " + uri);
		Log.e("hymnal", "sortOrder:" + sortOrder);
		return null;
	}
	
	//None of the rest of these functions are currently used.
	
	@Override
	public int delete(Uri uri, String selection, String[] selectionArgs) {
		Log.e("hymnal", "delete");
		return 0;
	}

	@Override
	public String getType(Uri uri) {
		Log.e("hymnal", "getType");
		return null;
	}

	@Override
	public Uri insert(Uri uri, ContentValues values) {
		Log.e("hymnal", "insert");
		return null;
	}

	@Override
	public int update(Uri uri, ContentValues values, String selection,
			String[] selectionArgs) {
		Log.e("hymnal", "update");
		return 0;
	}

}
