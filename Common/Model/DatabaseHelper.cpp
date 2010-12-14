/*
 *  DatabaseHelper.cpp
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DatabaseHelper.h"

void finalizeDatabaseStatements();

static sqlite3 *database = 0;

//When adding a sqlite statement, make sure to add it to finalizeDatabaseStatements() so that it is properly destroyed.
static sqlite3_stmt *get_allHymns_sort = 0;
static sqlite3_stmt *get_allHymnsForSection_sort = 0;

static sqlite3_stmt *get_hymnMusicSections = 0;
static sqlite3_stmt *get_hymnLyricsSections = 0;
static sqlite3_stmt *get_hymnSections = 0;

static sqlite3_stmt *get_numVersesForHymn = 0;

#pragma mark Database Lifecycle
void openConnectionWithPath(std::string& databasePath){
	if (database == 0) {
		sqlite3_open(databasePath.c_str(), &database);
		Hymnal::prepareDatabaseStatements(database);
		Hymn::prepareDatabaseStatements(database);
	} else {
		printf("Database is already open.\n");
	}
}

void tsOpenConnectionWithPath(std::string& databasePath, sqlite3** ret_dbPointer, sqlite3_stmt** ret_get_hymnal_stmt, sqlite3_stmt** ret_get_hymn_stmt){
	if (database == 0) {
		sqlite3_open(databasePath.c_str(), ret_dbPointer);
		Hymnal::tsPrepareDatabaseStatements(*ret_dbPointer, ret_get_hymnal_stmt);
		Hymn::tsPrepareDatabaseStatements(*ret_dbPointer, ret_get_hymn_stmt);
	} else {
		printf("Database is already open.\n");
	}	
}

void closeConnection() {
	finalizeDatabaseStatements();
	sqlite3_close(database);
	database = 0;
}

void tsCloseConnection(sqlite3* db, sqlite3_stmt* get_hymnal_stm, sqlite3_stmt* get_hymn_stm){
	sqlite3_finalize(get_hymnal_stm);
	sqlite3_finalize(get_hymn_stm);
	sqlite3_close(db);
}

void finalizeDatabaseStatements(){
	Hymnal::finalizeDatabaseStatements();
	Hymn::finalizeDatabaseStatements();
	if (0 != get_allHymns_sort) {
		sqlite3_finalize(get_allHymns_sort);
		get_allHymns_sort = 0;
	}
	if (0 != get_allHymnsForSection_sort) {
		sqlite3_finalize(get_allHymnsForSection_sort);
		get_allHymnsForSection_sort = 0;
	}

	if (0 != get_hymnMusicSections) {
		sqlite3_finalize(get_hymnMusicSections);
		get_hymnMusicSections = 0;
	}
	if (0 != get_hymnLyricsSections) {
		sqlite3_finalize(get_hymnLyricsSections);
		get_hymnLyricsSections = 0;
	}
	if (0 != get_hymnSections) {
		sqlite3_finalize(get_hymnSections);
		get_hymnSections = 0;
	}
	
}

#pragma mark -

Hymnal getHymnal(int hymnalID){
	return Hymnal(hymnalID);
}

Hymnal tsGetHymnal(int hymnalID, sqlite3_stmt* get_hymnal){
	return Hymnal(hymnalID, get_hymnal);
}

#pragma mark Hymn Display

HymnSectionVector getMusicPiecesForHymn(int hymnID, PartSpecifier part) {
	if (0 == get_hymnMusicSections) {
		const char *sql = "SELECT lineNumber, image, type FROM musicSection WHERE hymn = ? AND (type LIKE ? OR type = 0) ORDER BY lineNumber";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnMusicSections, NULL) != SQLITE_OK) {
			printf("Problem preparing statements get_hymnMusicSectionsForPart: %s\n", sqlite3_errmsg(database));
			//We don't want to recover from this error...
			throw 0;
		}
	}
	
	HymnSectionVector hymnPieces;
	sqlite3_bind_int(get_hymnMusicSections, 1, hymnID);
	
	//This will break if there is a hymn with more than 9 verses...
	char partChar[2];
	memset(partChar, 0, 2);
	if (part == ALLPARTS) {
		snprintf(partChar, 2, "%%");
	} else {
		snprintf(partChar, 2, "%d", part);
	}

	sqlite3_bind_text(get_hymnMusicSections, 2, partChar, strlen(partChar), SQLITE_TRANSIENT);

	while (sqlite3_step(get_hymnMusicSections) == SQLITE_ROW) {
		HymnSection piece;
		piece.lineNumber = sqlite3_column_int(get_hymnMusicSections, 0);
		piece.imagePath = (const char *)sqlite3_column_text(get_hymnMusicSections, 1);
		piece.part = sqlite3_column_int(get_hymnMusicSections, 2);
		hymnPieces.push_back(piece);
	}
	sqlite3_reset(get_hymnMusicSections);
	return hymnPieces;
}

HymnSectionVector getLyricPiecesForHymn(int hymnID, int verse, PartSpecifier part) {
	if (0 == get_hymnLyricsSections) {
		const char *sql = "SELECT lineNumber, image, type, verseNumber FROM lyricSection WHERE hymn = ? and (verseNumber LIKE ? OR verseNumber = -1) AND (type LIKE ? OR type = 0) ORDER BY lineNumber";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnLyricsSections, NULL) != SQLITE_OK) {
			printf("Problem preparing statements get_hymnLyricsSections: %s\n", sqlite3_errmsg(database));
			//We don't want to recover from this error...
			throw 0;
		}
	}
	
	HymnSectionVector hymnPieces;
	sqlite3_bind_int(get_hymnLyricsSections, 1, hymnID);
	
	char verseChar[2];
	memset(verseChar, 0, 2);
	if (verse == ALLVERSES) {
		snprintf(verseChar, 2, "%%");
	} else {
		snprintf(verseChar, 2, "%d", verse);
	}
	sqlite3_bind_text(get_hymnLyricsSections, 2, verseChar, strlen(verseChar), SQLITE_TRANSIENT);

	char partChar[2];
	memset(partChar, 0, 2);
	if (part == ALLPARTS) {
		snprintf(partChar, 2, "%%");
	} else {
		snprintf(partChar, 2, "%d", part);
	}
	sqlite3_bind_text(get_hymnLyricsSections, 3, partChar, strlen(partChar), SQLITE_TRANSIENT);

	while (sqlite3_step(get_hymnLyricsSections) == SQLITE_ROW) {
			HymnSection piece;
			piece.lineNumber = sqlite3_column_int(get_hymnLyricsSections, 0);
			piece.imagePath = (const char *)sqlite3_column_text(get_hymnLyricsSections, 1);
			piece.part = sqlite3_column_int(get_hymnLyricsSections, 2);
			hymnPieces.push_back(piece);
	}
	sqlite3_reset(get_hymnLyricsSections);
	return hymnPieces;
}

HymnSectionVector getPiecesForHymn(int hymnID, int verse, PartSpecifier part){
	if (0 == get_hymnSections) {
		const char *sql = "SELECT lineNumber, image, type FROM musicSection WHERE hymn = ? AND (type LIKE ? OR type = 0) UNION SELECT lineNumber, image, type FROM lyricSection WHERE hymn = ? and (verseNumber LIKE ? OR verseNumber = -1) AND (type LIKE ? OR type = 0) ORDER BY lineNumber";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnSections, NULL) != SQLITE_OK) {
			printf("Problem preparing statement get_hymnSections: %s", sqlite3_errmsg(database));
		}
	}
	
	sqlite3_bind_int(get_hymnSections, 1, hymnID);
	sqlite3_bind_int(get_hymnSections, 3, hymnID);
	
	char verseChar[2];
	memset(verseChar, 0, 2);
	if (verse == ALLVERSES) {
		snprintf(verseChar, 2, "%%");
	} else {
		snprintf(verseChar, 2, "%d", verse);
	}
	sqlite3_bind_text(get_hymnSections, 4, verseChar, strlen(verseChar), SQLITE_TRANSIENT);
	
	char partChar[2];
	memset(partChar, 0, 2);
	if (part == ALLPARTS) {
		snprintf(partChar, 2, "%%");
	} else {
		snprintf(partChar, 2, "%d", part);
	}
	sqlite3_bind_text(get_hymnSections, 2, partChar, strlen(partChar), SQLITE_TRANSIENT);
	sqlite3_bind_text(get_hymnSections, 5, partChar, strlen(partChar), SQLITE_TRANSIENT);
	
	HymnSectionVector hymnPieces;
	
	while (sqlite3_step(get_hymnSections) == SQLITE_ROW) {
		HymnSection piece;
		piece.lineNumber = sqlite3_column_int(get_hymnSections, 0);
		piece.imagePath = (const char *)sqlite3_column_text(get_hymnSections, 1);
		piece.part = sqlite3_column_int(get_hymnSections, 2);
		hymnPieces.push_back(piece);
	}
	sqlite3_reset(get_hymnSections);
	return hymnPieces;
	
}

unsigned int numVersesForHymn(int hymnID) {
	if (0 == get_numVersesForHymn) {
		const char * sql = "SELECT MAX(verseNumber) FROM lyricSection WHERE hymn = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_numVersesForHymn, NULL)) {
			printf("Problem preparing statement get_numVerses: %s", sqlite3_errmsg(database));
		}
	}
	
	int numVerses = 0;
	sqlite3_bind_int ( get_numVersesForHymn, 1, hymnID );
	while ( sqlite3_step ( get_numVersesForHymn ) == SQLITE_ROW ) {
		numVerses = sqlite3_column_int(get_numVersesForHymn, 0);
	}
	sqlite3_reset(get_numVersesForHymn);
	return numVerses;
}
#pragma mark -

#pragma mark Getting Hymns

HymnVector getHymnsForHymnal(int hymnalID, HymnSort sortBy){
	Hymnal hymnal = Hymnal(hymnalID);
	
	if (0 == get_allHymns_sort) {
		//I don't know if you can put a wild card into an ORDER BY like this.  We may need two separate statements.
		const char *sql = "SELECT hymnID, hymnNumber, title FROM hymn WHERE hymnal = ? ORDER BY ? ASC";
		if (sqlite3_prepare_v2(database, sql, -1, &get_allHymns_sort, NULL) != SQLITE_OK) {
			printf("Problem preparing get_allHymns_sort: %s", sqlite3_errmsg(database));
		}
	}
	
	sqlite3_bind_int(get_allHymns_sort, 1, hymnalID);
	sqlite3_bind_int(get_allHymns_sort, 2, sortBy);
	
	HymnVector hymns;
	while (sqlite3_step(get_allHymns_sort) == SQLITE_ROW) {
		//We could do one less query here if performance is an issue. The code is cleaner this way. TODO?
		int hymnID = sqlite3_column_int(get_allHymns_sort, 0);
		Hymn hymn = Hymn(hymnID, hymnal);
		hymns.push_back(hymn);
	}
	sqlite3_reset(get_allHymns_sort);
	return hymns;
}

#pragma mark -