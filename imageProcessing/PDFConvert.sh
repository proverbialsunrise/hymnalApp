#!/bin/bash


#Usage: ./PDFConvert.sh [directoryToConvert]
#Purpose: Converts all PDFs in a directory to PNGs using ImageMagick.
#ImageMagick can be found at http://www.imagemagick.org/script/index.php


cd $1
echo "Converting directory $1"
FILES="*.pdf"
for file in $FILES ; do
	echo Converting file: $file
	convert "$file" "${file%.eps}".eps
done
