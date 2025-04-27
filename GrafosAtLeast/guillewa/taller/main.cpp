#include "GraphND.h" // Your graph library
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <set>          // To easily check for dominated nodes
#include <unordered_set> // Potentially faster set operations
#include <vector>
#include <map>          // To map vertex names to indices
#include <unordered_map> // Potentially faster map
#include <algorithm>    // For sort, min
#include <sstream>      // For parsing input
#include <limits>       // For initial best size

// Assuming GraphND and EdgeTriple are in the global or eda namespace
// If they are in a namespace (e.g., eda), uncomment the next line
// using namespace eda;

using namespace std;

// --- Helper Function: Check if a set D is a dominating set for graph G ---
// Uses maps for potentially faster lookups if graph/set is large
bool isDominatingSet( const GraphND<string>& graph,const unordered_set<string>& current_D, const list<string>& all_vertices, const unordered_map<string, list<string>>& adj_list ) {
    if (all_vertices.empty()) return true; // Empty graph is dominated by empty set
    if (current_D.empty() && !all_vertices.empty()) return false; // Empty set cannot dominate non-empty graph

    unordered_set<string> dominated_nodes;

    // Mark nodes in D and their neighbors as dominated
    for (const string& node_in_d : current_D) {
        // Mark the node itself
        dominated_nodes.insert(node_in_d);
        // Mark its neighbors
        // Use the precomputed adjacency list
        auto it = adj_list.find(node_in_d);
        if (it != adj_list.end()) {
            for (const string& neighbor : it->second) {
                dominated_nodes.insert(neighbor);
            }
        }
        // Original way (might call graph.getNeighbors repeatedly):
        // try {
        //     list<string> neighbors = graph.getNeighbors(node_in_d);
        //     for (const string& neighbor : neighbors) {
        //         dominated_nodes.insert(neighbor);
        //     }
        // } catch (const exception& e) { /* Node might not exist if graph changed? Or isolated */ }
    }

    // Check if all vertices are dominated
    return dominated_nodes.size() == all_vertices.size();
}


// --- Recursive Backtracking Function ---
void findMDS_Backtrack_Recursive(
    const GraphND<string>& graph,
    const vector<string>& vertices_vec, // Vector of vertex names for indexed access
    int current_index,                  // Index of the vertex we are considering
    unordered_set<string>& current_D,   // The dominating set being built (passed by ref)
    list<string>& best_D_so_far,        // Best complete solution found (passed by ref)
    const unordered_map<string, list<string>>& adj // Precomputed adjacency list
) {
    // --- Pruning ---
    // If the current set is already >= the best found, no need to continue
    if (current_D.size() >= best_D_so_far.size()) {
        return;
    }

    // --- Base Case ---
    // If we have considered all vertices
    if (current_index == vertices_vec.size()) {
        // Check if the current_D is a valid dominating set
        list<string> all_vertices_list(vertices_vec.begin(), vertices_vec.end()); // Need list for isDominatingSet helper
        if (isDominatingSet(graph, current_D, all_vertices_list, adj)) {
            // If it is valid AND smaller than the best found so far
            if (current_D.size() < best_D_so_far.size()) {
                // Update the best solution
                best_D_so_far.assign(current_D.begin(), current_D.end());
                // cout << "  Found new best D (size " << best_D_so_far.size() << ")" << endl; // Debug
            }
        }
        return; // End of this path
    }

    // --- Recursive Steps ---
    string current_vertex = vertices_vec[current_index];

    // 1. Explore WITHOUT including current_vertex in D
    findMDS_Backtrack_Recursive(graph, vertices_vec, current_index + 1, current_D, best_D_so_far, adj);

    // 2. Explore INCLUDING current_vertex in D
    current_D.insert(current_vertex); // Add to current set
    findMDS_Backtrack_Recursive(graph, vertices_vec, current_index + 1, current_D, best_D_so_far, adj);
    current_D.erase(current_vertex); // Backtrack: remove from current set before returning
}

// --- Wrapper Function to Setup and Call Backtracking ---
list<string> findMinDominatingSet_Backtrack(GraphND<string>& graph) {
    list<string> all_vertices_list = graph.getVertices();
    if (all_vertices_list.empty()) {
        return {}; // Return empty list for empty graph
    }

    // Convert list to vector for indexed access in recursion
    vector<string> vertices_vec(all_vertices_list.begin(), all_vertices_list.end());

    // Precompute adjacency list for efficiency in isDominatingSet
    unordered_map<string, list<string>> adj_list;
     for(const string& v : vertices_vec) {
        try {
            adj_list[v] = graph.getNeighbors(v);
        } catch(...) { /* handle error or isolated node */
            adj_list[v] = {}; // Ensure entry exists even if isolated
        }
     }


    unordered_set<string> current_D; // Start with an empty set
    // Initialize best_D_so_far with a valid dominating set (e.g., all vertices)
    // This provides the initial upper bound for pruning.
    list<string> best_D_so_far = all_vertices_list;
    // cout << "Initial best D size: " << best_D_so_far.size() << endl; // Debug

    // Start the recursion from the first vertex (index 0)
    findMDS_Backtrack_Recursive(graph, vertices_vec, 0, current_D, best_D_so_far, adj_list);

    // Sort the final result alphabetically as required by PDF
    best_D_so_far.sort();
    return best_D_so_far;
}

list<string> findMinDominatingSet_Greedy(GraphND<string>& graph) {
    list<string> dominatingSet;
    list<string> allNodesList = graph.getVertices();

    if (allNodesList.empty()) {
        return dominatingSet; // Return empty list for empty graph
    }

    // Use unordered_set for efficient checking and removal of uncovered nodes
    unordered_set<string> uncoveredNodes(allNodesList.begin(), allNodesList.end());
    // Keep track of nodes already added to the dominating set to avoid re-adding
    unordered_set<string> nodesInD;

    // Precompute neighbors for efficiency inside the loop
    unordered_map<string, list<string>> adj_list;
    for(const string& v : allNodesList) {
        try {
            adj_list[v] = graph.getNeighbors(v);
        } catch(...) {
            adj_list[v] = {}; // Ensure entry exists even if isolated
        }
    }

    while (!uncoveredNodes.empty()) {
        string bestNodeToAdd = "";
        int maxCoverage = -1; // Max number of *currently uncovered* nodes covered

        // Iterate through all potential nodes to add (those not already in D)
        for (const string& candidateNode : allNodesList) {
            // Skip if this node is already in our dominating set
            if (nodesInD.count(candidateNode)) {
                continue;
            }

            int currentCoverage = 0;
            // Check if the candidate node itself is uncovered
            if (uncoveredNodes.count(candidateNode)) {
                currentCoverage++;
            }
            // Check how many of its neighbors are uncovered
            auto it = adj_list.find(candidateNode);
            if (it != adj_list.end()) {
                for (const string& neighbor : it->second) {
                    if (uncoveredNodes.count(neighbor)) {
                        currentCoverage++;
                    }
                }
            }

            // If this candidate covers more *new* nodes, it's the current best
            if (currentCoverage > maxCoverage) {
                maxCoverage = currentCoverage;
                bestNodeToAdd = candidateNode;
            }
            // Optional: Add tie-breaking rule here if needed (e.g., alphabetical)
            // else if (currentCoverage == maxCoverage && candidateNode < bestNodeToAdd) {
            //     bestNodeToAdd = candidateNode;
            // }
        }

        // If we didn't find any node that covers new nodes (e.g., remaining nodes are isolated)
        // or if the graph is disconnected and we finished a component.
        // We must pick *some* uncovered node to continue.
        if (bestNodeToAdd.empty() && !uncoveredNodes.empty()) {
            // Pick the first available uncovered node
             bestNodeToAdd = *uncoveredNodes.begin();
             // Ensure it's not accidentally already in D (shouldn't happen with the check above, but safe)
             if (nodesInD.count(bestNodeToAdd)) {
                 cerr << "Error: Greedy fallback selected a node already in D?" << endl;
                 // Find a different one - this indicates a potential logic issue if it occurs
                 for(const string& node : uncoveredNodes) {
                     if (!nodesInD.count(node)) {
                         bestNodeToAdd = node;
                         break;
                     }
                 }
             }
            // cout << "Greedy fallback: adding " << bestNodeToAdd << endl; // Debug
        } else if (bestNodeToAdd.empty() && uncoveredNodes.empty()) {
            break; // Should be covered by the while condition, but safe exit
        }


        // Add the chosen best node to the dominating set
        dominatingSet.push_back(bestNodeToAdd);
        nodesInD.insert(bestNodeToAdd); // Mark as added to D

        // Update the set of uncovered nodes
        // Remove the node itself
        uncoveredNodes.erase(bestNodeToAdd);
        // Remove its neighbors
        auto it = adj_list.find(bestNodeToAdd);
        if (it != adj_list.end()) {
            for (const string& neighbor : it->second) {
                uncoveredNodes.erase(neighbor); // erase is safe even if not present
            }
        }
    } // End while(!uncoveredNodes.empty())

    // Sort the final result alphabetically
    dominatingSet.sort();
    return dominatingSet;
}


// --- Función Principal ---
int main() {
    cout << "--- Taller 3: Dominion of Steel (Minimum Dominating Set - Backtracking) ---" << endl;
    
    GraphND<string> cityGraph;
    string line;
    string u, v;

    cout << "Introduce las conexiones (carreteras) entre pueblos (una por linea, ej: valhalla midgard), termina con linea vacia o EOF:" << endl;

    // Leer la entrada y construir el grafo
    set<string> vertex_set; // Use a set to automatically handle unique vertices
    vector<pair<string, string>> edges; // Store edges to add after all vertices are known (optional, safer)

    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        if (ss >> u >> v) {
            vertex_set.insert(u);
            vertex_set.insert(v);
            edges.push_back({u, v});
        } else {
            cerr << "Advertencia: Linea ignorada, formato incorrecto: '" << line << "'" << endl;
        }
    }

    // Add vertices to graph
    for(const string& vertex_name : vertex_set) {
        cityGraph.addVertex(vertex_name);
    }

    // Add edges to graph
    try {
        for(const auto& edge_pair : edges) {
            // Weight doesn't matter for MDS, use 1.0 or 0.0
            cityGraph.addEdge(edge_pair.first, edge_pair.second, 0.0f);
        }
    } catch (const exception& e) {
        cerr << "Error añadiendo aristas: " << e.what() << endl;
        return 1;
    }


    cout << "\nGrafo construido con " << cityGraph.order() << " pueblos y " << cityGraph.getNumEdges() << " carreteras." << endl;

    if (cityGraph.order() == 0) {
        cout << "\nNo se introdujeron pueblos. No hay conjunto dominante." << endl;
        return 0;
    }

    list<string> resultDominatingSet;
    int numVertices = cityGraph.order();
    if (numVertices <= 29) {
        // === Ejecutar el algoritmo Backtracking para MDS ===
        cout << "\nCalculando conjunto dominante minimo (Backtracking)..." << endl;
        resultDominatingSet = findMinDominatingSet_Backtrack(cityGraph);
    } else {
        cout << "\nCalculando conjunto dominante minimo (\"Backtracking\")..." << endl;
        resultDominatingSet = findMinDominatingSet_Greedy(cityGraph);
    }
    
    // === Mostrar Resultados ===
    cout << "\n--- Conjunto Dominante Minimo Encontrado (D) ---" << endl;
    if (resultDominatingSet.empty() && cityGraph.order() > 0) {
         cout << "(Error: No se encontro conjunto dominante, revisar logica o grafo)" << endl;
    } else if (resultDominatingSet.empty() && cityGraph.order() == 0) {
         cout << "(Grafo vacio)" << endl;
    }
    else {
        bool first = true;
        for (const string& node : resultDominatingSet) {
            if (!first) {
                cout << " ";
            }
            cout << node;
            first = false;
        }
        cout << endl;
        cout << "Tamaño del conjunto dominante minimo: " << resultDominatingSet.size() << endl;
    }

    cout << "\n--- Ejecucion Completa ---" << endl;
    return 0;
}
