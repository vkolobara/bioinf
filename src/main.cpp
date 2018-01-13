//
// Created by vkolobara on 12/26/17.
//

#include <iostream>
#include <fstream>
#include "format/Format.h"
#include "format/FASTA.h"
#include "algorithm/KMerGraph.h"

int main(int argc, char *argv[]) {

    if (argc != 6) {
        cerr << "5 arguments needed (k, g, path to the backbone in .fasta format, path to the mapping in .sam format, output path)" << endl;
    } else {
        int k, g;
        k = stoi(argv[1]);
        g = stoi(argv[2]);
        std::ifstream in(argv[4]);

        FASTA fasta(argv[3]);

        KMerGraph graph(k,g);
        graph.initialGraph(fasta.getSequence());

        string row;

        while (getline(in, row)) {
            if (row[0] == '@') continue;
            SAMRow samrow(row);
            if ((samrow.flag & (1<<2)) != 0) continue;
            graph.sparc(samrow);
        }

        fasta.setSequence(graph.getOptimalGenome());
        fasta.write(argv[5]);

        in.close();
    }

}
