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

#define MAX_RECENTS 2

static sqlite3 *database = 0;

//When adding a sqlite statement, make sure to add it to finalizeDatabaseStatements() so that it is properly destroyed.
static sqlite3_stmt *get_allHymns_sort = 0;
static sqlite3_stmt *get_allHymnsForSection_sort = 0;
static sqlite3_stmt *get_favouriteHymns_sort = 0;

static sqlite3_stmt *get_hymnMusicSections = 0;
static sqlite3_stmt *get_hymnLyricsSections = 0;
static sqlite3_stmt *get_hymnSections = 0;

static sqlite3_stmt *get_numVersesForHymn = 0;

//Search statements.
static sqlite3_stmt *get_hymnsForNumericSearch = 0;
static sqlite3_stmt *get_hymnsForTitleSearch = 0;
static sqlite3_stmt *get_recents = 0;

//Writing to database
static sqlite3_stmt *set_hymnFavouriteStatus = 0;

//Recents statements
static sqlite3_stmt *insert_hymnIntoRecents = 0;
static sqlite3_stmt *count_entries_in_recents = 0;
static sqlite3_stmt *get_oldest_entry_from_recents = 0;
static sqlite3_stmt *delete_entry_from_recents = 0;

#pragma mark Database Lifecycle
void openConnectionWithPath(std::string& databasePath){
	if (database == 0) {
		printf("Database: %s\n", databasePath.c_str());
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
	if (0 != get_favouriteHymns_sort) {
		sqlite3_finalize(get_favouriteHymns_sort);
		get_favouriteHymns_sort = 0;
	}
	if (0 != get_hymnsForNumericSearch) {
		sqlite3_finalize(get_hymnsForNumericSearch);
		get_hymnsForNumericSearch = 0;
	}
	if (0 != get_hymnsForTitleSearch) {
		sqlite3_finalize(get_hymnsForTitleSearch);
		get_hymnsForTitleSearch = 0;
	}
	if (0 != set_hymnFavouriteStatus) {
		sqlite3_finalize(set_hymnFavouriteStatus);
		set_hymnFavouriteStatus = 0;
	}
	if (0 != insert_hymnIntoRecents) {
		sqlite3_finalize(insert_hymnIntoRecents);
		insert_hymnIntoRecents = 0;
	}
	if (0 != count_entries_in_recents) {
		sqlite3_finalize(count_entries_in_recents);
		count_entries_in_recents = 0;
	}
	if (0 != get_oldest_entry_from_recents) {
		sqlite3_finalize(get_oldest_entry_from_recents);
		get_oldest_entry_from_recents = 0;
	}
	if (0 != delete_entry_from_recents) {
		sqlite3_finalize(delete_entry_from_recents);
		delete_entry_from_recents = 0;
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

unsigned int getNumVersesForHymn(int hymnID) {
	if (0 == get_numVersesForHymn) {
		const char * sql = "SELECT MAX(verseNumber) FROM lyricSection WHERE hymn = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_numVersesForHymn, NULL)) {
			printf("Problem preparing statement get_numVerses: %s", sqlite3_errmsg(database));
		}
	}
	unsigned int numVerses = 0;
	sqlite3_bind_int(get_numVersesForHymn, 1, hymnID);
	if (sqlite3_step(get_numVersesForHymn) == SQLITE_ROW) {
		numVerses = sqlite3_column_int(get_numVersesForHymn, 0);
	}
	if (numVerses == -1) { //All hymn parts are for ALLVERSES.
		numVerses = 1;
	}
	sqlite3_reset(get_numVersesForHymn);
	return numVerses;
}
#pragma mark -

#pragma mark Getting Hymns

HymnVector getHymnsForHymnal(int hymnalID, HymnSort sortBy){
	Hymnal hymnal = Hymnal(hymnalID);
	
	if (0 == get_allHymns_sort) {
		const char *sql = "SELECT hymnID, hymnNumber, title FROM hymn WHERE hymnal = ? ORDER BY CASE ? WHEN 1 THEN hymnID WHEN 2 THEN hymnNumber WHEN 3 THEN title END ASC";
		if (sqlite3_prepare_v2(database, sql, -1, &get_allHymns_sort, NULL) != SQLITE_OK) {
			printf("Problem preparing get_allHymns_sort: %s\n", sqlite3_errmsg(database));
		}
	}
	
	if (sqlite3_bind_int(get_allHymns_sort, 1, hymnalID) != SQLITE_OK){
		printf("Problem binding to 1 of get_allHymns_sort.\n");
	}
	
	 if (sqlite3_bind_int(get_allHymns_sort, 2, sortBy) != SQLITE_OK){
		printf("Problem binding to 2 of get_allHymns_sort.\n");
	} 
	
	HymnVector hymns;
	while (sqlite3_step(get_allHymns_sort) == SQLITE_ROW) {
		int hymnID = sqlite3_column_int(get_allHymns_sort, 0);
		Hymn hymn = Hymn(hymnID, hymnal);
		hymns.push_back(hymn);
	}
	sqlite3_reset(get_allHymns_sort);
	return hymns;
}

HymnVector getFavouriteHymnsForHymnal(int hymnalID, HymnSort sortBy) {
	Hymnal hymnal = Hymnal(hymnalID);
	if (0 == get_favouriteHymns_sort) {
		const char *sql = "SELECT hymnID, hymnNumber, title FROM hymn WHERE hymnal = ? AND favourite = 1 ORDER BY CASE ? WHEN 1 THEN hymnID WHEN 2 THEN hymnNumber WHEN 3 THEN title END ASC";
		if (sqlite3_prepare_v2(database, sql, -1, &get_favouriteHymns_sort, NULL) != SQLITE_OK) {
			printf("Problem preparing get_favouriteHymns_sort: %s\n", sqlite3_errmsg(database));
		}
	}
	
	if (sqlite3_bind_int(get_favouriteHymns_sort, 1, hymnalID) != SQLITE_OK){
		printf("Problem binding to 1 of get_favouriteHymns_sort.\n");
	}
	
	if (sqlite3_bind_int(get_favouriteHymns_sort, 2, sortBy) != SQLITE_OK){
		printf("Problem binding to 2 of get_favouriteHymns_sort.\n");
	} 
	HymnVector hymns;
	while (sqlite3_step(get_favouriteHymns_sort) == SQLITE_ROW) {
		int hymnID = sqlite3_column_int(get_favouriteHymns_sort, 0);
		Hymn hymn = Hymn(hymnID, hymnal);
		hymns.push_back(hymn);
	}
	sqlite3_reset(get_favouriteHymns_sort);
	return hymns;
}

HymnVector getHymnsForNumericSearch(int hymnalID, int hymnNumber){
	Hymnal hymnal = Hymnal(hymnalID);
	if (0 == get_hymnsForNumericSearch) {
		const char *sql = "SELECT hymnID, hymnNumber, title FROM hymn WHERE hymnal = ? AND hymnNumber = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnsForNumericSearch, NULL) != SQLITE_OK) {
			printf("Problem preparing get_hymnsForNumericSearch: %s\n", sqlite3_errmsg(database));
		}
	}
	
	if (sqlite3_bind_int(get_hymnsForNumericSearch, 1, hymnalID) != SQLITE_OK){
		printf("Problem binding to 1 of get_hymnsForNumericSearch.\n");
	}
	
	if (sqlite3_bind_int(get_hymnsForNumericSearch, 2, hymnNumber) != SQLITE_OK){
		printf("Problem binding to 2 of get_hymnsForNumericSearch.\n");
	} 
	HymnVector hymns;
	while (sqlite3_step(get_hymnsForNumericSearch) == SQLITE_ROW) {
		int hymnID = sqlite3_column_int(get_hymnsForNumericSearch, 0);
		Hymn hymn = Hymn(hymnID, hymnal);
		hymns.push_back(hymn);
	}
	sqlite3_reset(get_hymnsForNumericSearch);
	return hymns;
}

HymnVector getHymnsForTitleSearch(int hymnalID, std::string& searchString){
	Hymnal hymnal = Hymnal(hymnalID);
	if (0 == get_hymnsForTitleSearch) {
		const char *sql = "SELECT hymnID, hymnNumber, title FROM hymn WHERE hymnal = ? AND title LIKE ?";
		if (sqlite3_prepare_v2(database, sql, -1, &get_hymnsForTitleSearch, NULL) != SQLITE_OK) {
			printf("Problem preparing get_hymnsForTitleSeach: %s\n", sqlite3_errmsg(database));
		}
	}
	
	if (sqlite3_bind_int(get_hymnsForTitleSearch, 1, hymnalID) != SQLITE_OK){
		printf("Problem binding to 1 of get_hymnsForTitleSearch.\n");
	}
	//prepare the search string.
	searchString.append("%");
	searchString.insert(0, "%");
	if (sqlite3_bind_text(get_hymnsForTitleSearch, 2, searchString.c_str(), strlen(searchString.c_str()), SQLITE_TRANSIENT)) {
		printf("Problem binding text to get_hymnsForTitleSearch.\n");
	}
	
	HymnVector hymns;
	while (sqlite3_step(get_hymnsForTitleSearch) == SQLITE_ROW) {
		int hymnID = sqlite3_column_int(get_hymnsForTitleSearch, 0);
		Hymn hymn = Hymn(hymnID, hymnal);
		hymns.push_back(hymn);
	}
	sqlite3_reset(get_hymnsForTitleSearch);
	return hymns;
}

HymnVector getRecents(int hymnalID){
	Hymnal hymnal = Hymnal(hymnalID);
	if (0 == get_recents) {
		const char *sql = "SELECT hymn FROM recents WHERE hymnal = ? ORDER BY rowID DESC";
		if (sqlite3_prepare_v2(database, sql, -1, &get_recents, NULL) != SQLITE_OK) {
			printf("Problem preparing get_recents: %s\n", sqlite3_errmsg(database));
		}
	}
	
	sqlite3_bind_int(get_recents, 1, hymnalID);
	HymnVector hymns;
	while (sqlite3_step(get_recents) == SQLITE_ROW) {
		int hymnID = sqlite3_column_int(get_recents, 0);
		Hymn hymn = Hymn(hymnID, hymnal);
		hymns.push_back(hymn);
	}
	sqlite3_reset(get_recents);
	return hymns;
}

#pragma mark -

#pragma mark Favourites

void setFavouriteStatusForHymn(int hymnalID, int hymnID, bool favouriteStatus){
	if (0 == set_hymnFavouriteStatus) {
		const char * sql = "UPDATE hymn SET favourite = ? WHERE hymnID = ? AND hymnal = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &set_hymnFavouriteStatus, NULL) != SQLITE_OK) {
			printf("Problem preparing set_hymnFavouriteStatus: %s\n", sqlite3_errmsg(database));
		}
	}
	
	sqlite3_bind_int(set_hymnFavouriteStatus, 1, favouriteStatus);
	sqlite3_bind_int(set_hymnFavouriteStatus, 2, hymnID);
	sqlite3_bind_int(set_hymnFavouriteStatus, 3, hymnalID);
	
	if (sqlite3_step(set_hymnFavouriteStatus) != SQLITE_DONE) {
		printf("Couldn't set Favourite Status to %d For Hymn %d in Hymnal %d.\n", favouriteStatus, hymnID, hymnalID);
	}
	sqlite3_reset(set_hymnFavouriteStatus);
}
#pragma mark -

#pragma mark Recents

void deleteOldestEntryInRecents(){
	if (0 == get_oldest_entry_from_recents) {
		const char *sql = "SELECT rowID FROM recents ORDER BY rowID DESC LIMIT 1";
		if (sqlite3_prepare_v2(database, sql, -1, &get_oldest_entry_from_recents, NULL) != SQLITE_OK) {
			printf("Problem preparing get_oldest_entry_from_recents, %s\n", sqlite3_errmsg(database));
		}
	}
	if (0 == delete_entry_from_recents) {
		const char *sql = "DELETE FROM recents WHERE rowID = ?";
		if (sqlite3_prepare_v2(database, sql, -1, &delete_entry_from_recents, NULL) == SQLITE_OK) {
			printf("Problem preparing delete_entry_from_recents: %s\n", sqlite3_errmsg(database));
		}
	}
	if (sqlite3_step(get_oldest_entry_from_recents) == SQLITE_ROW) {
		int oldestRowID = sqlite3_column_int(get_oldest_entry_from_recents, 0);
		sqlite3_bind_int(delete_entry_from_recents, 1, oldestRowID);
		sqlite3_step(delete_entry_from_recents);
		sqlite3_reset(delete_entry_from_recents);
	}
	sqlite3_reset(get_oldest_entry_from_recents);
}


void deleteExtraEntriesFromRecents(){
	if (0 == count_entries_in_recents) {
		const char *sql = "SELECT COUNT(1) FROM recents";
		if (sqlite3_prepare_v2(database, sql, -1, &count_entries_in_recents, NULL) != SQLITE_OK) {
			printf("Problem preparing count_entries_in_recents: %s\n", sqlite3_errmsg(database));
		}
	}
	if (sqlite3_step(count_entries_in_recents) == SQLITE_ROW) {
		int count = sqlite3_column_int(count_entries_in_recents, 0);
		while (count > MAX_RECENTS) {
			deleteOldestEntryInRecents();
			count --;
		}
	}
	sqlite3_reset(count_entries_in_recents);
}

void addHymnToRecents(int hymnalID, int hymnID){
	if (0 == insert_hymnIntoRecents) {
		const char *sql = "INSERT INTO recents (hymnal, hymn) VALUES (?, ?)";
		if (sqlite3_prepare_v2(database, sql, -1, &insert_hymnIntoRecents, NULL) != SQLITE_OK) {
			printf("Problem preparing insert_hymnIntoRecentsAdd: %s\n", sqlite3_errmsg(database));
		}
	}
	sqlite3_bind_int(insert_hymnIntoRecents, 1, hymnalID);
	sqlite3_bind_int(insert_hymnIntoRecents, 2, hymnID);
	if (sqlite3_step(insert_hymnIntoRecents) != SQLITE_DONE) {
		printf("Problem adding hymn to recents: %s", sqlite3_errmsg(database));
	}
	sqlite3_reset(insert_hymnIntoRecents);
	deleteExtraEntriesFromRecents();
}



#pragma mark -