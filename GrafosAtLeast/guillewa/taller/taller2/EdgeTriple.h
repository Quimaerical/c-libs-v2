#ifndef EDGE_TRIPLE_H_
#define EDGE_TRIPLE_H_

#include <cstddef>

template <typename elem>
class EdgeTriple {
private:
    elem vertexA;
    elem vertexB;
    float weight;

public:

    EdgeTriple() : weight(0.0f) {}

    EdgeTriple(elem vA, elem vB, float w) : vertexA(vA), vertexB(vB), weight(w) {}

    elem getVertexA() const { return vertexA; }
    elem getVertexB() const { return vertexB; }
    float getWeight() const { return weight; }

    void setVertexA(elem vA) { vertexA = vA; }
    void setVertexB(elem vB) { vertexB = vB; }
    void setWeight(float w) { weight = w; }

    bool operator==(const EdgeTriple<elem>& other) const {
         return ( (this->vertexA == other.vertexA && this->vertexB == other.vertexB) ||
                  (this->vertexA == other.vertexB && this->vertexB == other.vertexA)
                ) && this->weight == other.weight;

    }
};

#endif