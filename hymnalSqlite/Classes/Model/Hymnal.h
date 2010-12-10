/*
 *  Hymnal.h
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-28.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __HYMNAL__
#define __HYMNAL__

#include <sqlite3.h>
#include <string>
#include <vector>

//It is not safe to use this class where we might be connecting to multiple databases at once.

class Hymnal {
	public:
	
	Hymnal();
	Hymnal(const Hymnal& h);
	Hymnal(int hID);
	Hymnal(int hID, sqlite3_stmt* get_hymnal);
	
	
	~Hymnal();
	
	int get_hymnalID() const;
	std::string get_title() const;
	std::string get_copyrightText() const;
	std::string get_shortName() const;
	
	static void finalizeDatabaseStatements();
	static void prepareDatabaseStatements(sqlite3 *database);
	static void tsPrepareDatabaseStatements(sqlite3 *database, sqlite3_stmt** ret_get_hymnal_stmt);

	
	private:

	int hymnalID;
	std::string title;
	std::string copyrightText;
	std::string shortName;
	
	void setHymnalDetailsFromSqliteRow(sqlite3_stmt *row);
};

#endif