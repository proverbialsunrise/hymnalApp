CREATE TABLE hymnal (
hymnalID INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT, 
copyrightText TEXT
);
					
CREATE TABLE hymn (
hymnID INTEGER PRIMARY KEY AUTOINCREMENT,
hymnNumber INTEGER,
author TEXT,
composer TEXT,
copyrightInfo TEXT,
firstLine TEXT,
metric TEXT,
section TEXT,
subSection TEXT,
subSubSection TEXT,
title TEXT,
translator TEXT,
tune TEXT,
hymnal INTEGER,
FOREIGN KEY(hymnal) REFERENCES hymnal(hymnalID)
);

CREATE TABLE lyricSection (
lyricSectionID INTEGER PRIMARY KEY AUTOINCREMENT,
image TEXT,
lineNumber INTEGER,
location TEXT,
type TEXT,
verseNumber INTEGER,
hymn INTEGER,
FOREIGN KEY(hymn) REFERENCES hymn(hymnID) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE musicSection (
musicSectionID INTEGER PRIMARY KEY AUTOINCREMENT,
image TEXT,
type TEXT,
lineNumber INTEGER,
hymn INTEGER,
FOREIGN KEY(hymn) REFERENCES hymn(hymnID) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE scripturalAllusion (
scripturalAllusionID INTEGER PRIMARY KEY AUTOINCREMENT,
citation TEXT,
referenceText TEXT,
hymn INTEGER,
FOREIGN KEY(hymn) REFERENCES hymn(hymnID) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE usesInWorship (
usesInWorshipID INTEGER PRIMARY KEY AUTOINCREMENT,
useInWorship TEXT,
hymn INTEGER,
FOREIGN KEY(hymn) REFERENCES hymn(hymnID) ON DELETE CASCADE ON UPDATE CASCADE 
);