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

#include "sqlite3.h"
#include "Hymn.h"
#include "Hymnal.h"


//enum for which verses to return
typedef enum VerseSpecifier {
	ALLVERSES = 0,
	VERSEONE = 1,
	VERSETWO = 2,
	VERSETHREE = 3,
	VERSEFOUR = 4,
	VERSEFIVE = 5,
	VERSESIX = 6,
	VERSESEVEN = 7,
	VERSEEIGHT = 8,
	VERSENINE = 9,
	VERSETEN = 10
	//This should be enough?
} VerseSpecifier;

//enum for which parts to return
typedef enum PartSpecifier {
	ALLPARTS = 0,
	SOPRANOALTO = 1,
	TENORBASS = 2	
} PartSpecifier;

//enum for how to sort lists of hymns
typedef enum HymnSort {
	SORT_BY_NUMBER = 1,
	SORT_BY_NAME = 2
} HymnSort;



#pragma mark Databse Lifecycle
void openConnectionWithPath(std::string& databasePath);

//Call this method before closing the database connection so we keep a clean DB.
//This method finalizes all open SQL statements in this class.  
//Along with the destructors being called on all Hymn and Hymnal objects, this should finalize all statements.
void finalizeDatabaseStatements();

void closeConnection();

#pragma mark -

#pragma mark Hymn Display
//typedefs to simplify declaring these
typedef  std::pair<int, std::string> HymnPiece;
typedef  std::vector<HymnPiece> HymnPieceVector;

//These methods return pairs, first the int tells you the ordering, then the string the filepath

HymnPieceVector getMusicPiecesForHymn(int hymnID, PartSpecifier part);

HymnPieceVector getLyricPiecesForHymn(int hymnID, VerseSpecifier verse);

#pragma mark -


#pragma mark Getting Hymns

typedef std::vector<Hymn> HymnVector;


//Gets all the hymns for the specified hymnal. Sorted by sortBy.
HymnVector getHymnsForHymnal(int hymnalID, HymnSort sortBy);

HymnVector getHymnsForSection(int hymnalID, std::string& section, HymnSort sortBy);

HymnVector getHymnsForSubSection(int hymnalID, std::string& subSection, HymnSort sortBy);



#pragma mark -

#endif