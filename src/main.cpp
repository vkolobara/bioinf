//
// Created by vkolobara on 12/26/17.
//

#include <iostream>
#include "format/Format.h"
#include "format/PAF.h"
#include "format/FASTA.h"
#include "format/FASTQ.h"
#include "algorithm/KMerGraph.h"

int main(int argc, char *argv[]) {
    FASTA fasta("../data/lambda_layout.fasta");

    KMerGraph graph(2,3);

    // graph.initialGraph("ACTGGACTAAA");

    graph.initialGraph(fasta.getSequence());
    auto root = graph.getRoot();


    while(root) {
        cout << "(" << root->position << "," << root->kmer << ")";

        if (!root->edges.empty()) {
            cout << " [" << root->edges[0]->edge << "]" << endl;
            root = root->edges[0]->next;
        } else {
            break;
        }

    }


}