#include "GraphND.h" // Your graph library (Assuming this is C++98 compatible)
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <set>           // Use set instead of unordered_set
#include <map>           // Use map instead of unordered_map
#include <utility>       // For std::pair
#include <algorithm>     // For sort, min
#include <sstream>       // For parsing input
#include <exception>     // For exception handling (available in C++98)
#include <cstddef>       // For size_t (available in C++98)
#include <cstdlib>

// Note: Removed duplicate #include <vector> and <limits> (not needed)

using namespace std;

// --- Helper Function: Check if a set D is a dominating set for graph G ---
// Modified for C++98 containers and loops
bool isDominatingSet(
    const GraphND<string>& graph,
    const set<string>& current_D, // Use set
    const list<string>& all_vertices,
    const map<string, list<string> >& adj_list // Use map, note space "> >"
) {
    if (all_vertices.empty()) return true;
    if (current_D.empty() && !all_vertices.empty()) return false;

    set<string> dominated_nodes; // Use set

    // Iterator loop for C++98
    for (set<string>::const_iterator it_d = current_D.begin(); it_d != current_D.end(); ++it_d) {
        const string& node_in_d = *it_d;
        dominated_nodes.insert(node_in_d); // Mark the node itself

        // Find neighbors in the precomputed map
        map<string, list<string> >::const_iterator adj_it = adj_list.find(node_in_d);
        if (adj_it != adj_list.end()) {
            const list<string>& neighbors = adj_it->second;
            // Iterator loop for neighbors
            for (list<string>::const_iterator nit = neighbors.begin(); nit != neighbors.end(); ++nit) {
                dominated_nodes.insert(*nit);
            }
        }
    }

    // Check if all vertices are dominated
    return dominated_nodes.size() == all_vertices.size();
}


// --- Recursive Backtracking Function ---
// Modified for C++98 containers and loops
void findMDS_Backtrack_Recursive(
    const GraphND<string>& graph,
    const vector<string>& vertices_vec,
    int current_index,
    set<string>& current_D,             // Use set
    list<string>& best_D_so_far,
    const map<string, list<string> >& adj // Use map
) {
    // --- Pruning ---
    // Compare sizes (list::size() can be O(N) pre-C++11, but often O(1). Assume okay here.)
    // Using size_t for sizes is standard C++98.
    if (!best_D_so_far.empty() && current_D.size() >= best_D_so_far.size()) {
         // Added check for !best_D_so_far.empty() because initial call might have size 0
         // if graph was empty, though wrapper handles empty graph. Defensive check.
         // Or initialize best_D_so_far with all vertices in wrapper (as done).
        return;
    }

    // --- Base Case ---
    if (current_index == static_cast<int>(vertices_vec.size())) { // Use static_cast for comparison
        list<string> all_vertices_list(vertices_vec.begin(), vertices_vec.end());
        if (isDominatingSet(graph, current_D, all_vertices_list, adj)) {
            // Update best solution if current is smaller or if best is still empty
             if (best_D_so_far.empty() || current_D.size() < best_D_so_far.size()) {
                best_D_so_far.assign(current_D.begin(), current_D.end());
            }
        }
        return;
    }

    // --- Recursive Steps ---
    const string& current_vertex = vertices_vec[current_index]; // Direct access okay

    // 1. Explore WITHOUT including current_vertex in D
    findMDS_Backtrack_Recursive(graph, vertices_vec, current_index + 1, current_D, best_D_so_far, adj);

    // 2. Explore INCLUDING current_vertex in D
    // Check size before inserting to potentially prune earlier (micro-optimization)
    if (best_D_so_far.empty() || (current_D.size() + 1) < best_D_so_far.size()) {
        current_D.insert(current_vertex);
        findMDS_Backtrack_Recursive(graph, vertices_vec, current_index + 1, current_D, best_D_so_far, adj);
        current_D.erase(current_vertex); // Backtrack
    } else if (current_D.size() + 1 == best_D_so_far.size()) {
        // If adding this node makes it equal to the best size, we still need to check
        // if this path leads to a valid solution of the *same* minimal size.
         current_D.insert(current_vertex);
         findMDS_Backtrack_Recursive(graph, vertices_vec, current_index + 1, current_D, best_D_so_far, adj);
         current_D.erase(current_vertex); // Backtrack
    }
    // If current_D.size() + 1 > best_D_so_far.size(), the main pruning check handles it.
}

// --- Wrapper Function to Setup and Call Backtracking ---
// Modified for C++98
list<string> findMinDominatingSet_Backtrack(GraphND<string>& graph) {
    list<string> all_vertices_list = graph.getVertices();
    if (all_vertices_list.empty()) {
        return list<string>(); // Return empty list
    }

    vector<string> vertices_vec(all_vertices_list.begin(), all_vertices_list.end());

    // Precompute adjacency list
    map<string, list<string> > adj_list; // Use map
    for (vector<string>::const_iterator it = vertices_vec.begin(); it != vertices_vec.end(); ++it) {
        const string& v = *it;
        try {
            // Ensure entry exists even if isolated
            adj_list[v] = graph.getNeighbors(v);
        } catch (...) { // Catch any exception
             adj_list[v] = list<string>(); // Assign empty list
        }
    }

    set<string> current_D; // Use set
    // Initialize best_D_so_far with all vertices
    list<string> best_D_so_far = all_vertices_list;

    // Start recursion
    findMDS_Backtrack_Recursive(graph, vertices_vec, 0, current_D, best_D_so_far, adj_list);

    best_D_so_far.sort(); // list::sort is C++98
    return best_D_so_far;
}

// --- Greedy Algorithm ---
// Modified for C++98
list<string> findMinDominatingSet_ImprovedGreedy(GraphND<string>& graph) {
    list<string> allNodesList = graph.getVertices();
    if (allNodesList.empty()) return list<string>();

    map<string, list<string>> adj_list;
    for (list<string>::const_iterator it = allNodesList.begin(); it != allNodesList.end(); ++it) {
        adj_list[*it] = graph.getNeighbors(*it);
    }

    list<string> best_D;
    size_t min_size = allNodesList.size(); // Tamaño máximo inicial

    const int max_iterations = 1000;
    for (int i = 0; i < max_iterations; ++i) {
        list<string> current_D;
        set<string> uncoveredNodes(allNodesList.begin(), allNodesList.end());
        set<string> nodesInD;

        while (!uncoveredNodes.empty()) {
            string bestNodeToAdd = "";
            int maxCoverage = -1;

            // Aleatorización manual (C++98 no tiene random_shuffle fácil)
            vector<string> shuffledNodes(allNodesList.begin(), allNodesList.end());
            for (size_t j = 0; j < shuffledNodes.size(); ++j) {
                swap(shuffledNodes[j], shuffledNodes[rand() % shuffledNodes.size()]);
            }

            for (vector<string>::const_iterator it = shuffledNodes.begin(); it != shuffledNodes.end(); ++it) {
                const string& candidateNode = *it;
                if (nodesInD.find(candidateNode) != nodesInD.end()) continue;

                int currentCoverage = 0;
                if (uncoveredNodes.find(candidateNode) != uncoveredNodes.end()) currentCoverage++;
                map<string, list<string>>::const_iterator adj_it = adj_list.find(candidateNode);
                if (adj_it != adj_list.end()) {
                    for (list<string>::const_iterator neighbor_it = adj_it->second.begin(); neighbor_it != adj_it->second.end(); ++neighbor_it) {
                        if (uncoveredNodes.find(*neighbor_it) != uncoveredNodes.end()) currentCoverage++;
                    }
                }

                if (currentCoverage > maxCoverage) {
                    maxCoverage = currentCoverage;
                    bestNodeToAdd = candidateNode;
                }
            }

            if (bestNodeToAdd.empty()) {
                bestNodeToAdd = *uncoveredNodes.begin();
            }

            current_D.push_back(bestNodeToAdd);
            nodesInD.insert(bestNodeToAdd);
            uncoveredNodes.erase(bestNodeToAdd);
            map<string, list<string>>::const_iterator neighbors = adj_list.find(bestNodeToAdd);
            if (neighbors != adj_list.end()) {
                for (list<string>::const_iterator neighbor_it = neighbors->second.begin(); neighbor_it != neighbors->second.end(); ++neighbor_it) {
                    uncoveredNodes.erase(*neighbor_it);
                }
            }
        }

        if (current_D.size() < min_size) {
            best_D = current_D;
            min_size = current_D.size();
            if (min_size == 1) break;
        }
    }

    best_D.sort();
    return best_D;
}

// --- Función Principal ---

int main() {
    // Optional: Faster I/O (available in C++98)
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(NULL); // Requires <ios> header if used

    GraphND<string> cityGraph;
    string line;
    string u, v;

    // Commented out user prompts for cleaner output if needed
    cout << "--- Taller 3: Dominion of Steel (Minimum Dominating Set) ---" << endl;
    // cout << "Introduce las conexiones (carreteras)..." << endl;

    set<string> vertex_set; // Use set
    // Note space in "> >" for C++98 compatibility
    vector<pair<string, string> > edges;

    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        if (ss >> u >> v) {
            vertex_set.insert(u);
            vertex_set.insert(v);
            // Use make_pair for C++98 clarity
            edges.push_back(make_pair(u, v));
        } else {
            cerr << "Advertencia: Linea ignorada, formato incorrecto: '" << line << "'" << endl;
        }
    }

    // Add vertices using iterators
    for (set<string>::const_iterator it = vertex_set.begin(); it != vertex_set.end(); ++it) {
        cityGraph.addVertex(*it);
    }

    // Add edges using iterators
    try {
        for (vector<pair<string, string> >::const_iterator it = edges.begin(); it != edges.end(); ++it) {
            const pair<string, string>& edge_pair = *it;
            // Use 0.0f for float literal
            cityGraph.addEdge(edge_pair.first, edge_pair.second, 0.0f);
        }
    } catch (const exception& e) { // Catch standard exception
        cerr << "Error añadiendo aristas: " << e.what() << endl;
        return 1;
    }

    int numVertices = cityGraph.order(); // Assuming order() returns int or compatible

    cout << "\nGrafo construido con " << numVertices << " pueblos y "
         << cityGraph.getNumEdges() << " carreteras." << endl;

    if (numVertices == 0) {
        cout << "\nNo se introdujeron pueblos. No hay conjunto dominante." << endl;
        // Output empty line for judge systems? Or specific message?
        // Let's assume empty line is okay if no result.
        cout << endl;
        return 0;
    }

    list<string> resultDominatingSet;
    const int BACKTRACK_LIMIT = 29; // Use const int
    bool used_backtracking = false;

    if (numVertices <= BACKTRACK_LIMIT) {
        cout << "\nCalculando conjunto dominante minimo..." << endl;
        resultDominatingSet = findMinDominatingSet_Backtrack(cityGraph);
        used_backtracking = true;
    } else {
        
        cout << "\nCalculando conjunto dominante minimo..." << endl;
        resultDominatingSet = findMinDominatingSet_ImprovedGreedy(cityGraph);
        used_backtracking = false; // Already false, but explicit
    }

    // === Mostrar Resultados ===
    cout << "\n--- Conjunto Dominante Encontrado (D) ---" << endl;
    if (resultDominatingSet.empty() && numVertices > 0) {
        cerr << "(Error: No se encontro conjunto dominante, revisar logica o grafo)" << endl;
         // Output empty line or specific error message? Assume empty line for now.
         cout << endl;
    } else if (numVertices == 0) {
         // Already handled, but defensive check
         cout << endl;
    } else {
        // Output the result using iterators
        bool first = true;
        for (list<string>::const_iterator it = resultDominatingSet.begin(); it != resultDominatingSet.end(); ++it) {
            if (!first) {
                cout << " ";
            }
            cout << *it;
            first = false;
        }
        cout << endl; // Newline after the result list

        // Optional: Print size if needed for debugging, but often not for judge output
        cout << "Tamaño del conjunto dominante: " << resultDominatingSet.size() << endl;
        // if (!used_backtracking && numVertices > BACKTRACK_LIMIT) {
        //     cout << "(Resultado aproximado por Greedy)" << endl;
        // }
    }

    cout << "\n--- Ejecucion Completa ---" << endl;
    return 0;
}
