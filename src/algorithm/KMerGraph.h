//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_KMERGRAPH_H
#define BIOINF_KMERGRAPH_H

#include <vector>
#include <string>
#include <memory>
#include <set>
#include "../format/SAM.h"

using namespace std;

struct Edge;

struct Vertex {
    unsigned int position;
    string kmer;

    vector<Edge*> edges;

    int weight;

    Edge* returnEdge;
    Vertex* previousVertex;

    Vertex(unsigned int position, const string &kmer);

    void addEdge(Edge* edge);
    ~Vertex();

};

struct VertexComp
{
    bool operator()(const Vertex* lhs, const Vertex* rhs) const;
};

struct Edge {
    unsigned int position;
    int quality;
    string edge;

    Vertex* next;
    int cov;

    Edge(unsigned int position, int quality, const string &edge);
    Edge(unsigned int position, int quality, const string &edge, Vertex* next);

};

class KMerGraph {
private:
    set<Vertex*, VertexComp> vertices;
    short k;
    short g;

    Vertex* findBestPath();

public:
    KMerGraph(short k, short g);
    ~KMerGraph();

    void initialGraph(string backbone);

    Vertex* getRoot();

    string getOptimalGenome();

    void sparc(SAMRow sam);
};




#endif //BIOINF_KMERGRAPH_H
