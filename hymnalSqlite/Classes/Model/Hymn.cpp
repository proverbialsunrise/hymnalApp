/*
 *  Hymn.cpp
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-28.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Hymn.h"


static sqlite3_stmt *get_hymnInfo = 0;
static sqlite3_stmt *get_lyricImages = 0;
static sqlite3_stmt *get_musicImages = 0;

Hymn::Hymn() {
	//Don't use this constructor explicitly.  We don't want an empty hymn. 
}

Hymn::Hymn(const Hymn& h){
	hymnal = h.hymnal;
	hymnID = h.hymnID;
	hymnNumber = h.hymnNumber;
	
	author = h.author;
	composer = h.composer;
	copyrightInfo = h.copyrightInfo;
	firstLine = h.firstLine;
	metric = h.metric;
	section = h.section;
	subSection = h.subSection;
	subSubSection = h.subSubSection;
	title = h.title;
	translator = h.translator;
	tune = h.tune;
}

Hymn::Hymn(const int hymnID, const Hymnal& parentHymnal){
	this->hymnal = parentHymnal;
	this->hymnID = hymnID;
	sqlite3_bind_int(get_hymnInfo, 1, hymnID);
	if (sqlite3_step(get_hymnInfo) == SQLITE_ROW) {
		this->setHymnDetailsFromSqlRow(get_hymnInfo);		
	}
	sqlite3_reset(get_hymnInfo);
	//this->printDescription();
}



Hymn::~Hymn() {
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

void Hymn::prepareDatabaseStatements(sqlite3 *database){
	if (0 != database) {
		if (0 == get_hymnInfo) {
			const char *sql = "SELECT hymnNumber, author, composer, copyrightInfo, firstLine, metric, section, subSection, subSubSection, title, translator, tune FROM hymn WHERE hymnID = ?";
			if (sqlite3_prepare_v2(database, sql, -1, &get_hymnInfo, NULL) != SQLITE_OK) {
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

void Hymn::printDescription() const{
	printf("HymnID: %d\n", this->hymnID);
	printf("HymnNumber: %d\n", this->hymnNumber);
	printf("Author: %s\n", this->author.c_str());
	printf("Composer: %s\n", this->composer.c_str());
	printf("CopyrightInfo: %s\n", this->copyrightInfo.c_str());
	printf("FirstLine: %s\n", this->firstLine.c_str());
	printf("Metric: %s\n", this->metric.c_str());
	printf("Section: %s\n", this->section.c_str());
	printf("SubSection: %s\n", this->subSection.c_str());
	printf("SubSubSection: %s\n", this->subSubSection.c_str());
	printf("Title: %s\n", this->title.c_str());
	printf("Translator: %s\n", this->translator.c_str());
	printf("Tune: %s\n", this->tune.c_str());
}

int Hymn::get_hymnID() const{
	return hymnID;
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