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

// --- Estructuras para Branch and Bound ---
struct BnBNode {
    int bitmask;        // Bitmask de nodos dominados
    vector<int> D;      // Índices de nodos en el conjunto dominante
    int nextIdx;        // Siguiente nodo a considerar
    int lowerBound;     // Cota inferior estimada
};

struct CompareBnBNode {
    bool operator()(const BnBNode& a, const BnBNode& b) {
        return a.lowerBound > b.lowerBound; // Min-heap
    }
};

// --- Variables globales para optimización ---
vector<string> vertices;                // Nombres de nodos
vector< vector<int> > adj;              // Lista de adyacencia (índices)
vector<int> degrees;                    // Grados de los nodos
vector<int> sortedIndices;              // Índices ordenados por grado

// --- Precomputar datos del grafo ---
void precomputeGraphData(GraphND<string>& graph) {
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
    sort(sortedIndices.begin(), sortedIndices.end(), [](int a, int b) {
        return degrees[a] > degrees[b];
    });
}

// --- Verificar si un bitmask domina todos los nodos ---
bool isDominatingSet(int bitmask) {
    int n = vertices.size();
    int allCovered = (1 << n) - 1;
    return (bitmask & allCovered) == allCovered;
}

// --- Calcular cota inferior para poda ---
int computeLowerBound(const vector<int>& D, int bitmask) {
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

// --- Algoritmo Branch and Bound optimizado ---
vector<string> findMDS_BranchAndBound(GraphND<string>& graph) {
    precomputeGraphData(graph);
    int n = vertices.size();

    // Inicializar mejor solución con greedy
    vector<string> bestD;
    int bestSize = INT_MAX;

    // Cola de prioridad para BnB
    vector<BnBNode> heap;
    BnBNode root;
    root.bitmask = 0;
    root.nextIdx = 0;
    root.lowerBound = computeLowerBound(root.D, root.bitmask);
    heap.push_back(root);

    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), CompareBnBNode());
        BnBNode current = heap.back();
        heap.pop_back();

        // Poda: si la cota es peor que la mejor solución
        if (current.lowerBound >= bestSize) continue;

        // Caso base: todos los nodos considerados
        if (current.nextIdx == n) {
            if (isDominatingSet(current.bitmask)) {
                if (current.D.size() < bestSize) {
                    bestSize = current.D.size();
                    bestD.clear();
                    for (size_t i = 0; i < current.D.size(); ++i) {
                        bestD.push_back(vertices[current.D[i]]);
                    }
                }
            }
            continue;
        }

        int node = sortedIndices[current.nextIdx];

        // Opción 1: Excluir el nodo
        BnBNode exclude = current;
        exclude.nextIdx++;
        exclude.lowerBound = computeLowerBound(exclude.D, exclude.bitmask);
        if (exclude.lowerBound < bestSize) {
            heap.push_back(exclude);
            push_heap(heap.begin(), heap.end(), CompareBnBNode());
        }

        // Opción 2: Incluir el nodo
        BnBNode include = current;
        include.D.push_back(node);
        include.bitmask |= (1 << node);
        for (size_t i = 0; i < adj[node].size(); ++i) {
            include.bitmask |= (1 << adj[node][i]);
        }
        include.nextIdx++;
        include.lowerBound = computeLowerBound(include.D, include.bitmask);
        if (include.lowerBound < bestSize) {
            heap.push_back(include);
            push_heap(heap.begin(), heap.end(), CompareBnBNode());
        }
    }

    sort(bestD.begin(), bestD.end());
    return bestD;
}

// --- Algoritmo Greedy (para comparación) ---
vector<string> findMinDominatingSet_Greedy(GraphND<string>& graph) {
    list<string> vertexList = graph.getVertices();
    vector<string> vertices(vertexList.begin(), vertexList.end());
    int n = vertices.size();
    vector<bool> covered(n, false);
    vector<string> D;

    while (true) {
        int maxCover = -1;
        int bestNode = -1;

        for (int i = 0; i < n; ++i) {
            // Se cambió la lógica para considerar nodos aún no en D,
            // y contar cuántos *nuevos* nodos cubriría si se añade.
            bool alreadyInD = false;
            for(size_t k = 0; k < D.size(); ++k) {
                if (vertices[i] == D[k]) {
                    alreadyInD = true;
                    break;
                }
            }
            if (alreadyInD) continue; // Si ya está en D, no lo volvemos a considerar

            int count = 0;
            // Contar el nodo i si no está cubierto
            if (!covered[i]) count++;

            // Contar vecinos no cubiertos
            list<string> neighbors = graph.getNeighbors(vertices[i]);
            for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
                int j = find(vertices.begin(), vertices.end(), *it) - vertices.begin();
                if (!covered[j]) count++;
            }

            if (count > maxCover) {
                maxCover = count;
                bestNode = i;
            }
        }

        // Si maxCover es 0, significa que todos los nodos restantes ya están cubiertos
        // o no hay nodos que puedan cubrir a otros nuevos.
        if (bestNode == -1 || maxCover == 0) {
             // Verificar si todos los nodos están cubiertos antes de salir
            bool allNodesCovered = true;
            for(int i = 0; i < n; ++i) {
                if (!covered[i]) {
                    allNodesCovered = false;
                    // Si hay un nodo no cubierto, forzar su inclusión
                    // (Esto podría no ser óptimo, pero asegura cobertura)
                    // Una mejor estrategia greedy podría requerirse aquí.
                    // Para este caso simple, añadimos el primero no cubierto.
                    bestNode = i;
                    break;
                }
            }
            if (allNodesCovered || bestNode == -1) break; // Salir si todo está cubierto o no se puede mejorar
        }


        D.push_back(vertices[bestNode]);
        covered[bestNode] = true;
        list<string> neighbors = graph.getNeighbors(vertices[bestNode]);
        for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
            int j = find(vertices.begin(), vertices.end(), *it) - vertices.begin();
            covered[j] = true;
        }
    }

    sort(D.begin(), D.end());
    return D;
}


// --- Función Principal ---
int main() {
    GraphND<string> cityGraph;
    string line, u, v;
    set<string> vertexSet;
    vector<pair<string, string>> edges;

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
        resultDominatingSet = findMDS_BranchAndBound(cityGraph);
    // } else {
    //     cout << "# Using Greedy Algorithm" << endl;
    //     resultDominatingSet = findMinDominatingSet_Greedy(cityGraph);
    // }


    // Imprimir resultados
    if (resultDominatingSet.empty() && numVertices > 0) {
        cout << "(Error: No se encontró conjunto dominante)" << endl;
    } else if (!resultDominatingSet.empty()) {
        for (size_t i = 0; i < resultDominatingSet.size(); ++i) {
            if (i > 0) cout << " ";
            cout << resultDominatingSet[i];
        }
        cout << endl;
        // cout << "Tamaño del conjunto dominante mínimo: " << resultDominatingSet.size() << endl;
    } else if (numVertices == 0) {
        cout << "Grafo vacío." << endl;
        cout << "Tamaño del conjunto dominante mínimo: 0" << endl;
    }


    return 0;
}