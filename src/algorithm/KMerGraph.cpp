//
// Created by vkolobara on 12/29/17.
//

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include "KMerGraph.h"

Edge::Edge(unsigned int position, int quality, const string &edge) : position(position), quality(quality), edge(edge), cov(1) {
    next = nullptr;
}

Edge::Edge(unsigned int position, int quality, const string &edge, Vertex *next) : position(position), quality(quality),
                                                                                   edge(edge), next(next), cov(1) {}

void Vertex::addEdge(Edge *edge) {
    edges.push_back(edge);
}

Vertex::Vertex(unsigned int position, const string &kmer) : position(position), kmer(kmer), weight(-1) {
    returnEdge = nullptr;
    previousVertex = nullptr;
}

Vertex::~Vertex() {
    returnEdge = nullptr;
    previousVertex = nullptr;

    for (auto edge : edges) {
        delete edge;
    }

    edges.clear();
}

KMerGraph::KMerGraph(short k, short g) : k(k), g(g) {
}

void KMerGraph::initialGraph(string backbone) {
    vertices.clear();

    auto size = (unsigned int) backbone.size();

    Edge *edge = nullptr;

    for (unsigned int i = 0; i + k <= size;) {
        auto position = i;
        auto kmer = backbone.substr(i, k);

        auto v = new Vertex(position, kmer);
        vertices.insert(v);

        if (edge) {
            edge->next = v;
        }

        i += k;
        if (i + g > size) {
            break;
        }

        auto edgeS = backbone.substr(i, g);
        edge = new Edge(position, 1, edgeS);
        v->edges.push_back(edge);

        i += g - k;
    }

}

Vertex *KMerGraph::getRoot() {
    return *vertices.begin();
}

void KMerGraph::sparc(SAMRow row) {
    row.pos--;

    unsigned int offset = 0;

    // Align to the first kmer in backbone
    while (row.pos % g != 0) {
        row.pos++;
        offset++;
    }

    string kmer;
    kmer.reserve(k);

    short kmerIndex = 0;

    for (unsigned int i = offset; i < row.alignment.size() && kmerIndex < k; i++) {
        if (row.alignment[i] != '-') kmer[kmerIndex++] = row.alignment[i];
    }

    auto curr = new Vertex(row.pos, kmer);

    set<Vertex *, VertexComp>::iterator it;

    auto ret = vertices.insert(curr);
    if (!ret.second) {
        delete curr;
    }

    curr = *ret.first;

    for (unsigned int index = offset; index + k + g <= row.alignment.size(); index += g) {
        string edgeStr;

        edgeStr = row.alignment.substr(index + k, g);

        string kmerNext;
        kmerIndex = 0;
        kmerNext.reserve(k);

        for (unsigned int i = index + g; i < row.alignment.size() && kmerIndex < k; i++) {
            if (row.alignment[i] != '-') kmerNext[kmerIndex++] = row.alignment[i];
        }

        // If the kmer is not complete, break
        if (kmerIndex != k) break;

        // Calculate link multiplicity as the average quality
        int edgeQ = 0;
        for (int i = 0; i < g; i++) {
            edgeQ += (int) row.qual[index + k + i] - 33;
        }
        edgeQ /= edgeStr.size();


        auto nextVertex = new Vertex(curr->position + g, kmerNext);

        auto ret = vertices.insert(nextVertex);
        if (!ret.second) {
            delete nextVertex;
        }
        nextVertex = *ret.first;


        bool flag = true;
        for (auto edge : curr->edges) {
            if (edge->edge == edgeStr) {
                flag = false;
                edge->quality += edgeQ;
                edge->cov++;
            }
        }

        // If edge doesn't already exist, create a new one and assign it to the vertex
        if (flag) curr->addEdge(new Edge(curr->position, edgeQ, edgeStr, nextVertex));

        curr = nextVertex;
    }
}

KMerGraph::~KMerGraph() {
    for (auto vertex : vertices) {
        delete vertex;
    }

    vertices.clear();

}


Vertex *KMerGraph::findBestPath() {

    // Do the weight update
    for (auto vertex : vertices) {
        for (auto edge : vertex->edges) {
            edge->quality -= max(5, (int) 0.1 * edge->cov);
        }
    }

    auto root = getRoot();

    queue<Vertex *> queue;
    queue.push(root);

    root->weight = 0;
    auto top = root;

    while (!queue.empty()) {
        auto currentVertex = queue.front();
        queue.pop();

        if (currentVertex->weight > top->weight) {
            top = currentVertex;
        }

        for (auto edge : currentVertex->edges) {
            if (edge->next->weight == -1) {
                queue.push(edge->next);
            }

            if (edge->next->weight < currentVertex->weight + edge->quality) {
                edge->next->weight = currentVertex->weight + edge->quality;
                edge->next->previousVertex = currentVertex;
                edge->next->returnEdge = edge;
            }
        }
    }

    return top;
}


string KMerGraph::getOptimalGenome() {
    auto vertex = findBestPath();
    stack<string> stringStack;

    while (vertex->previousVertex && vertex->returnEdge) {
        stringStack.push(vertex->returnEdge->edge);
        vertex = vertex->previousVertex;
    }
    auto root = getRoot();

    string genome = root->kmer;

    while (!stringStack.empty()) {
        genome += stringStack.top();
        stringStack.pop();
    }

    genome.erase(remove(genome.begin(), genome.end(), '-'), genome.end());
    return genome;
}

bool VertexComp::operator()(const Vertex *lhs, const Vertex *rhs) const {
    if (lhs->position < rhs->position)
        return true;
    if (rhs->position < lhs->position)
        return false;
    return lhs->kmer < rhs->kmer;
}
