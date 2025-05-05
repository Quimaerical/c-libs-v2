#include "GraphND.h"
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <climits>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class BnBNode {
    private:
        int bitmask;            // Bitmask de nodos dominados
        vector<int> d;          // Índices de nodos en el conjunto dominante (lowerCamelCase)
        int nextIdx;            // Siguiente nodo a considerar
        int lowerBound;         // Cota inferior estimada
    
    public:
        // Constructor por defecto
        BnBNode() : bitmask(0), nextIdx(0), lowerBound(0) {}
        // Getters
        int getBitmask() const { 
            return bitmask; 
        }
        const vector<int>& getd() const { 
            return d; 
        } // Getter const
        vector<int>& getd() { 
            return d; 
        }             // Getter no-const para modificar
        int getNextIdx() const { 
            return nextIdx; 
        }
        int getLowerBound() const { 
            return lowerBound; 
        }
    
        // Setters
        void setBitmask(int newBitmask) { 
            bitmask = newBitmask; 
        }
        void setNextIdx(int newNextIdx) { 
            nextIdx = newNextIdx; 
        }
        void setLowerBound(int newLowerBound) { 
            lowerBound = newLowerBound; 
        }
        // No hay setter directo para 'd', se modifica a través de la referencia obtenida con getd()
    };


// --- Precomputar datos del grafo ---
void precomputeGraphData(GraphND<string>& graph, vector<string>& vertices, vector< vector<int> >& adj, vector<int>& degrees, vector<int>& sortedIndices ) {
    list<string> vertexList = graph.getVertices();
    vertices.assign(vertexList.begin(), vertexList.end());
    int n = vertices.size();

    // Inicializar estructuras
    adj.resize(n);
    degrees.resize(n, 0);
    sortedIndices.resize(n);

    // Llenar lista de adyacencia y grados
    for (int i = 0; i < n; ++i) {
        list<string> neighbors = graph.getNeighbors(vertices[i]);
        for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
            int j = find(vertices.begin(), vertices.end(), *it) - vertices.begin();
            adj[i].push_back(j);
            degrees[i]++;
        }
        sortedIndices[i] = i;
    }

    // Ordenar nodos por grado (descendente)
    sort(sortedIndices.begin(), sortedIndices.end(), [&](int a, int b) {
        return (degrees[a] > degrees[b]);
    });
}

// --- Verificar si un bitmask domina todos los nodos ---
bool isDominatingSet(int bitmask, vector<string>& vertices) {
    int n = vertices.size();
    int allCovered = (1 << n) - 1;
    return (bitmask & allCovered) == allCovered;
}

// --- Calcular cota inferior para poda ---
int computeLowerBound(const vector<int>& D, int bitmask, vector<string>& vertices, vector<int>& degrees) {
    int remaining = vertices.size() - __builtin_popcount(bitmask);
    if (remaining <= 0) return D.size();

    // Heurística: nodos no cubiertos / grado máximo + 1
    int maxDegree = 1;
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (!(bitmask & (1 << i))) {
            if (degrees[i] + 1 > maxDegree) {
                maxDegree = degrees[i] + 1;
            }
        }
    }
    return D.size() + (remaining + maxDegree - 1) / maxDegree;
}

bool compareNodes (const BnBNode& a, const BnBNode& b) {
    return a.getLowerBound() > b.getLowerBound();
}

// --- Algoritmo Branch and Bound optimizado ---
vector<string> findMDS_BranchAndBound(GraphND<string>& graph, vector<string>& vertices, vector< vector<int> >& adj, vector<int>& degrees, vector<int>& sortedIndices) {
    precomputeGraphData(graph, vertices, adj, degrees, sortedIndices);
    int n = vertices.size();

    // Inicializar mejor solución con greedy
    vector<string> bestD;
    int bestSize = INT_MAX;

    // Cola de prioridad para BnB
    vector<BnBNode> heap;
    BnBNode root;
    root.setBitmask(0);
    root.setNextIdx(0);
    root.setLowerBound(computeLowerBound(root.getd(), root.getBitmask(), vertices, degrees));
    heap.push_back(root);

    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), compareNodes);
        BnBNode current = heap.back();
        heap.pop_back();

        // Poda: si la cota es peor que la mejor solución
        if (current.getLowerBound() >= bestSize) continue;

        // Caso base: todos los nodos considerados
        if (current.getNextIdx() == n) {
            if (isDominatingSet(current.getBitmask(), vertices)) {
                if (current.getd().size() < bestSize) {
                    bestSize = current.getd().size();
                    bestD.clear();
                    for (size_t i = 0; i < current.getd().size(); ++i) {
                        bestD.push_back(vertices[current.getd()[i]]);
                    }
                }
            }
            continue;
        }

        int node = sortedIndices[current.getNextIdx()];

        // Opción 1: Excluir el nodo
        BnBNode exclude = current;
        exclude.setNextIdx(exclude.getNextIdx() + 1);
        exclude.setLowerBound(computeLowerBound(exclude.getd(), exclude.getBitmask(), vertices, degrees));
        if (exclude.getLowerBound() < bestSize) {
            heap.push_back(exclude);
            push_heap(heap.begin(), heap.end(), compareNodes);
        }

        // Opción 2: Incluir el nodo
        BnBNode include = current;
        include.getd().push_back(node);
        include.setBitmask(include.getBitmask() | (1 << node));
        for (size_t i = 0; i < adj[node].size(); ++i) {
            include.setBitmask(include.getBitmask() | (1 << adj[node][i]));
        }
        include.setNextIdx(include.getNextIdx() + 1);
        include.setLowerBound(computeLowerBound(include.getd(), include.getBitmask(), vertices, degrees));
        if (include.getLowerBound() < bestSize) {
            heap.push_back(include);
            push_heap(heap.begin(), heap.end(), compareNodes);
        }
    }

    sort(bestD.begin(), bestD.end());
    return bestD;
}

// // --- Algoritmo Greedy (para comparación) ---
// vector<string> findMinDominatingSet_Greedy(GraphND<string>& graph) {
//     list<string> vertexList = graph.getVertices();
//     vector<string> vertices(vertexList.begin(), vertexList.end());
//     int n = vertices.size();
//     vector<bool> covered(n, false);
//     vector<string> D;

//     while (true) {
//         int maxCover = -1;
//         int bestNode = -1;

//         for (int i = 0; i < n; ++i) {
//             // Se cambió la lógica para considerar nodos aún no en D,
//             // y contar cuántos *nuevos* nodos cubriría si se añade.
//             bool alreadyInD = false;
//             for(size_t k = 0; k < D.size(); ++k) {
//                 if (vertices[i] == D[k]) {
//                     alreadyInD = true;
//                     break;
//                 }
//             }
//             if (alreadyInD) continue; // Si ya está en D, no lo volvemos a considerar

//             int count = 0;
//             // Contar el nodo i si no está cubierto
//             if (!covered[i]) count++;

//             // Contar vecinos no cubiertos
//             list<string> neighbors = graph.getNeighbors(vertices[i]);
//             for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
//                 int j = find(vertices.begin(), vertices.end(), *it) - vertices.begin();
//                 if (!covered[j]) count++;
//             }

//             if (count > maxCover) {
//                 maxCover = count;
//                 bestNode = i;
//             }
//         }

//         // Si maxCover es 0, significa que todos los nodos restantes ya están cubiertos
//         // o no hay nodos que puedan cubrir a otros nuevos.
//         if (bestNode == -1 || maxCover == 0) {
//              // Verificar si todos los nodos están cubiertos antes de salir
//             bool allNodesCovered = true;
//             for(int i = 0; i < n; ++i) {
//                 if (!covered[i]) {
//                     allNodesCovered = false;
//                     // Si hay un nodo no cubierto, forzar su inclusión
//                     // (Esto podría no ser óptimo, pero asegura cobertura)
//                     // Una mejor estrategia greedy podría requerirse aquí.
//                     // Para este caso simple, añadimos el primero no cubierto.
//                     bestNode = i;
//                     break;
//                 }
//             }
//             if (allNodesCovered || bestNode == -1) break; // Salir si todo está cubierto o no se puede mejorar
//         }


//         D.push_back(vertices[bestNode]);
//         covered[bestNode] = true;
//         list<string> neighbors = graph.getNeighbors(vertices[bestNode]);
//         for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
//             int j = find(vertices.begin(), vertices.end(), *it) - vertices.begin();
//             covered[j] = true;
//         }
//     }

//     sort(D.begin(), D.end());
//     return D;
// }


// --- Función Principal ---
int main() {
    GraphND<string> cityGraph;
    string line, u, v;
    set<string> vertexSet;
    vector<pair<string, string>> edges;
    vector<string> vertices;
    vector< vector<int> > adj;
    vector<int> degrees;
    vector<int> sortedIndices;

    // Leer entrada
    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        if (ss >> u >> v) {
            vertexSet.insert(u);
            vertexSet.insert(v);
            edges.push_back(make_pair(u, v));
        }
    }

    // Construir grafo
    for (set<string>::iterator it = vertexSet.begin(); it != vertexSet.end(); ++it) {
        cityGraph.addVertex(*it);
    }
    for (size_t i = 0; i < edges.size(); ++i) {
        cityGraph.addEdge(edges[i].first, edges[i].second, 0.0f); // Asumiendo no ponderado o peso irrelevante
    }

    int numVertices = cityGraph.order();
    vector<string> resultDominatingSet;

    // Decide qué algoritmo usar basado en el tamaño del grafo
    // if (numVertices <= 20) { // Umbral ajustado para Branch and Bound
        // cout << "# Using Branch and Bound Algorithm" << endl;
        resultDominatingSet = findMDS_BranchAndBound(cityGraph, vertices, adj, degrees, sortedIndices);
    // } else {
    //     // cout << "# Using Greedy Algorithm" << endl;
    //     resultDominatingSet = findMinDominatingSet_Greedy(cityGraph);
    // }


    // Imprimir resultados
    if (resultDominatingSet.empty() && numVertices > 0) {
        // cout << "(Error: No se encontró conjunto dominante)" << endl;
    } else if (!resultDominatingSet.empty()) {
        for (size_t i = 0; i < resultDominatingSet.size(); ++i) {
            if (i > 0) cout << " ";
            cout << resultDominatingSet[i];
        }
        cout << endl;
        // cout << "Tamaño del conjunto dominante mínimo: " << resultDominatingSet.size() << endl;
    } else if (numVertices == 0) {
        cout << "Grafo vacío." << endl;
        // cout << "Tamaño del conjunto dominante mínimo: 0" << endl;
    }


    return 0;
}