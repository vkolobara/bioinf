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

Vertex* KMerGraph::findVertex(int position, Vertex* vertex) {
    if (vertex->position <= position && vertex->position + g > position) {
        return vertex;
    }
    else {
        for (auto iter = vertex->edges.begin(); iter != vertex->edges.end(); iter++) {
            auto edge = iter.operator*();
            return findVertex(position, edge->next);
        }
    }
}

Vertex* KMerGraph::getRoot() {
    return root;
}

void KMerGraph::sparcConsensus(PAF paf, string sequence) {
    auto pafRows = paf.getRows();

    for (auto row = pafRows.begin(); row != pafRows.end(); row++) {
        auto pafRow = row.base();

        auto targetVertex = findVertex(pafRow->targetStart, root);

        int targetOffset = pafRow->targetStart - targetVertex->position;
        int position = pafRow->queryStart;

        string queryStartTransition = sequence.substr(position, g);

        while (position <= pafRow->queryEnd) {
            auto kmer = sequence.substr(position, k);

            if (kmer.compare(targetVertex->kmer) != 0 && targetVertex->position != position) {
                auto vertex= new Vertex(targetVertex->position, kmer);
                vertices.emplace(unique_ptr<Vertex>(vertex));

                vertex->addEdge(unique_ptr<Edge>(new Edge(vertex->position, 1, sequence.substr(position + targetOffset, g))));
            }
            else {
                targetVertex->addEdge(unique_ptr<Edge>(new Edge(targetVertex->position, 1, sequence.substr(position + targetOffset, g))));
            }

            position += g;


        }

        cout<<endl;
    }
}

void KMerGraph::calculateLongestPath() {
    root->weight = 0;

    queue<Vertex*> queue;
    queue.push(root);

    while (!queue.empty()) {
        auto currentVertex = queue.front();
        queue.pop();

        for (auto edge : currentVertex->edges) {
            if (edge->next->weight == INT32_MIN) {
                queue.push(edge->next);
            }

            if (edge->next->weight < currentVertex->weight + edge->quality) {
                edge->next->weight = currentVertex->weight + edge->quality;
                currentVertex->bestEdge = edge;
            }
        }
    }
}

string KMerGraph::getOptimalGenome() {
    calculateLongestPath();
    Vertex* vertex = root;

    string genome = root->kmer;

    while (vertex->bestEdge != NULL) {
        genome += vertex->bestEdge->edge;
        vertex = vertex->bestEdge->next;
    }

    return genome;
}
