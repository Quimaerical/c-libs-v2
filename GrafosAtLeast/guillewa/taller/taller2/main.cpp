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
    int next_idx;       // Siguiente nodo a considerar
    int lower_bound;    // Cota inferior estimada
};

struct CompareBnBNode {
    bool operator()(const BnBNode& a, const BnBNode& b) {
        return a.lower_bound > b.lower_bound; // Min-heap
    }
};

// --- Variables globales para optimización ---
vector<string> vertices;                    // Nombres de nodos
vector< vector<int> > adj;                  // Lista de adyacencia (índices)
vector<int> degrees;                        // Grados de los nodos
vector<int> sorted_indices;                 // Índices ordenados por grado

// --- Precomputar datos del grafo ---
void precomputeGraphData(GraphND<string>& graph) {
    list<string> vertex_list = graph.getVertices();
    vertices.assign(vertex_list.begin(), vertex_list.end());
    int n = vertices.size();
    
    // Inicializar estructuras
    adj.resize(n);
    degrees.resize(n, 0);
    sorted_indices.resize(n);

    // Llenar lista de adyacencia y grados
    for (int i = 0; i < n; ++i) {
        list<string> neighbors = graph.getNeighbors(vertices[i]);
        for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
            int j = find(vertices.begin(), vertices.end(), *it) - vertices.begin();
            adj[i].push_back(j);
            degrees[i]++;
        }
        sorted_indices[i] = i;
    }

    // Ordenar nodos por grado (descendente)
    sort(sorted_indices.begin(), sorted_indices.end(), [](int a, int b) {
        return degrees[a] > degrees[b];
    });
}

// --- Verificar si un bitmask domina todos los nodos ---
bool isDominatingSet(int bitmask) {
    int n = vertices.size();
    int all_covered = (1 << n) - 1;
    return (bitmask & all_covered) == all_covered;
}

// --- Calcular cota inferior para poda ---
int computeLowerBound(const vector<int>& D, int bitmask) {
    int remaining = vertices.size() - __builtin_popcount(bitmask);
    if (remaining <= 0) return D.size();
    
    // Heurística: nodos no cubiertos / grado máximo + 1
    int max_degree = 1;
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (!(bitmask & (1 << i))) {
            if (degrees[i] + 1 > max_degree) {
                max_degree = degrees[i] + 1;
            }
        }
    }
    return D.size() + (remaining + max_degree - 1) / max_degree;
}

// --- Algoritmo Branch and Bound optimizado ---
vector<string> findMDS_BranchAndBound(GraphND<string>& graph) {
    precomputeGraphData(graph);
    int n = vertices.size();

    // Inicializar mejor solución con greedy
    vector<string> best_D;
    int best_size = INT_MAX;

    // Cola de prioridad para BnB
    vector<BnBNode> heap;
    BnBNode root;
    root.bitmask = 0;
    root.next_idx = 0;
    root.lower_bound = computeLowerBound(root.D, root.bitmask);
    heap.push_back(root);

    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), CompareBnBNode());
        BnBNode current = heap.back();
        heap.pop_back();

        // Poda: si la cota es peor que la mejor solución
        if (current.lower_bound >= best_size) continue;

        // Caso base: todos los nodos considerados
        if (current.next_idx == n) {
            if (isDominatingSet(current.bitmask)) {
                if (current.D.size() < best_size) {
                    best_size = current.D.size();
                    best_D.clear();
                    for (size_t i = 0; i < current.D.size(); ++i) {
                        best_D.push_back(vertices[current.D[i]]);
                    }
                }
            }
            continue;
        }

        int node = sorted_indices[current.next_idx];

        // Opción 1: Excluir el nodo
        BnBNode exclude = current;
        exclude.next_idx++;
        exclude.lower_bound = computeLowerBound(exclude.D, exclude.bitmask);
        if (exclude.lower_bound < best_size) {
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
        include.next_idx++;
        include.lower_bound = computeLowerBound(include.D, include.bitmask);
        if (include.lower_bound < best_size) {
            heap.push_back(include);
            push_heap(heap.begin(), heap.end(), CompareBnBNode());
        }
    }

    sort(best_D.begin(), best_D.end());
    return best_D;
}

// --- Algoritmo Greedy (para comparación) ---
// vector<string> findMinDominatingSet_Greedy(GraphND<string>& graph) {
//     list<string> vertex_list = graph.getVertices();
//     vector<string> vertices(vertex_list.begin(), vertex_list.end());
//     int n = vertices.size();
//     vector<bool> covered(n, false);
//     vector<string> D;
//     while (true) {
//         int max_cover = -1;
//         int best_node = -1;
//         for (int i = 0; i < n; ++i) {
//             if (!covered[i]) {
//                 int count = 1; // El nodo mismo
//                 list<string> neighbors = graph.getNeighbors(vertices[i]);
//                 for (list<string>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
//                     int j = find(vertices.begin(), vertices.end(), *it) - vertices.begin();
//                     if (!covered[j]) count++;
//                 }
//                 if (count > max_cover) {
//                     max_cover = count;
//                     best_node = i;
//                 }
//             }
//         }
//         if (best_node == -1) break;
//         D.push_back(vertices[best_node]);
//         covered[best_node] = true;
//         list<string> neighbors = graph.getNeighbors(vertices[best_node]);
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
    set<string> vertex_set;
    vector<pair<string, string>> edges;

    // Leer entrada
    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        if (ss >> u >> v) {
            vertex_set.insert(u);
            vertex_set.insert(v);
            edges.push_back(make_pair(u, v));
        }
    }

    // Construir grafo
    for (set<string>::iterator it = vertex_set.begin(); it != vertex_set.end(); ++it) {
        cityGraph.addVertex(*it);
    }
    for (size_t i = 0; i < edges.size(); ++i) {
        cityGraph.addEdge(edges[i].first, edges[i].second, 0.0f);
    }

    int numVertices = cityGraph.order();
    vector<string> resultDominatingSet;

    // if (numVertices <= 29) {
        resultDominatingSet = findMDS_BranchAndBound(cityGraph);
    // } else {
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
        cout << "Tamaño del conjunto dominante mínimo: " << resultDominatingSet.size() << endl;
    }

    return 0;
}