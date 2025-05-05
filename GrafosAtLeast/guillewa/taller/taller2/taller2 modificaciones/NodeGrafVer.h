#ifndef NODE_GRAF_VER_H_
#define NODE_GRAF_VER_H_

#include "NodeGrafArc.h"
#include <cstddef>
#include <limits>

template <typename elem>
class NodeGrafVer {
private:
    elem data;
    NodeGrafVer<elem>* nextVertex;
    NodeGrafArc<elem>* adjList;

public:

    NodeGrafVer() : nextVertex(NULL), adjList(NULL) {

        if (std::numeric_limits<elem>::is_specialized) {
             data = elem();
        }

    }

    NodeGrafVer(elem d) : data(d), nextVertex(NULL), adjList(NULL) {}

    NodeGrafVer(elem d, NodeGrafVer<elem>* next, NodeGrafArc<elem>* adj)
        : data(d), nextVertex(next), adjList(adj) {}

    elem getData() const { return data; }
    NodeGrafVer<elem>* getNextVertex() const { return nextVertex; }
    NodeGrafArc<elem>* getAdjList() const { return adjList; }

    void setData(elem d) { data = d; }
    void setNextVertex(NodeGrafVer<elem>* next) { nextVertex = next; }
    void setAdjList(NodeGrafArc<elem>* adj) { adjList = adj; }
};

#endif