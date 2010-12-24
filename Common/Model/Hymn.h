/*
 *  Hymn.h
 *  hymnalSqlite
 *
 *  Created by Daniel Johnson on 10-11-28.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __HYMN__
#define __HYMN__

#include <sqlite3.h>
#include <string>
#include <vector>
#include "Hymnal.h"

class Hymn  {
public:
	Hymn();
	~Hymn();
	Hymn(const Hymn& h);
	Hymn(const int hymnID, const Hymnal& parentHymnal);
	Hymn(const int hymnID, const Hymnal& parentHymnal, sqlite3_stmt* get_hymn);
	
	int get_hymnID() const;
	int get_hymnNumber() const;
	std::string get_author() const;
	std::string get_composer() const;
	std::string get_copyrightInfo() const;
	std::string get_firstLine() const;
	std::string get_metric() const;
	std::string get_section() const;
	std::string get_subSection() const;
	std::string get_subSubSection() const;
	std::string get_title() const;
	std::string get_translator() const;
	std::string get_tune() const;
	bool get_favourite() const;
	//Setters aren't necessary. 
	
	static void prepareDatabaseStatements(sqlite3 *database);
	static void tsPrepareDatabaseStatements(sqlite3 *database, sqlite3_stmt** ret_get_hymn_info);
	
	static void finalizeDatabaseStatements();
	
	//Debug Function.
	void printDescription() const;
	
private:
	Hymnal hymnal;
	int hymnID;
	int hymnNumber;

	std::string author;
	std::string composer;
	std::string copyrightInfo;
	std::string firstLine;
	std::string metric;
	std::string section;
	std::string subSection;
	std::string subSubSection;
	std::string title;
	std::string translator;
	std::string tune;
	bool favourite;

	void setHymnDetailsFromSqlRow(sqlite3_stmt *row);
};

#endif