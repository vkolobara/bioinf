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
    int position;
    string kmer;

    vector<Edge*> edges;

    int weight;

    Edge* returnEdge;
    Vertex* previousVertex;

    Vertex(int position, const string &kmer);

    void addEdge(Edge* edge);

};

struct VertexComp
{
    bool operator()(const Vertex* lhs, const Vertex* rhs) const;
};

struct Edge {
    int position;
    int quality;
    string edge;

    Vertex* next;

    Edge(int position, int quality, const string &edge);
};

class KMerGraph {
private:
    set<Vertex*, VertexComp> vertices;
    int k;
    int g;
    Vertex* root;

public:
    virtual ~KMerGraph();

private:
    Vertex* findBestPath();
public:
    uint G;
    uint L;

    KMerGraph(int k, int g);

    void initialGraph(string backbone);

    Vertex* getRoot();

    string getOptimalGenome();

    void sparc(SAMRow sam);
};




#endif //BIOINF_KMERGRAPH_H
