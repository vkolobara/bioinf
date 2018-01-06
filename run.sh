#!/bin/bash

NUM_ITER=2
INSTANCE="lambda"

if [[ $# -eq 0 ]]; then
	echo "No parameters specified, running 2 iterations on lambda"
else 
	NUM_ITER=$1
	INSTANCE=$2
fi

BACKBONE_PATH="data/"$INSTANCE"_layout.fasta"
READS_PATH="data/"$INSTANCE"_reads.fastq"
MAPPING_PATH="data/"$INSTANCE"_mapping.sam"
OUTPUT_PATH="data/"$INSTANCE"_sparc_output.fasta"
REFERENCE_PATH="data/"$INSTANCE"_reference.fasta"

./mapping.sh $BACKBONE_PATH $READS_PATH $MAPPING_PATH &>/dev/null

echo "BASE SCORE"
./print_score.sh $BACKBONE_PATH $REFERENCE_PATH

for i in $(eval echo {1..$NUM_ITER})
do
	./build/bioinf $BACKBONE_PATH $MAPPING_PATH $OUTPUT_PATH
	echo "SCORE at iteration "$i
	./print_score.sh $OUTPUT_PATH $REFERENCE_PATH
	./mapping.sh $OUTPUT_PATH $READS_PATH $MAPPING_PATH &>/dev/null
done
