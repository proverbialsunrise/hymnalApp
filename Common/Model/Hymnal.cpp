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

Hymnal::Hymnal() {	
	//Don't use this constructor explicitly.
}

Hymnal::Hymnal(const Hymnal& h) {
	hymnalID = h.hymnalID;
	title = h.title;
	copyrightText = h.copyrightText;
}


Hymnal::Hymnal(int hID){
	sqlite3_bind_int(get_hymnal_stmt, 1, hID);
	this->hymnalID = hID;
	if (sqlite3_step(get_hymnal_stmt) == SQLITE_ROW) {
		this->setHymnalDetailsFromSqliteRow(get_hymnal_stmt);
	} else {
		printf("Hymnal Not Found With ID: %d \n", this->hymnalID);
		throw 0;
	}
	sqlite3_reset(get_hymnal_stmt);
}

Hymnal::Hymnal(int hID, sqlite3_stmt* get_hymnal){
	sqlite3_bind_int(get_hymnal, 1, hID);
	this->hymnalID = hID;
	if (sqlite3_step(get_hymnal) == SQLITE_ROW) {
		this->setHymnalDetailsFromSqliteRow(get_hymnal);
	} else {
		printf("Hymnal Not Found threadsafe With ID: %d \n", this->hymnalID);
		throw 0;
	}
	sqlite3_reset(get_hymnal);
	
}

void Hymnal::setHymnalDetailsFromSqliteRow(sqlite3_stmt *row) {
	//I'm not sure if this sort of cast will work well(at all) with special characters...
	this->title = (const char *)sqlite3_column_text(row, 0);
	this->copyrightText = (const char *)sqlite3_column_text(row, 1);
	this->shortName = (const char *)sqlite3_column_text(row, 2);
}

Hymnal::~Hymnal(){

}

void Hymnal::prepareDatabaseStatements(sqlite3 *database){
	const char *sql = "SELECT name, copyrightText, shortName FROM hymnal WHERE hymnalID = ?";
	if (sqlite3_prepare_v2(database, sql, -1, &get_hymnal_stmt, NULL) != SQLITE_OK) {
		printf("Problem preparing statement get_hymnal_stmt: %s\n", sqlite3_errmsg(database));
		//If we can't get the hymnal, we're toast...
		throw 0;
	}
}

void Hymnal::tsPrepareDatabaseStatements(sqlite3 *database, sqlite3_stmt** ret_get_hymnal_stmt) {
	const char *sql = "SELECT name, copyrightText, shortName FROM hymnal WHERE hymnalID = ?";
	if (sqlite3_prepare_v2(database, sql, -1, ret_get_hymnal_stmt, NULL) != SQLITE_OK) {
		printf("Problem preparing threadsafe statement get_hymnal_stmt: %s\n", sqlite3_errmsg(database));
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
std::string Hymnal::get_shortName() const{
	return shortName;
}