//
// Created by vkolobara on 12/26/17.
//

#include <iostream>
#include "format/Format.h"
#include "format/PAF.h"
#include "format/FASTA.h"
#include "format/FASTQ.h"

int main(int argc, char *argv[]) {
    PAF paf("../data/lambda_mapping.paf");

    for (auto row : paf.getRows()) {
        cout << row.write() << endl;
    }

    cout << endl << endl;

    FASTA fasta("../data/lambda_layout.fasta");

    cout << fasta.getSequence() << endl;

    cout << endl << endl;

    FASTQ fastq("../data/lambda_reads.fastq");

    for (auto row : fastq.getReadings()) {
        cout << row.write() << endl;
    }

}