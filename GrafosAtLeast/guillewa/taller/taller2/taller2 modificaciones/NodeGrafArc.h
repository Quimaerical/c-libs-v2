#ifndef NODE_GRAF_ARC_H_
#define NODE_GRAF_ARC_H_

#include <cstddef>


template <typename elem> class NodeGrafVer;

template <typename elem>
class NodeGrafArc {
private:
    float weight;
    NodeGrafVer<elem>* destination; 
    NodeGrafArc<elem>* nextArc;

public:

    NodeGrafArc() : weight(0.0f), destination(NULL), nextArc(NULL) {}


    NodeGrafArc(float w) : weight(w), destination(NULL), nextArc(NULL) {}

    NodeGrafArc(float w, NodeGrafVer<elem>* dest, NodeGrafArc<elem>* next)
        : weight(w), destination(dest), nextArc(next) {}

    float getWeight() const { return weight; }

    NodeGrafVer<elem>* getDestination() const { return destination; }

    NodeGrafArc<elem>* getNextArc() const { return nextArc; }


    void setWeight(float w) { weight = w; }

    void setDestination(NodeGrafVer<elem>* dest) { destination = dest; }

    void setNextArc(NodeGrafArc<elem>* next) { nextArc = next; }
};

#endif