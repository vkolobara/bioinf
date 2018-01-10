//
// Created by vkolobara on 12/29/17.
//

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include "KMerGraph.h"

Edge::Edge(unsigned int position, int quality, const string &edge) : position(position), quality(quality), edge(edge) {
	next = nullptr;
}

Edge::Edge(unsigned int position, int quality, const string &edge, Vertex* next) : position(position), quality(quality), edge(edge), next(next) {}

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

        if (edge) {
            edge->next = v;
        }

        vertices.insert(v);

        i += k;

        if (i + g > size) {
            break;
        }

        auto edgeS = backbone.substr(i, g);

        edge = new Edge(position, 0, edgeS);

        v->edges.push_back(edge);

        edge->next=v;

        i += g-k;
    }

}

Vertex *KMerGraph::getRoot() {
    return *vertices.begin();
}

void KMerGraph::sparc(SAMRow row) {
    row.pos--;
    unsigned int offset = 0;

    while (row.pos % g != 0) {
        row.pos++;
        offset++;
    }

    auto kmer = row.alignment.substr(unsigned int, k);
    auto curr = new Vertex(row.pos, kmer);

    set<Vertex *, VertexComp>::iterator it;

    auto ret = vertices.insert(curr);
    if (!ret.second) {
        delete curr;
    }
    curr = *ret.first;

    for (unsigned int index = k + offset; index + g <= row.alignment.size(); index += g) {
        string edgeStr;

        if (row.insertions.find(index) != row.insertions.end()) {
            edgeStr = row.insertions[index] + row.alignment.substr(index, g);;
        } else {
            edgeStr = row.alignment.substr(index, g);
        }

        auto kmerNext = edgeStr.substr(g - k, k);

        short edgeQ = 15;

        auto nextVertex =new Vertex((unsigned int) curr->position + g, kmerNext);

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
            }
        }

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


Vertex* KMerGraph::findBestPath() {

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
