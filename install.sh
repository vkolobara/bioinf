#!/bin/bash

########################## DOWNLOAD AND UNZIP DATA #############################
if [[ $(ls data | wc -l) -ge 6 ]]; then
	echo "---------- Skip download data ----------"
else
	echo "---------- Download data ----------"
	mkdir data
	cd data
	wget https://www.dropbox.com/s/a40dhhfchojyf0c/consenus_input.zip
	unzip consenus_input.zip 
	rm consenus_input.zip
	cd ..
fi
################################################################################

########################## BUILD LIBRARIES #####################################
mkdir libs
cd libs

if [[ -d "minimap" ]]; then
	echo "---------- Skip cloning minimap ----------"
else
	echo "---------- Clone minimap ----------"
	git clone https://github.com/lh3/minimap.git
fi

cd minimap
if [[ $(ls minimap | wc -l) == 1 ]]; then
	echo "---------- Skip building minimap ----------"
else
	echo "---------- Build minimap ----------"
	make
fi
cd ..

if [[ -d "graphmap" ]]; then
	echo "---------- Skip cloning graphmap ----------"
else
	echo "---------- Clone graphmap ----------"
	git clone https://github.com/isovic/graphmap.git
fi

cd graphmap

if [[ $(find bin -type f | wc -l) == 1 ]]; then
	echo "---------- Skip building graphmap ----------"
else
	echo "---------- Build graphmap ----------"
	make modules
	make
fi

cd ../..
################################################################################


# TODO: makefile for our project
