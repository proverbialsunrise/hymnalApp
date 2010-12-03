/*
 *  Hymn.cpp
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-28.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Hymn.h"

static unsigned int numInstances = 0;
//counting instances like this is not safe for multiple threads.  Be wary.

static sqlite3_stmt *get_hymnInfo = 0;
static sqlite3_stmt *get_lyricImages = 0;
static sqlite3_stmt *get_musicImages = 0;

Hymn::Hymn() {
	//Don't use this constructor.  We don't want an empty hymn. 
	throw 0;
}

Hymn::Hymn(const int hymnID, const Hymnal& parentHymnal, sqlite3 *db){
	this->database = db;
	if (0 == numInstances) {
		this->prepareDatabaseStatements();
	}
	this->hymnal = parentHymnal;
	this->hymnID = hymnID;
	sqlite3_bind_int(get_hymnInfo, 1, hymnID);
	if (sqlite3_step(get_hymnInfo) == SQLITE_ROW) {
		this->setHymnDetailsFromSqlRow(get_hymnInfo);		
	}
	sqlite3_reset(get_hymnInfo);
	numInstances ++;
}

Hymn::~Hymn() {
	numInstances --;
	if (0 == numInstances) {
		this->finalizeDatabaseStatements();
	}
}

void Hymn::setHymnDetailsFromSqlRow(sqlite3_stmt *row){
	this->hymnNumber = sqlite3_column_int(row, 0);
	//I'm not sure if this sort of cast will work well for special characters.  We'll have to try it and see. 
	this->author = (const char *)sqlite3_column_text(row, 1);
	this->composer = (const char *)sqlite3_column_text(row, 2);
	this->copyrightInfo = (const char *)sqlite3_column_text(row, 3);
	this->firstLine = (const char *)sqlite3_column_text(row, 4);
	this->metric = (const char *)sqlite3_column_text(row, 5);
	this->section = (const char *)sqlite3_column_text(row, 6);
	this->subSection = (const char *)sqlite3_column_text(row, 7);
	this->subSubSection = (const char *)sqlite3_column_text(row, 8);
	this->title = (const char *)sqlite3_column_text(row, 9);
	this->translator = (const char *)sqlite3_column_text(row, 10);
	this->tune = (const char *)sqlite3_column_text(row, 11);
}

void Hymn::prepareDatabaseStatements(){
	if (0 != this->database) {
		if (0 == get_hymnInfo) {
			const char *sql = "SELECT hymnNumber, author, composer, copyrightInfo, firstLine, metric, section, subSection, subSubSection, title, translator, tune FROM hymn WHERE hymnID = ?";
			if (sqlite3_prepare_v2(this->database, sql, -1, &get_hymnInfo, NULL) != SQLITE_OK) {
				printf("Problem preparing statement get_hymnInfo: %s \n", sqlite3_errmsg(database));
			}
		}
		if (0 == get_lyricImages) {
			//TODO
		}
		
		if (0 == get_musicImages) {
			//TODO
		}
	}
}

void Hymn::finalizeDatabaseStatements(){
	if (0 != get_hymnInfo) {
		sqlite3_finalize(get_hymnInfo);
		get_hymnInfo = 0;
	}
	if (0 != get_lyricImages) {
		sqlite3_finalize(get_lyricImages);
		get_lyricImages = 0;
	}
	if (0 != get_musicImages) {
		sqlite3_finalize(get_musicImages);
		get_musicImages = 0;
	}
}


int Hymn::get_hymnNumber() const{
	return hymnNumber;
}
std::string Hymn::get_author() const{
	return author;
}
std::string Hymn::get_composer() const{
	return composer;
}
std::string Hymn::get_copyrightInfo() const{
	return copyrightInfo;
}
std::string Hymn::get_firstLine() const{
	return firstLine;
}
std::string Hymn::get_metric() const{
	return metric;
}
std::string Hymn::get_section() const{
	return section;
}
std::string Hymn::get_subSection() const{
	return subSection;
}
std::string Hymn::get_subSubSection() const{
	return subSubSection;
}
std::string Hymn::get_title() const{
	return title;
}
std::string Hymn::get_translator() const{
	return translator;
}
std::string Hymn::get_tune() const{
	return tune;
}