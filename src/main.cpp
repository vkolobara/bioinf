//
// Created by vkolobara on 12/26/17.
//

#include <iostream>
#include <fstream>
#include "format/Format.h"
#include "format/FASTA.h"
#include "algorithm/KMerGraph.h"

int main(int argc, char *argv[]) {

    if (argc != 4) {
        cerr << "3 arguments needed (path to the backbone (.fasta), path to the mapping (.sam), output path .fasta)" << endl;
    } else {
        std::ifstream in(argv[2]);
        std::cin.rdbuf(in.rdbuf());

        FASTA fasta(argv[1]);

        KMerGraph graph(3, 4);
        graph.initialGraph(fasta.getSequence());

        string row;

        while (getline(cin, row)) {
            if (row[0] == '@') continue;
            SAMRow samrow(row);
            if (samrow.flag == 4) continue;
            graph.sparc(samrow);
        }

        fasta.setSequence(graph.getOptimalGenome());
        fasta.write(argv[3]);

    }

}
