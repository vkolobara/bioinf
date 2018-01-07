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
cd ..

if [ -d "MUMmer3.23" ]; then
	echo "---------- Skip downloading mummer ----------"
else
	echo "---------- Download mummer ----------"
	wget -O mummer.tar.gz https://sourceforge.net/projects/mummer/files/mummer/3.23/MUMmer3.23.tar.gz
	tar -xzvf mummer.tar.gz
	rm mummer.tar.gz
fi

cd MUMmer3.23
if [ -f "dnadiff" ]; then
	echo "---------- Skip building mummer ----------"
else
	echo "---------- Build mummer ----------"
	make
fi
cd ..


if [ -d "cgmemtime" ]; then
	echo "---------- Skip downloading cgmemtime ----------"
else
	echo "---------- Download cgmemtime ----------"
	git clone https://github.com/gsauthof/cgmemtime.git
fi

cd cgmemtime
if [ -f "cgmemtime" ]; then
	echo "---------- Skip building cgmemtime ----------"
else
	echo "---------- Build cgmemtime ----------"
	make
fi

cd ../..
################################################################################

########################## BUILD OUR SPARC #####################################
mkdir build
cd build
cmake ..
make
