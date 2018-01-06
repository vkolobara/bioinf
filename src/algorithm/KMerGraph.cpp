//
// Created by vkolobara on 12/29/17.
//

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include "KMerGraph.h"

Edge::Edge(int position, int quality, const string &edge) : position(position), quality(quality), edge(edge) {}

void Vertex::addEdge(Edge *edge) {
    edges.push_back(edge);
}

Vertex::Vertex(int position, const string &kmer) : position(position), kmer(kmer), weight(-1) {}

KMerGraph::KMerGraph(int k, int g) : k(k), g(g) {}

void KMerGraph::initialGraph(string backbone) {
    vertices.clear();

    auto size = (uint) backbone.size();

    Edge *edge = nullptr;

    for (uint i = 0; i + k <= size;) {
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

        edge = new Edge(position, 1, edgeS);

        v->edges.push_back(edge);

        edge->next=v;

        i += g-k;
    }

    root = *vertices.begin();

    G = backbone.size();

}

Vertex *KMerGraph::getRoot() {
    return root;
}

void KMerGraph::sparc(SAMRow row) {
    L+=row.alignment.size();
    auto pos = row.pos-1;
    uint offset = 0;

    while (pos % g != 0) {
        pos++;
        offset++;
    }

    auto kmer = row.alignment.substr(offset, k);
    auto curr = new Vertex(pos, kmer);

    set<Vertex *, VertexComp>::iterator it;

    if ((it = vertices.find(curr)) == vertices.end()) {
        vertices.insert(curr);
    } else {
        delete curr;
        curr = *it;
    }

    for (int index = k + offset; index + g <= row.alignment.size(); index += g) {
        string edgeStr;

        if (row.insertions.find(index) != row.insertions.end()) {
            edgeStr = row.insertions[index] + row.alignment.substr(index, g);;
        } else {
            edgeStr = row.alignment.substr(index, g);
        }

        auto kmerNext = edgeStr.substr(g - k, k);

        auto edgeQ = 0;

        /*for (int i=0; i<g; i++) {
            edgeQ += (int) row.qual[index+i];
        }*/

        edgeQ = 15;

        auto nextVertex = new Vertex(curr->position + g, kmerNext);

        if ((it = vertices.find(nextVertex)) == vertices.end()) {
            vertices.insert(nextVertex);
        } else {
            delete nextVertex;
            nextVertex = *it;
        }

        bool flag = true;
        for (auto edge : curr->edges) {
            if (edge->edge == edgeStr) {
                flag = false;
                edge->quality += edgeQ;
            }
        }

        if (flag) {
            auto edge = new Edge(curr->position, edgeQ, edgeStr);
            edge->next = nextVertex;
            curr->addEdge(edge);
            //edges.insert(edge);
        }

        curr = nextVertex;
    }
}

KMerGraph::~KMerGraph() {
    for (auto vertex : vertices) {
        for (auto edge : vertex->edges) {
            if (edge) delete edge;
        }

        delete vertex;
    }

    vertices.clear();

}


Vertex *KMerGraph::findBestPath() {
    auto c = max (2, (int) (0.2 * L/G));

    for (auto v : vertices) {
        for (auto e : v->edges) {
            e->quality -= c;
        }
    }

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

    while (vertex != root && vertex->previousVertex && vertex->returnEdge) {
        stringStack.push(vertex->returnEdge->edge);
        vertex = vertex->previousVertex;
    }

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
