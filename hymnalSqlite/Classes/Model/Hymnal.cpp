/*
 *  Hymnal.cpp
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-28.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Hymnal.h"


static sqlite3_stmt *get_hymnal_stmt = 0;
static unsigned int numInstances = 0;

Hymnal::Hymnal() {	
	//Don't use this constructor.
}


Hymnal::Hymnal(sqlite3 *db, int hID){
	this->database = db;
	if (numInstances == 0) {
		this->prepareDatabaseStatements();
	}
	sqlite3_bind_int(get_hymnal_stmt, 1, hID);
	this->hymnalID = hID;
	if (sqlite3_step(get_hymnal_stmt) == SQLITE_ROW) {
		//I'm not sure if this sort of cast will work well(at all) with special characters...
		this->title = (const char *)sqlite3_column_text(get_hymnal_stmt, 0);
		this->copyrightText = (const char *)sqlite3_column_text(get_hymnal_stmt, 1);
	} else {
		printf("Hymnal Not Found With ID: %d \n", this->hymnalID);
		throw 0;
	}
	sqlite3_reset(get_hymnal_stmt);
	numInstances ++;
}

Hymnal::~Hymnal(){
	numInstances --;
	if (numInstances == 0) {
		this->finalizeDatabaseStatements();
	}
}

void Hymnal::prepareDatabaseStatements(){
	const char *sql = "SELECT name, copyrightText FROM hymnal WHERE hymnalID = ?";
	if (sqlite3_prepare_v2(this->database, sql, -1, &get_hymnal_stmt, NULL) != SQLITE_OK) {
		printf("Problem preparing statement get_hymnal_stmt: %s\n", sqlite3_errmsg(this->database));
		//If we can't get the hymnal, we're toast...
		throw 0;
	}
}

void Hymnal::finalizeDatabaseStatements(){
	if (0 != get_hymnal_stmt) {
		sqlite3_finalize(get_hymnal_stmt);
		get_hymnal_stmt = 0;
	}
}

int Hymnal::get_hymnalID() const{
	return hymnalID;
}
std::string Hymnal::get_title() const{
	return title;
}
std::string Hymnal::get_copyrightText() const{
	return copyrightText;
}