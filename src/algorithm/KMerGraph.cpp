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

bool Vertex::containsEdge(const string &edgeString) {
    for (auto edge = edges.begin(); edge != edges.end(); edge++) {
        if (edge.operator*()->edge.compare(edgeString) == 0) {
            edge.operator*()->quality++;
            return true;
        }
    }

    return false;
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

Vertex* KMerGraph::findVertexKMer(int position, Vertex* vertex, const string &kmer) {
    if (vertex->position == position && vertex->kmer.compare(kmer) == 0) {
        return vertex;
    }
    else {
        for (auto iter = vertex->edges.begin(); iter != vertex->edges.end(); iter++) {
            auto edge = iter.operator*();
            return findVertexKMer(position, edge->next, kmer);
        }
    }
}

Vertex* KMerGraph::getRoot() {
    return root;
}

void KMerGraph::sparc(PAF paf, string sequence) {
    auto pafRows = paf.getRows();

    for (auto row = pafRows.begin(); row != pafRows.end(); row++) {
        auto pafRow = row.base();

        auto targetVertex = findVertex(pafRow->targetStart, root);
        auto originVertex = targetVertex;

        int targetOffset = pafRow->targetStart - targetVertex->position;
        int offset = pafRow->targetStart - targetOffset;
        int position = pafRow->queryStart;

        string queryStartStr = sequence.substr(position, targetOffset);
        string targetStartStr = targetVertex->kmer.substr(targetVertex->kmer.size() - targetOffset, targetOffset);

        // first k-mer is good, just add edge and next vertex
        if (queryStartStr.compare(targetStartStr) == 0) {
            position += targetOffset;
            string edgeString = sequence.substr(position, g);

            if (targetVertex->containsEdge(edgeString)) {
                continue;
            }

            unique_ptr<Vertex> nextVertex(new Vertex(offset + g, edgeString.substr(g - k, k)));

            unique_ptr<Edge> edgePtr(new Edge(offset, 1, edgeString));
            auto edge = edgePtr.get();
            edge->next = nextVertex.get();

            targetVertex->edges.emplace_back(edge);

            for (auto edge = targetVertex->edges.begin(); edge != targetVertex->edges.end(); edge++) {
                cout << "origin = ("<< targetVertex->position << ", "<< targetVertex->kmer << ")\t\tedge = " <<
                     edge.operator*()->edge << "\t\tnext = (" << edge.operator*()->next->position <<", " << edge.operator*()->next->kmer << ")" << endl;
            }

            targetVertex = nextVertex.get();
            position += k;

            vertices.insert(move(nextVertex));
            edges.insert(move(edgePtr));
        }
        else {
            continue;
        }

        // process the rest of sequence
        while (position <= pafRow->queryEnd) {
            auto edgeString = sequence.substr(position, g);

            if (targetVertex->containsEdge(edgeString)) {
                position += g;
                continue;
            }

            auto nextKmer = edgeString.substr(g - k, k);

            auto nextVertex = findVertexKMer(targetVertex->position + g, originVertex, nextKmer);

            if (nextVertex == NULL) {
                unique_ptr<Vertex> temp(new Vertex(targetVertex->position + g, nextKmer));
                nextVertex = temp.get();
                vertices.insert(move(temp));
            }

            unique_ptr<Edge> edgePtr(new Edge(targetVertex->position, 1, edgeString));
            auto edge = edgePtr.get();
            targetVertex->edges.emplace_back(edge);
            edge->next = nextVertex;

            edges.insert(move(edgePtr));

            targetVertex = nextVertex;

            position += g;
        }

        break;
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
