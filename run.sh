#!/bin/bash

K=3
G=4
NUM_ITER=2
INSTANCE="lambda"

if [[ $# -eq 4 ]]; then
	K=$1
	G=$2
	NUM_ITER=$3
	INSTANCE=$4
fi

BACKBONE_PATH="data/"$INSTANCE"_layout.fasta"
READS_PATH="data/"$INSTANCE"_reads.fastq"
MAPPING_PATH="data/"$INSTANCE"_mapping.sam"
OUTPUT_PATH="data/"$INSTANCE"_sparc_output.fasta"
REFERENCE_PATH="data/"$INSTANCE"_reference.fasta"

echo "BASE SCORE"
./print_score.sh $BACKBONE_PATH $REFERENCE_PATH

for i in $(eval echo {1..$NUM_ITER})
do
	echo "MAPPING "$READS_PATH "on "$BACKBONE_PATH
	./mapping.sh $BACKBONE_PATH $READS_PATH $MAPPING_PATH &>/dev/null
	echo "MAPPING DONE"
	echo "RUNNING SPARC ALGORITHM"
	#sudo ./libs/cgmemtime/cgmemtime ./build/bioinf $K $G $BACKBONE_PATH $MAPPING_PATH $OUTPUT_PATH
	./build/bioinf $K $G $BACKBONE_PATH $MAPPING_PATH $OUTPUT_PATH
	echo "SPARC ALGORITHM DONE"
	echo "SCORE at iteration "$i
	./print_score.sh $OUTPUT_PATH $REFERENCE_PATH
	BACKBONE_PATH=$OUTPUT_PATH
	rm data/*.gmidx
done
