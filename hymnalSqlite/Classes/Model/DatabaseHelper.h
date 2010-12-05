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


//enum for which parts to return
typedef enum PartSpecifier {
	ALLPARTS = 0,
	TREBLE = 1,
	BASS = 2	
} PartSpecifier;

//enum for how to sort lists of hymns
typedef enum HymnSort {
	SORT_BY_NUMBER = 1,
	SORT_BY_NAME = 2
} HymnSort;

typedef struct HymnSection {	
	int lineNumber;
	int part;
	std::string imagePath;
} HymnPiece;

#pragma mark Databse Lifecycle
void openConnectionWithPath(std::string& databasePath);

//Call this method before closing the database connection so we keep a clean DB.
//This method finalizes all open SQL statements in this class.  
//Along with the destructors being called on all Hymn and Hymnal objects, this should finalize all statements.
void finalizeDatabaseStatements();

void closeConnection();

#pragma mark -

Hymnal getHymnal(int hymnalID);

#pragma mark Hymn Display
typedef  std::vector<HymnSection> HymnSectionVector;

//These methods return pairs, first the int tells you the ordering, then the string the filepath

HymnSectionVector getMusicPiecesForHymn(int hymnID, PartSpecifier part);

HymnSectionVector getLyricPiecesForHymn(int hymnID, int verse, PartSpecifier part);

#pragma mark -


#pragma mark Getting Hymns

typedef std::vector<Hymn> HymnVector;

//Gets all the hymns for the specified hymnal. Sorted by sortBy.
HymnVector getHymnsForHymnal(int hymnalID, HymnSort sortBy);

HymnVector getHymnsForSection(int hymnalID, std::string& section, HymnSort sortBy);

HymnVector getHymnsForSubSection(int hymnalID, std::string& subSection, HymnSort sortBy);



#pragma mark -

#endif