/*
 *  DatabaseHelper.h
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __HYMNDATABASEHELPER__
#define __HYMNDATABASEHELPER__

#include <vector>
#include <string>
#include <utility>

extern "C"{
#include "sqlite3.h"
}
#include "Hymn.h"
#include "Hymnal.h"

static const unsigned int ALLVERSES = 0;

//enum for which parts to return
typedef enum PartSpecifier {
	ALLPARTS = 0,
	TREBLE = 1,
	BASS = 2	
} PartSpecifier;

//enum for how to sort lists of hymns
typedef enum HymnSort {
	SORT_BY_NUMBER = 2,
	SORT_BY_NAME = 3
} HymnSort;

typedef struct HymnSection {	
	int lineNumber;
	int part;
	std::string imagePath;
} HymnPiece;

#pragma mark Database Lifecycle
void openConnectionWithPath(std::string& databasePath);

void tsOpenConnectionWithPath(std::string& databasePath, sqlite3** dbPointer);
//Call this method when finished with the DB so we keep a clean DB.
//This method finalizes all open SQL statements in this class.  
//Along with the destructors being called on all Hymn and Hymnal objects, this should finalize all statements.

void closeConnection();

void tsCloseConnection(sqlite3* db, sqlite3_stmt* get_hymnal_stm, sqlite3_stmt* get_hymn_stm);
#pragma mark -

Hymnal getHymnal(int hymnalID);

Hymnal tsGetHymnal(int hymnalID, sqlite3_stmt* get_hymnal);

#pragma mark Hymn Display
typedef  std::vector<HymnSection> HymnSectionVector;

HymnSectionVector getMusicPiecesForHymn(int hymnID, PartSpecifier part);

HymnSectionVector getLyricPiecesForHymn(int hymnID, int verse, PartSpecifier part);

HymnSectionVector getPiecesForHymn(int hymnID, int verse, PartSpecifier part);

unsigned int getNumVersesForHymn(int hymnID);
#pragma mark -


#pragma mark Getting Hymns

typedef std::vector<Hymn> HymnVector;

//Gets all the hymns for the specified hymnal. Sorted by sortBy.
HymnVector getHymnsForHymnal(int hymnalID, HymnSort sortBy);

HymnVector getHymnsForSection(int hymnalID, std::string& section, HymnSort sortBy);

HymnVector getHymnsForSubSection(int hymnalID, std::string& subSection, HymnSort sortBy);

HymnVector getFavouriteHymnsForHymnal(int hymnalID, HymnSort sortBy);

HymnVector getHymnsForNumericSearch(int hymnalID, int hymnNumber);

HymnVector getHymnsForTitleSearch(int hymnalID, std::string& searchString);

#pragma mark -

#pragma mark Writing To DB
//This method needs to take the hymnID not the hymn number.  HymnIDs are unique while hymnNumbers are only unique to a hymnal.
void setFavouriteStatusForHymn(int hymnID, bool favouriteStatus);

void addHymnToRecents(int hymnID);

#pragma mark -

#endif
