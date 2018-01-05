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
    PAF paf("../data/lambda_mapping.paf");

    KMerGraph graph(2,3);
    graph.initialGraph(fasta.getSequence());

    graph.sparcConsensus(paf, fasta.getSequence());

    auto root = graph.getRoot();
//    fasta.sequence = graph.getOptimalGenome();
  //  fasta.write("../data/lambda_our_output.fasta");

    while(root) {
        cout << "(" << root->position << "," << root->kmer << ")";
        //cout << root->weight;

        if (!root->edges.empty()) {
            cout << " [";
            for(auto edge = root->edges.begin(); edge != root->edges.end(); edge++) {
                cout << edge.operator*()->edge <<",";
            }
            cout << "]" << endl;
            root = root->edges[0]->next;
        } else {
            break;
        }

    }


}