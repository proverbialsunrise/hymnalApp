/*
 *  DatabaseHelper.cpp
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DatabaseHelper.h"

//When adding a sqlite statement, make sure to add it to finalizeDatabaseStatements() so that it is properly destroyed.
static sqlite3_stmt *get_allHymns_sort = 0;
static sqlite3_stmt *get_allHymnsForSection_sort = 0;

static sqlite3_stmt *get_hymnMusicSections = 0;

static sqlite3_stmt *get_hymnLyricsSections = 0;



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


hymnPieceVector getMusicPiecesForHymn(int hymnID, PartSpecifier part, sqlite3* database) {
	if (0 == get_hymnMusicSections) {
		const char *sql = "SELECT lineNumber, image, type FROM musicSection WHERE hymn = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnMusicSections, NULL) != SQLITE_OK) {
			printf("Problem preparing statements get_hymnMusicSectionsForPart: %s\n", sqlite3_errmsg(database));
			//We don't want to recover from this error...
			throw 0;
		}
	}
	
	hymnPieceVector hymnPieces;
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
			hymnPiece piece = hymnPiece(lineNumber, imagePath);
			hymnPieces.push_back(piece);
		}
	}
	sqlite3_reset(get_hymnMusicSections);
	return hymnPieces;
}

hymnPieceVector getLyricPiecesForHymn(int hymnID, VerseSpecifier verse, sqlite3* database) {
	if (0 == get_hymnLyricsSections) {
		const char *sql = "SELECT lineNumber, image, type, verseNumber FROM lyricSection WHERE hymn = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnLyricsSections, NULL) != SQLITE_OK) {
			printf("Problem preparing statements get_hymnLyricsSections: %s\n", sqlite3_errmsg(database));
			//We don't want to recover from this error...
			throw 0;
		}
	}
	
	hymnPieceVector hymnPieces;
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
			hymnPiece piece = hymnPiece(lineNumber, imagePath);
			hymnPieces.push_back(piece);
		}
	}
	sqlite3_reset(get_hymnMusicSections);
	return hymnPieces;
}
