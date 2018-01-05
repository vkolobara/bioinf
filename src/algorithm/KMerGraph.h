//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_KMERGRAPH_H
#define BIOINF_KMERGRAPH_H

#include <vector>
#include <string>
#include <memory>
#include <set>
#include "../format/PAF.h"
#include "../format/FASTA.h"

using namespace std;

struct Edge;

struct Vertex {
    int position;
    string kmer;
    vector<Edge*> edges;
    int weight;
    Edge* returnEdge;
    Vertex* previousVertex;

    Vertex(int position, const string &kmer);

    void addEdge(unique_ptr<Edge> edge);

    bool containsEdge(const string &edgeString, int quality);

    bool operator<(const Vertex &rhs) const;

};

struct Edge {
    int position;
    int quality;
    string edge;

    Vertex* next;

    bool operator<(const Edge &rhs) const;

    Edge(int position, int quality, const string &edge);
};

class KMerGraph {
private:
    int k;
    int g;
    Vertex* root;
    set<unique_ptr<Vertex>> vertices;
    set<unique_ptr<Edge>> edges;
    Vertex* findBestPath();
public:
    KMerGraph(int k, int g);

    void initialGraph(string backbone);

    Vertex* findVertex(int position, Vertex* vertex);

    Vertex* findVertexKMer(int position, Vertex* vertex, const string &kmer);

    void sparc(PAF paf, string sequence);

    Vertex* getRoot();

    string getOptimalGenome();

};




#endif //BIOINF_KMERGRAPH_H
