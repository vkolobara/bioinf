#!/bin/bash

INSTANCE="lambda"
MAPPER="graphmap"

if [[ $# -eq 0 ]]; then
	echo "No parameters specified, using lambda with graphmap"

else
	if [[ $# -eq 1 ]]; then
		INSTANCE=$1
	else
		INSTANCE=$1
		MAPPER=$2
	fi
fi

if [[ $MAPPER -eq "graphmap" ]]; then
	libs/graphmap/bin/Linux-x64/graphmap align -r data/"$INSTANCE"_layout.fasta -d data/"$INSTANCE"_reads.fastq -o data/"$INSTANCE"_mapping.sam
	# TODO: CONVERT .sam to .paf
else
	libs/minimap/minimap data/"$INSTANCE"_layout.fasta data/"$INSTANCE"_reads.fastq > data/"$INSTANCE"_mapping.paf
fi
