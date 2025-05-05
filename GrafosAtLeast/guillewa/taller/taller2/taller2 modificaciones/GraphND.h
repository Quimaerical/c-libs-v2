#ifndef GRAPHND_H_
#define GRAPHND_H_

#include "NodeGrafVer.h"
#include "NodeGrafArc.h"
#include "EdgeTriple.h"
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <limits>
#include <cstddef>
#include <stdexcept>


template <typename elem>
class GraphND {

private:


    NodeGrafVer<elem>* FIND_VERTEX_PREV(NodeGrafVer<elem>* startNode, const elem& value) {
        NodeGrafVer<elem>* current = startNode;
        NodeGrafVer<elem>* previous = NULL;

        if (!current) return NULL;

        if (current->getData() == value) {
            return current;
        }

        previous = current;
        current = current->getNextVertex();

        while (current != NULL) {
            if (current->getData() == value) {
                return previous;
            }
            previous = current;
            current = current->getNextVertex();
        }
        return NULL;
    }


    NodeGrafVer<elem>* FIND_VERTEX_NODE(NodeGrafVer<elem>* startNode, const elem& value) {
        NodeGrafVer<elem>* current = startNode;
        while(current != NULL) {
            if (current->getData() == value) {
                return current;
            }
            current = current->getNextVertex();
        }
        return NULL;
    }



    NodeGrafArc<elem>* FIND_EDGE(NodeGrafArc<elem>* startArc, const elem& destinationValue) {
        NodeGrafArc<elem>* current = startArc;
        while (current != NULL) {
            if (current->getDestination() && current->getDestination()->getData() == destinationValue) {
                return current;
            }
            current = current->getNextArc();
        }
        return NULL;
    }


    bool ADD_DIRECTED_EDGE(const elem& vA, const elem& vB, float w) {
        NodeGrafVer<elem>* nodeA = FIND_VERTEX_NODE(this->prim, vA);
        NodeGrafVer<elem>* nodeB = FIND_VERTEX_NODE(this->prim, vB);

        if (!nodeA || !nodeB) return false;



        NodeGrafArc<elem>* newArc = new NodeGrafArc<elem>(w, nodeB, nodeA->getAdjList());
        nodeA->setAdjList(newArc);
        return true;
    }


    bool REMOVE_DIRECTED_EDGE(NodeGrafVer<elem>* sourceNode, const elem& targetValue) {
        if (!sourceNode || !sourceNode->getAdjList()) return false;

        NodeGrafArc<elem>* currentArc = sourceNode->getAdjList();
        NodeGrafArc<elem>* prevArc = NULL;


        if (currentArc->getDestination() && currentArc->getDestination()->getData() == targetValue) {
            sourceNode->setAdjList(currentArc->getNextArc());
            delete currentArc;
            return true;
        }


        prevArc = currentArc;
        currentArc = currentArc->getNextArc();

        while (currentArc != NULL) {
            if (currentArc->getDestination() && currentArc->getDestination()->getData() == targetValue) {
                prevArc->setNextArc(currentArc->getNextArc());
                delete currentArc;
                return true;
            }
            prevArc = currentArc;
            currentArc = currentArc->getNextArc();
        }

        return false;
    }


    void DFS_RECURSIVE(NodeGrafVer<elem>* currentVertex, std::vector<bool>& visited, std::list<elem>& path) {
        if (!currentVertex) return;

        int currentIndex = getVertexIndex(currentVertex->getData());
        if (currentIndex == -1 || visited[currentIndex]) return;

        visited[currentIndex] = true;
        path.push_back(currentVertex->getData());

        NodeGrafArc<elem>* arc = currentVertex->getAdjList();
        while (arc != NULL) {
            DFS_RECURSIVE(arc->getDestination(), visited, path);
            arc = arc->getNextArc();
        }
    }


    void LONGEST_PATH_RECURSIVE(NodeGrafVer<elem>* current, const elem& endValue, std::vector<bool>& visited, std::list<elem>& currentPath, std::list<elem>& longestPath) {
        int currentIndex = getVertexIndex(current->getData());
        visited[currentIndex] = true;
        currentPath.push_back(current->getData());

        if (current->getData() == endValue) {
            if (currentPath.size() > longestPath.size()) {
                longestPath = currentPath;
            }
        } else {
            NodeGrafArc<elem>* arc = current->getAdjList();
            while(arc != NULL) {
                int nextIndex = getVertexIndex(arc->getDestination()->getData());
                if (!visited[nextIndex]) {
                    LONGEST_PATH_RECURSIVE(arc->getDestination(), endValue, visited, currentPath, longestPath);
                }
                arc = arc->getNextArc();
            }
        }


        currentPath.pop_back();
        visited[currentIndex] = false;
    }


    void SHORTEST_PATH_RECURSIVE(NodeGrafVer<elem>* current, const elem& endValue, std::vector<bool>& visited, std::list<elem>& currentPath, std::list<elem>& shortestPath) {
        int currentIndex = getVertexIndex(current->getData());
        visited[currentIndex] = true;
        currentPath.push_back(current->getData());


        if (!shortestPath.empty() && currentPath.size() >= shortestPath.size()) {
             currentPath.pop_back();
             visited[currentIndex] = false;
             return;
        }


        if (current->getData() == endValue) {
            if (shortestPath.empty() || currentPath.size() < shortestPath.size()) {
                shortestPath = currentPath;
            }
        } else {
            NodeGrafArc<elem>* arc = current->getAdjList();
            while(arc != NULL) {
                int nextIndex = getVertexIndex(arc->getDestination()->getData());
                if (!visited[nextIndex]) {
                   SHORTEST_PATH_RECURSIVE(arc->getDestination(), endValue, visited, currentPath, shortestPath);
                }
                arc = arc->getNextArc();
            }
        }


        currentPath.pop_back();
        visited[currentIndex] = false;
    }


    void HAMILTONIAN_PATH_RECURSIVE(NodeGrafVer<elem>* current, std::vector<bool>& visited, std::list<elem>& path, bool& found) {
        if (found) return;

        int currentIndex = getVertexIndex(current->getData());
        path.push_back(current->getData());
        visited[currentIndex] = true;

        if (path.size() == numVert) {
            found = true;
            return;
        }

        NodeGrafArc<elem>* arc = current->getAdjList();
        while (arc != NULL && !found) {
             NodeGrafVer<elem>* neighbor = arc->getDestination();
             int neighborIndex = getVertexIndex(neighbor->getData());
             if (!visited[neighborIndex]) {
                 HAMILTONIAN_PATH_RECURSIVE(neighbor, visited, path, found);
             }
             arc = arc->getNextArc();
        }


        if (!found) {
            path.pop_back();
            visited[currentIndex] = false;
        }
    }


    void MIN_COST_HAMILTONIAN_RECURSIVE(NodeGrafVer<elem>* current, std::vector<bool>& visited, std::list<elem>& currentPath, float currentCost, std::list<elem>& bestPath, float& minCost) {
        int currentIndex = getVertexIndex(current->getData());
        currentPath.push_back(current->getData());
        visited[currentIndex] = true;


        if (currentCost >= minCost) {
            currentPath.pop_back();
            visited[currentIndex] = false;
            return;
        }

        if (currentPath.size() == numVert) {


             if (currentCost < minCost) {
                  minCost = currentCost;
                  bestPath = currentPath;
             }

        } else {
            NodeGrafArc<elem>* arc = current->getAdjList();
            while (arc != NULL) {
                NodeGrafVer<elem>* neighbor = arc->getDestination();
                int neighborIndex = getVertexIndex(neighbor->getData());
                if (!visited[neighborIndex]) {
                     MIN_COST_HAMILTONIAN_RECURSIVE(neighbor, visited, currentPath, currentCost + arc->getWeight(), bestPath, minCost);
                }
                arc = arc->getNextArc();
            }
        }


        currentPath.pop_back();
        visited[currentIndex] = false;
    }


    void UPDATE_MAP() {
        mapGraph.clear();
        NodeGrafVer<elem>* current = prim;
        while (current != NULL) {
            mapGraph.push_back(current->getData());
            current = current->getNextVertex();
        }
    }


public:

    std::vector<elem> mapGraph;
    NodeGrafVer<elem>* prim;
    int numVert;
    int numEdges;



    GraphND() : prim(NULL), numVert(0), numEdges(0) {}


    GraphND(const GraphND<elem>& other) : prim(NULL), numVert(0), numEdges(0) {
        copyFrom(other);
    }


    ~GraphND() {
        clear();
    }


    GraphND<elem>& operator=(const GraphND<elem>& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }


    bool isEmpty() const {
        return prim == NULL;
    }


    void addVertex(const elem& data) {
        NodeGrafVer<elem>* current = prim;
        NodeGrafVer<elem>* previous = NULL;


        while (current != NULL && current->getData() < data) {
            previous = current;
            current = current->getNextVertex();
        }


        if (current != NULL && current->getData() == data) {
            return;
        }


        NodeGrafVer<elem>* newNode = new NodeGrafVer<elem>(data);
        if (previous == NULL) {
            newNode->setNextVertex(prim);
            prim = newNode;
        } else {
            newNode->setNextVertex(current);
            previous->setNextVertex(newNode);
        }
        numVert++;
        UPDATE_MAP();
    }


    void addEdge(const elem& vA, const elem& vB, float w) {

        NodeGrafVer<elem>* nodeA = FIND_VERTEX_NODE(prim, vA);
        NodeGrafVer<elem>* nodeB = FIND_VERTEX_NODE(prim, vB);

        if (!nodeA || !nodeB) {


            return;
        }


        if (FIND_EDGE(nodeA->getAdjList(), vB)) {
            return;
        }

        bool added1 = false;
        bool added2 = false;

        if (vA == vB) {
            added1 = ADD_DIRECTED_EDGE(vA, vB, w);
            if (added1) {
                numEdges++;
            }
        } else {
            added1 = ADD_DIRECTED_EDGE(vA, vB, w);
            added2 = ADD_DIRECTED_EDGE(vB, vA, w);
            if (added1 && added2) {
                numEdges++;
            } else if (added1 && !added2) {

                 REMOVE_DIRECTED_EDGE(nodeA, vB);
            } else if (!added1 && added2) {

                  REMOVE_DIRECTED_EDGE(nodeB, vA);
            }
        }
    }


    void removeVertex(const elem& data) {
        NodeGrafVer<elem>* prevVertex = NULL;
        NodeGrafVer<elem>* toDelete = prim;


        while(toDelete != NULL && toDelete->getData() != data) {
            prevVertex = toDelete;
            toDelete = toDelete->getNextVertex();
        }

        if (!toDelete) return;


        NodeGrafVer<elem>* currentVert = prim;
        while (currentVert != NULL) {
            if (currentVert != toDelete) {
                 REMOVE_DIRECTED_EDGE(currentVert, data);

            }
            currentVert = currentVert->getNextVertex();
        }


        NodeGrafArc<elem>* currentArc = toDelete->getAdjList();
        NodeGrafArc<elem>* nextArc = NULL;
        while (currentArc != NULL) {
            nextArc = currentArc->getNextArc();


             if (currentArc->getDestination() != toDelete) {
                 numEdges--;
             } else {
                 numEdges--;
             }
             delete currentArc;
            currentArc = nextArc;
        }
        toDelete->setAdjList(NULL);


        if (prevVertex == NULL) {
            prim = toDelete->getNextVertex();
        } else {
            prevVertex->setNextVertex(toDelete->getNextVertex());
        }
        delete toDelete;
        numVert--;
        UPDATE_MAP();
    }



    void removeEdge(const elem& vA, const elem& vB) {
        NodeGrafVer<elem>* nodeA = FIND_VERTEX_NODE(prim, vA);
        NodeGrafVer<elem>* nodeB = FIND_VERTEX_NODE(prim, vB);

        if (!nodeA || !nodeB) return;

        bool removed1 = false;
        bool removed2 = false;

        if (vA == vB) {
            removed1 = REMOVE_DIRECTED_EDGE(nodeA, vB);
            if (removed1) {
                numEdges--;
            }
        } else {
             removed1 = REMOVE_DIRECTED_EDGE(nodeA, vB);
             removed2 = REMOVE_DIRECTED_EDGE(nodeB, vA);

             if (removed1 || removed2) {


                   numEdges--;
             }
        }
    }


    bool edgeExists(const elem& vA, const elem& vB) const {
         NodeGrafVer<elem>* nodeA = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, vA);
         if (!nodeA) return false;
         return const_cast<GraphND<elem>*>(this)->FIND_EDGE(nodeA->getAdjList(), vB) != NULL;
    }


    float getEdgeWeight(const elem& vA, const elem& vB) const {
        NodeGrafVer<elem>* nodeA = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, vA);
         if (!nodeA) return -1.0f;
         NodeGrafArc<elem>* arc = const_cast<GraphND<elem>*>(this)->FIND_EDGE(nodeA->getAdjList(), vB);
        return (arc != NULL) ? arc->getWeight() : -1.0f;
    }


    int order() const {
        return numVert;
    }


    int getNumVertices() const {
        return numVert;
    }


    int getNumEdges() const {
        return numEdges;
    }


    elem getFirstVertex() const {
        if (isEmpty()) {
            throw std::runtime_error("getFirstVertex() called on empty graph");
        }
        return prim->getData();
    }


    std::list<elem> getNeighbors(const elem& vertexData) const {
        std::list<elem> neighbors;
        NodeGrafVer<elem>* node = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, vertexData);
        if (node) {
            NodeGrafArc<elem>* arc = node->getAdjList();
            while (arc != NULL) {
                if (arc->getDestination()) {
                    neighbors.push_back(arc->getDestination()->getData());
                }
                arc = arc->getNextArc();
            }
        }
        return neighbors;
    }


    void clear() {
        NodeGrafVer<elem>* currentVertex = prim;
        NodeGrafVer<elem>* nextVertex = NULL;

        while (currentVertex != NULL) {
            NodeGrafArc<elem>* currentArc = currentVertex->getAdjList();
            NodeGrafArc<elem>* nextArc = NULL;

            while (currentArc != NULL) {
                nextArc = currentArc->getNextArc();
                delete currentArc;
                currentArc = nextArc;
            }

            nextVertex = currentVertex->getNextVertex();
            delete currentVertex;
            currentVertex = nextVertex;
        }
        prim = NULL;
        numVert = 0;
        numEdges = 0;
        mapGraph.clear();
    }


    bool isComplete() const {
        if (numVert < 2) return true;
        long long maxEdges = static_cast<long long>(numVert) * (numVert - 1) / 2;
        return numEdges == maxEdges;
    }


    bool isConnected() const {
        if (isEmpty() || numVert == 1) return true;

        std::list<elem> visited = bfs(getFirstVertex());
        return visited.size() == numVert;
    }


    int vertexDegree(const elem& vertexData) const {



        NodeGrafVer<elem>* node = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, vertexData);
        int degree = 0;
        if (node) {
            NodeGrafArc<elem>* arc = node->getAdjList();
            while (arc != NULL) {
                 degree++;


                arc = arc->getNextArc();
            }
        }
        return degree;


    }


    std::list<elem> getVertices() const {
        std::list<elem> vertices;
        NodeGrafVer<elem>* current = prim;
        while (current != NULL) {
            vertices.push_back(current->getData());
            current = current->getNextVertex();
        }
        return vertices;
    }


    std::list< EdgeTriple<elem> > getEdges() const {
        std::list< EdgeTriple<elem> > edges;
        std::vector<bool> visitedEdges;


        NodeGrafVer<elem>* currentVert = prim;
        while (currentVert != NULL) {
            NodeGrafArc<elem>* currentArc = currentVert->getAdjList();
            elem sourceData = currentVert->getData();
            while (currentArc != NULL) {
                 if (currentArc->getDestination()) {
                     elem destData = currentArc->getDestination()->getData();


                      edges.push_back(EdgeTriple<elem>(sourceData, destData, currentArc->getWeight()));

                 }
                currentArc = currentArc->getNextArc();
            }
            currentVert = currentVert->getNextVertex();
        }

        return edges;
    }


    void copyFrom(const GraphND<elem>& other) {
        if (this == &other) return;

        clear();

        if (other.isEmpty()) return;


        NodeGrafVer<elem>* otherCurrentVert = other.prim;
        while (otherCurrentVert != NULL) {
            this->addVertex(otherCurrentVert->getData());
            otherCurrentVert = otherCurrentVert->getNextVertex();
        }


        otherCurrentVert = other.prim;
         while (otherCurrentVert != NULL) {
             NodeGrafArc<elem>* otherCurrentArc = otherCurrentVert->getAdjList();
             elem sourceData = otherCurrentVert->getData();
             while (otherCurrentArc != NULL) {
                  if (otherCurrentArc->getDestination()) {
                      elem destData = otherCurrentArc->getDestination()->getData();




                       this->ADD_DIRECTED_EDGE(sourceData, destData, otherCurrentArc->getWeight());


                  }
                 otherCurrentArc = otherCurrentArc->getNextArc();
             }
             otherCurrentVert = otherCurrentVert->getNextVertex();
         }

         this->numEdges = other.numEdges;


    }



    void loadFromTriples(const std::list< EdgeTriple<elem> >& triples) {
        clear();



         typename std::list< EdgeTriple<elem> >::const_iterator it;
         for (it = triples.begin(); it != triples.end(); ++it) {
            addVertex(it->getVertexA());
            addVertex(it->getVertexB());
        }

        for (it = triples.begin(); it != triples.end(); ++it) {
            addEdge(it->getVertexA(), it->getVertexB(), it->getWeight());
        }
    }



    std::list<elem> bfs(const elem& startVertex) const {
        std::list<elem> path;
        std::queue<NodeGrafVer<elem>*> q;
        std::vector<bool> visited(numVert, false);

        NodeGrafVer<elem>* startNode = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, startVertex);
        if (!startNode) return path;

        int startIndex = getVertexIndex(startVertex);
        if (startIndex == -1) return path;

        q.push(startNode);
        visited[startIndex] = true;

        while (!q.empty()) {
            NodeGrafVer<elem>* current = q.front();
            q.pop();
            path.push_back(current->getData());

            NodeGrafArc<elem>* arc = current->getAdjList();
            while (arc != NULL) {
                 NodeGrafVer<elem>* neighbor = arc->getDestination();
                 if (neighbor) {
                     int neighborIndex = getVertexIndex(neighbor->getData());
                     if (neighborIndex != -1 && !visited[neighborIndex]) {
                         visited[neighborIndex] = true;
                         q.push(neighbor);
                     }
                 }
                arc = arc->getNextArc();
            }
        }
        return path;
    }



    std::list<elem> dfs(const elem& startVertex) const {
        std::list<elem> path;
        std::vector<bool> visited(numVert, false);
        NodeGrafVer<elem>* startNode = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, startVertex);

        if (startNode) {

             const_cast<GraphND<elem>*>(this)->DFS_RECURSIVE(startNode, visited, path);
        }
        return path;
    }



    std::vector<float> dijkstra(const elem& startVertex) const {
        const float INF = std::numeric_limits<float>::max();
        std::vector<float> dist(numVert, INF);


        std::vector<bool> visited(numVert, false);

        int startIndex = getVertexIndex(startVertex);
        if (startIndex == -1) {

             return dist;
        }

        dist[startIndex] = 0.0f;

        for (int count = 0; count < numVert; ++count) {

            float minDist = INF;
            int u = -1;

            for (int v_idx = 0; v_idx < numVert; ++v_idx) {
                if (!visited[v_idx] && dist[v_idx] <= minDist) {
                    minDist = dist[v_idx];
                    u = v_idx;
                }
            }

            if (u == -1) break;

            visited[u] = true;
            elem u_val = getVertexFromIndex(u);
            NodeGrafVer<elem>* u_node = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, u_val);

            if (!u_node) continue;


            NodeGrafArc<elem>* arc = u_node->getAdjList();
            while(arc != NULL) {
                NodeGrafVer<elem>* v_node = arc->getDestination();
                 if (v_node) {
                     int v_idx = getVertexIndex(v_node->getData());
                     float weight = arc->getWeight();
                     if (v_idx != -1 && !visited[v_idx] && dist[u] != INF && dist[u] + weight < dist[v_idx]) {
                         dist[v_idx] = dist[u] + weight;
                     }
                 }
                arc = arc->getNextArc();
            }
        }
        return dist;
    }



    std::list<elem> shortestPathLength(const elem& startVertex, const elem& endVertex) const {
        std::list<elem> shortestPath;
        std::list<elem> currentPath;
        std::vector<bool> visited(numVert, false);
        NodeGrafVer<elem>* startNode = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, startVertex);

        if (startNode) {
            const_cast<GraphND<elem>*>(this)->SHORTEST_PATH_RECURSIVE(startNode, endVertex, visited, currentPath, shortestPath);
        }
        return shortestPath;
    }


    std::list<elem> longestPathLength(const elem& startVertex, const elem& endVertex) const {
         std::list<elem> longestPath;
         std::list<elem> currentPath;
         std::vector<bool> visited(numVert, false);
         NodeGrafVer<elem>* startNode = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, startVertex);

         if (startNode) {
             const_cast<GraphND<elem>*>(this)->LONGEST_PATH_RECURSIVE(startNode, endVertex, visited, currentPath, longestPath);
         }
         return longestPath;
    }


    std::list<elem> findHamiltonianPath(const elem& startVertex) const {
        std::list<elem> path;
        std::vector<bool> visited(numVert, false);
        bool found = false;
        NodeGrafVer<elem>* startNode = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, startVertex);

        if (startNode) {
            const_cast<GraphND<elem>*>(this)->HAMILTONIAN_PATH_RECURSIVE(startNode, visited, path, found);
        }

        return found ? path : std::list<elem>();
    }


     std::list<elem> findMinCostHamiltonianPath(const elem& startVertex) const {
        std::list<elem> bestPath;
        std::list<elem> currentPath;
        std::vector<bool> visited(numVert, false);
        float minCost = std::numeric_limits<float>::max();
        NodeGrafVer<elem>* startNode = const_cast<GraphND<elem>*>(this)->FIND_VERTEX_NODE(prim, startVertex);

        if (startNode) {
            const_cast<GraphND<elem>*>(this)->MIN_COST_HAMILTONIAN_RECURSIVE(startNode, visited, currentPath, 0.0f, bestPath, minCost);
        }

        return (minCost == std::numeric_limits<float>::max()) ? std::list<elem>() : bestPath;
    }


    bool isEqualTo(const GraphND<elem>& other) const {
        if (this->numVert != other.numVert || this->numEdges != other.numEdges) {
            return false;
        }
        if (this->isEmpty() && other.isEmpty()) {
            return true;
        }
        if (this->mapGraph != other.mapGraph) {
            return false;
        }


        NodeGrafVer<elem>* thisCurrent = this->prim;
        NodeGrafVer<elem>* otherCurrent = other.prim;

        while(thisCurrent != NULL && otherCurrent != NULL) {
             if (thisCurrent->getData() != otherCurrent->getData()) return false;


             NodeGrafArc<elem>* thisArc = thisCurrent->getAdjList();
             std::list< std::pair<elem, float> > thisNeighbors;
             while(thisArc != NULL) {
                 if(thisArc->getDestination()) thisNeighbors.push_back(std::make_pair(thisArc->getDestination()->getData(), thisArc->getWeight()));
                 thisArc = thisArc->getNextArc();
             }

             NodeGrafArc<elem>* otherArc = otherCurrent->getAdjList();
             std::list< std::pair<elem, float> > otherNeighbors;
             while(otherArc != NULL) {
                  if(otherArc->getDestination()) otherNeighbors.push_back(std::make_pair(otherArc->getDestination()->getData(), otherArc->getWeight()));
                 otherArc = otherArc->getNextArc();
             }

             if (thisNeighbors.size() != otherNeighbors.size()) return false;


             thisNeighbors.sort();
             otherNeighbors.sort();
             if(thisNeighbors != otherNeighbors) return false;


            thisCurrent = thisCurrent->getNextVertex();
            otherCurrent = otherCurrent->getNextVertex();
        }


        return thisCurrent == NULL && otherCurrent == NULL;
    }



    const std::vector<elem>& getMap() const {
        return mapGraph;
    }


    int getVertexIndex(const elem& vertexData) const {

        for (size_t i = 0; i < mapGraph.size(); ++i) {
            if (mapGraph[i] == vertexData) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }


    elem getVertexFromIndex(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= mapGraph.size()) {
            throw std::out_of_range("Index out of range in getVertexFromIndex");
        }
        return mapGraph[index];
    }

};

#endif
