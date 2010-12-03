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
	Hymn(const int hymnID, const Hymnal& parentHymnal, sqlite3 *db);

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
	//Setters aren't necessary. 
	
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
	
	sqlite3 *database;

	
	void prepareDatabaseStatements();
	void finalizeDatabaseStatements();
	void setHymnDetailsFromSqlRow(sqlite3_stmt *row);
};

#endif