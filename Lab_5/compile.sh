#!/bin/bash

sourceFile=$1.c
exeFile=$1.exe

if [ -f $sourceFile ]; then
	gcc -o $exeFile $sourceFile && echo "$sourceFile compiled, $exeFile created"
else
	echo "File $sourceFile not found"
fi