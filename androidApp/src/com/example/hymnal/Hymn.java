package com.example.hymnal;

public class Hymn {
	public Hymn(){
		
	}
	
	public Hymn ( int hymnId, int hymnNumber, String hymnName ){
		this.hymnId = hymnId;
		this.hymnName = hymnName;
		this.hymnNumber = hymnNumber;
	}
	public int hymnId;
	public int hymnNumber;
	public String hymnName;
}
