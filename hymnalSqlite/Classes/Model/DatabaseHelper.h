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
	SORT_BY_NUMBER = 0,
	SORT_BY_NAME
} HymnSort;

//typedefs to simplify declaring these
//Read these to understand.
typedef  std::pair<int, std::string> hymnPiece;
typedef  std::vector<hymnPiece> hymnPieceVector;

//These methods return pairs, first the int tells you the ordering, then the string the filepath
hymnPieceVector getMusicPiecesForHymn(int hymnID, PartSpecifier part, sqlite3* database);

hymnPieceVector getLyricPiecesForHymn(int hymnID, VerseSpecifier verse, sqlite3* database);



//Call this method before closing the database connection so we keep a clean DB.
//This method finalizes all open SQL statements in this class.  
//Along with the destructors being called on all Hymn and Hymnal objects, this should finalize all statements.
void finalizeDatabaseStatements();

#endif