import sqlite3
import DatabaseCommands
import os

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
                   parts[6],parts[7], parts[8], parts[9], parts[10], parts[11] )
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

def addHymnSectionsToDB ( imagesDir, hymnNumber ):
    '''Iterate through the given directory, and add the files to the lyricSection/musicSection
    tables, associating with the given hymnNumber'''
    #TODO: also make sure the hymn is from the right hymnal
    list = os.listdir ( imagesDir )
    list.sort();
    title=[]
    lyrics=[]
    music=[]
    
    for name in list:
        components = parseName(name)
        if ( components[1] == "title" ):
            title = components
        elif ( components[2].find("text") > -1 ):
            lyrics.append(components)
        else:
            music.append(components)
    
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute ( "SELECT hymnID FROM hymn WHERE hymnNumber='" + str(hymnNumber) + "';" ) #AND hymnal == X
    row = c.fetchone()
    hymnID = row[0] #will fail here if hymn not found
            
    print title
    for x in lyrics:
        print x
        sql = "insert into lyricSection (image, lineNumber, type, verseNumber, hymn) VALUES ('" +\
              x[0] + "', '" + x[1] + "', '" + x[2] + "', '" + x[3] + "', '" + str(hymnID) + "');"
        #print sql
        c.execute (sql)
    for x in music:
        print x
        sql = "insert into musicSection (image, lineNumber, type, hymn) VALUES ('" +\
              x[0] + "', '" + x[1] + "', '" + x[2] + "', '" + str(hymnID) + "');"
        #print sql
        c.execute (sql)
    
    conn.commit()

def parseName (name):
    '''Take a hymn image file name and return a list of its components.'''
    if ( name.find("title") > -1 ):
        return [name, "title"]
    ret = []
    ret.append ( name )
    ret.append ( name[0:4] )
    i = 4
    type=""
    while i < len(name):
        if ( name[i].isdigit() ):
            type = name[4:i]
            break
        i = i+1
    
    ret.append ( type )
    if ( type.find("text") > -1 ): #TODO: other types? textmen, textwomen?
        ret.append ( name[i:name.find("_")] )
        ret.append ( name[name.find("_")+1:name.find(".")])
    else:
        ret.append ( name[i:name.find(".")] )
    return ret

createDB()
addHymnal ( "Hymnal: A Worship Book", "copyright unknown to jake")
hymns = readSampleInput()
for hymn in hymns:
    print hymn.getInfoString()
    writeToDatabase(hymn)

#addHymnSectionsToDB ( 
#    "/Users/jake/Documents/programming/iHymnal/src/hymnalApp/imageProcessing/z sampleParsed/043 My faith has foundmus",
#    43 )

print ( "Finished" )
