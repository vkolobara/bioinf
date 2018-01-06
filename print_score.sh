#!/bin/bash

mkdir results
echo "------------------------------"
cd results
../libs/MUMmer3.23/dnadiff ../$1 ../$2 &>/dev/null
cat out.report | grep AvgId | tr -s ' ' | cut -d' ' -f2 | head -1
echo "------------------------------"
cd ..
rm -rf results
