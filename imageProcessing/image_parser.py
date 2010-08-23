#Script which iterates over '.png' files in a directory, for each it:
#   recognizes clefs in the hymn
#   cuts out the staves it finds, and stores them in a directory
#   cuts out the lines of text it finds between staves
#   cuts out the title (this doesn't work if there is no title on the page

import Image
import sys
import os
import glob

def matchImage(smallData, smallWidth, bigData, bigWidth, offsetX, offsetY, minRatio):
    bigIndex = (bigWidth * offsetY) + offsetX
    if bigIndex > len(bigData):
        return -1
    count = 0
    s = ""
    ssmall = ""
    for i in range(len(smallData)):
        if i % smallWidth == 0:
            #print(s,ssmall)
            s = ""
            ssmall = ""
            bigIndex = (bigWidth * ((i/smallWidth) + offsetY)) + offsetX
        else:
            bigIndex = bigIndex + 1

        if(bigIndex > len(bigData)):
            return -1

        if smallData[i][0] == 255:
            if bigData[bigIndex][0] == 255:
                count = count + 1
        else:
            if bigData[bigIndex][0] < 255:
                count = count + 1

        if bigData[bigIndex][0] == 255:
            s = s + " "
        else:
            s = s + "X"
        if smallData[i][0] == 255:
            ssmall = ssmall + " "
        else:
            ssmall = ssmall + "X"

        if float(i - count) / float(len(smallData)) > float(1.0 - minRatio):
            #print("fail: i:%f, count=%f, len=%f, ratio=%f" % (float(i), float(count), float(len(smallData)), float(1.0 - minRatio)))
            return 0
        
    return float(count) / float(len(smallData))

def cutSection(image, top, bottom, left, right, outFileName):
    im = image.copy()
    region = im.crop([left,top, right, bottom])
    region.save(outFileName)

def pixelsOnLine(imageWidth, imageData, offsetY, offsetX):
    index = imageWidth * offsetY
    count = 0
    for i in range(index + offsetX, index + imageWidth - 1): #skip the beginning of the line to avoid staff-connectors
        if(imageData[i][0] < 255):
            count = count + 1

    return count

def pixelsInColumn(imageWidth, imageData, column):
    index = column
    count = 0
    while(index < len(imageData)):
        if(imageData[index][0] < 255):
            count = count + 1
        index = index + imageWidth

    return count

X_OFFSET = 376
Y_OFFSET = 200

def topOfStaff(imageWidth, imageData, offsetY):
    lineNum = offsetY
    nearlyDone = 0
    while lineNum > 0:
        pixels = pixelsOnLine(imageWidth, imageData, lineNum, X_OFFSET)
        if(pixels == 0):
            return lineNum
        elif(pixels < 5):
            nearlyDone = 1
        elif(nearlyDone):
            return lineNum + 1
        lineNum = lineNum - 1
        
    return lineNum

def bottomOfStaff(imageWidth, imageData, offsetY):
    lineNum = offsetY + 1
    nearlyDone = 0
    while 1:
        pixels = pixelsOnLine(imageWidth, imageData, lineNum, X_OFFSET)
        if(pixels == 0):
            return lineNum
        elif(pixels < 5):
            nearlyDone = 1
        elif(nearlyDone == 1):
            return lineNum - 1
        lineNum = lineNum + 1

    return lineNum

def leftOfStaves(imageWidth, imageData):
    colNum = 1
    while 1:
        if(pixelsInColumn(imageWidth, imageData, colNum) > 0):
            return colNum - 1
        colNum = colNum + 1

def rightOfStaves(imageWidth, imageData):
    colNum = imageWidth - 2
    while 1:
        if(pixelsInColumn(imageWidth, imageData, colNum) > 0):
            return colNum + 1
        colNum = colNum - 1


def getStaves(directory, hymnFileName, hymnImage, clefFileName, clefName):
    indices = []
    clefImage = Image.open(clefFileName, 'r')
    y = Y_OFFSET
    x = X_OFFSET
    minRatio=0.8
    clefsFound = 0
    
    clefData = list(clefImage.getdata())
    clefWidth = clefImage.getbbox()[2]
    
    while 1:
        y = y + 1
        #print("trying position: %d, %d" % (x,y))
        ratio = matchImage(clefData, clefWidth, hymnData, hymnWidth, x, y, minRatio)
        if ratio == -1:
            break
        if ratio < minRatio:
            continue
        
        print("matchRatio: %f" % ratio)
        clefsFound = clefsFound + 1
        top = topOfStaff(hymnWidth, hymnData, y + 5)
        bottom = bottomOfStaff(hymnWidth, hymnData, y + 5)
        left = leftOfStaves(hymnWidth, hymnData)
        right = rightOfStaves(hymnWidth, hymnData)
        if(not os.path.exists("%s/%s" % (directory, hymnFileName[0:-4]))):
           os.makedirs("%s/%s" % (directory, hymnFileName[0:-4]))
        cutSection(hymnImage, top, bottom, left, right, "%s/%s/%04d%s%d.png" % (directory, hymnFileName[0:-4], top, clefName, clefsFound))
        indices.append([top,bottom])
        if y < bottom:
            y = bottom

    return indices

def mergeVals(treble, bass):
    retVal = []
    while(len(treble) + len(bass) > 0):
        if len(treble) == 0:
            retVal.append(bass.pop(0))
        elif len(bass) == 0:
            retVal.append(treble.pop(0))
        else:
            if treble[0][0] < bass[0][0]:
                retVal.append(treble.pop(0))
            elif bass[0][0] < treble[0][0]:
                retVal.append(bass.pop(0))
                
    return retVal

def getTextBetween(top, bottom, imageWidth, imageData, image, lineNumber):
    print("getting text between %d and %d; line %d" % (top, bottom, lineNumber))
    locations = []
    i = top
    textLineStart = -1
    while(i < bottom):
        if(pixelsOnLine(imageWidth, imageData, i, X_OFFSET) > 0):
            if textLineStart == -1:
                textLineStart = i
        else:
            if textLineStart > -1:
                if i - textLineStart > 20:
                    locations.append([textLineStart, i])
                textLineStart = -1
        i = i + 1

    #print(locations)
    
    left = leftOfStaves(hymnWidth, hymnData)
    right = rightOfStaves(hymnWidth, hymnData)
    i = 1
    for vals in locations:
        cutSection(image, vals[0], vals[1], left, right, "%s/%s/%04dtext%d_%d.png" % (directory, hymnFileName[0:-4], vals[0], i, lineNumber))
        i = i + 1

    return len(locations) > 0

def getTitle(hymnWidth, hymnData, hymnImage):
    i = 0
    while(pixelsOnLine(hymnWidth, hymnData, i, 0) == 0):
        i = i + 1
    top = i
    while(pixelsOnLine(hymnWidth, hymnData, i, 0) > 0):
        i = i + 1
    bottom = i
    cutSection(hymnImage, top, bottom, leftOfStaves(hymnWidth, hymnData), rightOfStaves(hymnWidth, hymnData), \
               "%s/%s/%04dtitle.png" % (directory, hymnFileName[0:-4], top))

    

### BEGIN SCRIPT ###
if len(sys.argv) < 2:
    print("Usage: image_parser.py directory")
    exit()
directory = sys.argv[1]
print(directory)
trebleClefFileName = directory + "/util/treble_clef.png"
bassClefFileName = directory + "/util/bass_clef.png"
for infile in glob.glob(directory + "/*.png"):
    hymnFileName = infile[len(directory)+1:]
    print("hymn: "+ hymnFileName)
    
    hymnImage = Image.open(directory + "/" + hymnFileName, 'r')
    hymnData = list(hymnImage.getdata())
    hymnWidth = hymnImage.getbbox()[2]
    
    yVals = getStaves(directory, hymnFileName, hymnImage, trebleClefFileName, "treble")
    #print(yVals)
    if(os.path.exists(bassClefFileName)):
        bassYVals = getStaves(directory, hymnFileName, hymnImage, bassClefFileName, "bass")
        #print(bassYVals)

    clefVals = mergeVals(yVals, bassYVals)
    #print(clefVals)
    lineNumber = 1
    for i in range(0, len(clefVals)):
        bottom = len(hymnData) / hymnWidth
        if i < len(clefVals) - 1:
            bottom = clefVals[i+1][0] + 1
            
        foundStuff = getTextBetween(clefVals[i][1], bottom, hymnWidth, hymnData, hymnImage, lineNumber)
        if(foundStuff):
            lineNumber = lineNumber + 1

    getTitle(hymnWidth, hymnData, hymnImage)

