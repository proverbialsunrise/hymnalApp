import sqlite3
import DatabaseCommands
import os

DEBUG = True
DB_FILE = '/tmp/exampleDB'

def createDB():
    '''Create the hymnal database.'''
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    commands = DatabaseCommands.sql_commands
    
    for command in commands:
        c.execute ( command )
        
    conn.commit()

def addHymnal ( name, copyrightText ):
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute ( "INSERT INTO hymnal (name, copyrightText) VALUES ('" + \
                name + "', '" + copyrightText + "');")
    conn.commit()

class Hymn:
    def __init__(self,  hymnal="" , number="", author="", composer="", copyrightInfo="", firstLine="", metric="", section="",
                 subSection="", subSubSection="", title="", translator="", tune=""):
        self.hymnal = hymnal
        self.num = number
        self.author = author
        self.composer = composer
        self.copyrightInfo = copyrightInfo
        self.firstLine = firstLine
        self.metric = metric
        self.section = section
        self.subSection = subSection
        self.subSubSection = subSubSection
        self.title = title
        self.translator = translator
        self.tune = tune
    
    def getInfoString (self):
        '''Prints all the information on separate lines.'''
        return self.hymnal + \
                "\n" + self.num + \
                "\n" + self.author + \
                "\n" + self.composer + \
                "\n" + self.copyrightInfo + \
                "\n" + self.firstLine + \
                "\n" + self.metric + \
                "\n" + self.section + \
                "\n" + self.subSection + \
                "\n" + self.subSubSection + \
                "\n" + self.title + \
                "\n" + self.translator + \
                "\n" + self.tune
                
def readSampleInput():
    '''Read hymn information from a file of hymns separated by newlines,
    with values separated by the | character'''
    f = open ( 'sampleInput', 'r' )
    hymns = []
    for line in f:
        parts = line.split('|')
        h = Hymn ( parts[0], parts[1], parts[2], parts[3], parts[4], parts[5],
                   parts[6],parts[7], parts[8], parts[9], parts[10], parts[11], parts[12] )
        hymns.append(h)
    return hymns

def readFromDatabase():
    '''Just read all the hymns from the database.'''
    conn = sqlite3.connect(DB_FILE)
    conn.text_factory=str
    c = conn.cursor()
    c.execute ( "select * from hymn;" )
    for row in c:
        for cell in row:
            print cell

def writeToDatabase ( hymn ):
    '''Write the hymn object to the hymn table in the database.'''
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute ( "SELECT hymnalID FROM hymnal WHERE name='" + hymn.hymnal + "';" )
    row = c.fetchone()
    hymnalID = row[0]
    sql = 'INSERT INTO hymn (hymnNumber, author, composer, copyrightInfo, firstLine, metric, '   + \
            'section, subSection, subSubSection, title, translator, tune, hymnal) ' + \
            'VALUES (\'' + \
                hymn.num + "', '" + \
                hymn.author + "', '" + \
                hymn.composer + "', '" + \
                hymn.copyrightInfo + "', '" + \
                hymn.firstLine + "', '" + \
                hymn.metric + "', '" + \
                hymn.section + "', '" + \
                hymn.subSection + "', '" + \
                hymn.subSubSection + "', '" + \
                hymn.title + "', '" + \
                hymn.translator + "', '" + \
                hymn.tune + "', " + \
                str(hymnalID) + \
                ');'
    c.execute ( sql )

    conn.commit()

def addHymnSectionsToDB ( imagesDir, hymnNumber, hymnalName ):
    '''Iterate through the given directory, and add the files to the lyricSection/musicSection
    tables, associating with the given hymn'''
    dirName = hymnalName.replace(":", "") + imagesDir[imagesDir.rfind("/"):]
    
    list = os.listdir ( imagesDir )
    list.sort();
    title=[]
    lyrics=[]
    music=[]
    
    for name in list:
        if name[0] == ".":
            continue
        components = parseName(name)
        if ( components[1] == "title" ):
            title = components
        elif ( len(components) == 4 ):
            lyrics.append(components)
        else:
            music.append(components)
    
    #Simple detection of unison treble clefs
    #If a treble clef is at the end or is followed by
    #another treble clef, we assume it is unison
    for i in range(len(music)):
        if music[i][2] == 1 and \
                (i == len(music)-1 or music[i+1][2] == 1):
            music[i][2] = 0
    
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute( "SELECT hymnalID FROM hymnal WHERE name='" + hymnalName +  "';")
    row = c.fetchone()
    hymnalID = row[0]
    c.execute ( "SELECT hymnID FROM hymn WHERE hymnNumber='" + str(hymnNumber) + "' AND " + \
                "hymnal='" + str(hymnalID) + "';" )
    row = c.fetchone()
    hymnID = row[0] #will fail here if hymn not found
            
    if DEBUG: print title
    i=0
    while i < len(lyrics):
        x = lyrics[i]
        if ( x[3] == '1' and \
           (i + 1 == len(lyrics) or lyrics[i+1][3] == '1') ):
            x[3] = '-1'
        if DEBUG: print x
        sql = "insert into lyricSection (image, lineNumber, type, verseNumber, hymn) VALUES ('" +\
              dirName + "/" + x[0] + "', '" + x[1] + "', '" + str(x[2]) + "', '" + x[3] + "', '" + str(hymnID) + "');"
        c.execute (sql)
        i = i + 1

    for x in music:
        if DEBUG: print x
        sql = "insert into musicSection (image, lineNumber, type, hymn) VALUES ('" +\
              dirName + "/" + x[0] + "', '" + x[1] + "', '" + str(x[2]) + "', '" + str(hymnID) + "');"
        c.execute (sql)

    conn.commit()

def parseName (name):
    '''Take a hymn image file name and return a list of its components.
       lyric section: filename, line, type, verse
       music section: filename, line, type'''
    if ( name.find("title") > -1 ):
        return [name, "title"]
    ret = []
    ret.append ( name )
    ret.append ( name[0:4] )
    i = 4
    type=name[4:-4] #-4 = ".png"
    
    if type.find("text") > -1:
        #ret.append(type[0:-1])
        ret.append ( 0 ) #for now just assuming unisex text
        ret.append(type[-1])
    elif type == "treble": #for now assuming no other staff type
        ret.append ( 1 )
    elif type == "bass":
        ret.append ( 2 )

    return ret

def addAllHymnSections ( hymnalName, baseDir ):
    '''Takes a hymnal directory, goes through the hymn
       subdirectories and adds the sections to the database.'''
    list = os.listdir ( baseDir )
    for name in list:
        if os.path.isdir(baseDir + "/" + name) == False or \
            name[0] == ".":
            continue
        #convert to int to ensure that it's a number
        #and to strip leading zeroes
        hymnNumber = int(name[0:3])
        
        addHymnSectionsToDB ( baseDir + "/" + name, hymnNumber, hymnalName )

###BEGIN SCRIPT###
HWB = "Hymnal: A Worship Book"
createDB()
addHymnal ( HWB, "copyright unknown to jake")
hymns = readSampleInput()
for hymn in hymns:
    if DEBUG: print hymn.getInfoString()
    writeToDatabase(hymn)

addAllHymnSections ( HWB, "../../imageProcessing/sampleParsed/" )

print ( "Finished" )
