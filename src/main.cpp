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
    //PAF paf("../data/lambda_mapping.paf");

    KMerGraph graph(2,3);

    // graph.initialGraph("ACTGGACTAAA");

    graph.initialGraph(fasta.getSequence());
    auto root = graph.getRoot();
    fasta.sequence = graph.getOptimalGenome();
    fasta.write("../data/lambda_our_output.fasta");

    while(root) {
        cout << "(" << root->position << "," << root->kmer << ")";
        cout << root->weight;

        if (!root->edges.empty()) {
            cout << " [" << root->edges[0]->edge << "]" << endl;
            root = root->edges[0]->next;
        } else {
            break;
        }

    }


}