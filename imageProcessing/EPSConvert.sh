#!/bin/bash

#Usage: ./EPSConvert.sh [directoryToConvert]
#Purpose: Converts all EPS files in a directory to PNGs using GhostScript.
#GhostScript can be found at http://pages.cs.wisc.edu/~ghost/

cd $1
echo "Converting directory $1"
FILES="*.eps"
for file in $FILES ; do
	echo Converting file: $file
	gs -dSAFER -dBATCH -dNOPAUSE -sDEVICE=png16m -r200 -sOutputFile="${file%.eps}".png "$file"
done
