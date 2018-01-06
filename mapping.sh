#!/bin/bash

BACKBONE_PATH=$1
READS_PATH=$2
OUTPUT_PATH=$3

libs/graphmap/bin/Linux-x64/graphmap align -r $BACKBONE_PATH -d $READS_PATH -o $OUTPUT_PATH
