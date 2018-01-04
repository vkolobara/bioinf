//
// Created by vkolobara on 12/29/17.
//

#include <iostream>
#include <queue>
#include "KMerGraph.h"

Edge::Edge(int position, int quality, const string &edge) : position(position), quality(quality), edge(edge) {}

bool Edge::operator<(const Edge &rhs) const {
    if (position < rhs.position)
        return true;
    if (rhs.position < position)
        return false;
    return edge < rhs.edge;
}

void Vertex::addEdge(unique_ptr<Edge> edge) {
    edges.emplace_back(edge.get());
}

Vertex::Vertex(int position, const string &kmer) : position(position), kmer(kmer), weight(INT32_MIN) {}

bool Vertex::operator<(const Vertex &rhs) const {
    if (position < rhs.position)
        return true;
    if (rhs.position < position)
        return false;
    return kmer < rhs.kmer;
}

KMerGraph::KMerGraph(int k, int g) : k(k), g(g) {}

void KMerGraph::initialGraph(string backbone) {
    vertices.clear();
    edges.clear();

    auto size = (int) backbone.size();

    Edge* edgePtr = nullptr;

    for (int i=0; i+k<=size;) {
        auto position = i;
        auto kmer = backbone.substr(i, k);

        unique_ptr<Vertex> v(new Vertex(position, kmer));


        if (edgePtr) {
            edgePtr->next = v.get();
        }

        i+=k;

        if (i+g > size) {
            vertices.insert(move(v));
            break;
        }

        auto edgeS = backbone.substr(i, g);

        unique_ptr<Edge> edge(new Edge(position, 1, edgeS));
        edgePtr = edge.get();

        v.get()->edges.emplace_back(edge.get());

        if (i==k) root = v.get();

        vertices.insert(move(v));
        edges.insert(move(edge));

        i+=g-k;
    }

}

Vertex* KMerGraph::getRoot() {
    return root;
}


// Dijkstra for max weighted path
string KMerGraph::getOptimalGenome() {
    root->weight = 0;

    queue<Vertex*> queue;
    queue.push(root);

    Vertex* vertex = root;

    while (!queue.empty()) {
        auto currentVertex = queue.front();
        queue.pop();

        if (currentVertex->weight > vertex->weight) {
            vertex = currentVertex;
        }

        for (auto edge : vertex->edges) {
            if (edge->next->weight == INT32_MIN) {
                queue.push(edge->next);
            }
            if (edge->next->weight < currentVertex->weight + edge->quality) {
                edge->next->weight = currentVertex->weight + edge->quality;
            }
        }
    }

    return "";
}
