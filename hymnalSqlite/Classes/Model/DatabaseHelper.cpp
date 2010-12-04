/*
 *  DatabaseHelper.cpp
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DatabaseHelper.h"

static sqlite3 *database = 0;

//When adding a sqlite statement, make sure to add it to finalizeDatabaseStatements() so that it is properly destroyed.
static sqlite3_stmt *get_allHymns_sort = 0;
static sqlite3_stmt *get_allHymnsForSection_sort = 0;

static sqlite3_stmt *get_hymnMusicSections = 0;

static sqlite3_stmt *get_hymnLyricsSections = 0;

#pragma mark Database Lifecycle
void openConnectionWithPath(std::string& databasePath){
	if (database == 0) {
		sqlite3_open(databasePath.c_str(), &database);
	} else {
		printf("Database is already open.\n");
	}

}

void closeConnection() {
	sqlite3_close(database);
	database = 0;
}

void finalizeDatabaseStatements(){
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
	
}

#pragma mark -

#pragma mark Hymn Display

HymnPieceVector getMusicPiecesForHymn(int hymnID, PartSpecifier part) {
	if (0 == get_hymnMusicSections) {
		const char *sql = "SELECT lineNumber, image, type FROM musicSection WHERE hymn = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnMusicSections, NULL) != SQLITE_OK) {
			printf("Problem preparing statements get_hymnMusicSectionsForPart: %s\n", sqlite3_errmsg(database));
			//We don't want to recover from this error...
			throw 0;
		}
	}
	
	HymnPieceVector hymnPieces;
	sqlite3_bind_int(get_hymnMusicSections, 1, hymnID);
	while (sqlite3_step(get_hymnMusicSections) == SQLITE_ROW) {
		
		//Don't do anything if the part is incorrect. 
		//If this turns out to be slow, we can add more SQL statements to make it faster TODO?
		//I don't think it will be a problem.
		if (part != ALLPARTS) {
			int p = sqlite3_column_int(get_hymnMusicSections, 2);
			if (p != part) {
				continue;
			}
		} else {
			int lineNumber = sqlite3_column_int(get_hymnMusicSections, 0);
			std::string imagePath = (const char *)sqlite3_column_text(get_hymnMusicSections, 1);
			HymnPiece piece = HymnPiece(lineNumber, imagePath);
			hymnPieces.push_back(piece);
		}
	}
	sqlite3_reset(get_hymnMusicSections);
	return hymnPieces;
}

HymnPieceVector getLyricPiecesForHymn(int hymnID, VerseSpecifier verse) {
	if (0 == get_hymnLyricsSections) {
		const char *sql = "SELECT lineNumber, image, type, verseNumber FROM lyricSection WHERE hymn = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnLyricsSections, NULL) != SQLITE_OK) {
			printf("Problem preparing statements get_hymnLyricsSections: %s\n", sqlite3_errmsg(database));
			//We don't want to recover from this error...
			throw 0;
		}
	}
	
	HymnPieceVector hymnPieces;
	sqlite3_bind_int(get_hymnMusicSections, 1, hymnID);
	while (sqlite3_step(get_hymnMusicSections) == SQLITE_ROW) {
		
		//Don't do anything if the part is incorrect. 
		//If this turns out to be slow, we can add more SQL statements to make it faster TODO?
		//I don't think it will be a problem. 
		if (verse != ALLVERSES) {
			int v = sqlite3_column_int(get_hymnMusicSections, 3);
			if (v != verse) {
				continue;
			}
			//what about the type?  Do we need to do something with the type? TODO
		} else {
			int lineNumber = sqlite3_column_int(get_hymnMusicSections, 0);
			std::string imagePath = (const char *)sqlite3_column_text(get_hymnMusicSections, 1);
			HymnPiece piece = HymnPiece(lineNumber, imagePath);
			hymnPieces.push_back(piece);
		}
	}
	sqlite3_reset(get_hymnMusicSections);
	return hymnPieces;
}

#pragma mark -

#pragma mark Getting Hymns

HymnVector getHymnsForHymnal(int hymnalID, HymnSort sortBy){
	Hymnal hymnal = Hymnal(database, hymnalID);
	
	if (0 == get_allHymns_sort) {
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
		Hymn hymn = Hymn(hymnID, hymnal, database);
		hymns.push_back(hymn);
	}
	return hymns;
}

#pragma mark -