#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <numeric>
#include <algorithm>
#include <utility>
#include <stdexcept>

#include "nodoArco.hpp"
#include "nodoVertice.hpp"
#include "grafo.hpp" 

using namespace std;

// --- Estructura DSU (Union-Find) ---
// Funciona con strings como identificadores de vértices
struct DSU {
    map<string, string> parent;

    void add_vertex(const string& s) {
        if (parent.find(s) == parent.end()) {
            parent[s] = s;
        }
    }

    string find(const string& i) {
        add_vertex(i);
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]); // Path compression
    }

    void unite(const string& i, const string& j) {
        string root_i = find(i);
        string root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

// --- Estructura para representar una CONEXIÓN no dirigida ---
// Se usa para el algoritmo de selección tipo Kruskal
struct Edge {
    string u; // Vértice normalizado (menor)
    string v; // Vértice normalizado (mayor)

    // Operador < para std::set (basado en la forma normalizada)
    bool operator<(const Edge& other) const {
        // La normalización ya se hizo al crear el Edge,
        // pero por seguridad o si se creara de otra forma:
        string u1 = min(u, v);
        string v1 = max(u, v);
        string u2 = min(other.u, other.v);
        string v2 = max(other.u, other.v);
        if (u1 != u2) return u1 < u2;
        return v1 < v2;
    }
     // Operador == (opcional)
     bool operator==(const Edge& other) const {
        return (min(u, v) == min(other.u, other.v)) && (max(u, v) == max(other.u, other.v));
    }
};


int main() {
    int k;
    if (!(cin >> k)) {
        cerr << "Error: No se pudo leer el valor de k." << endl;
        return 1;
    }
    cin.ignore();

    grafo<string> g; // Grafo dirigido para almacenar la entrada original
    string linea;
    set<Edge> aristas_potenciales; // Set de CONEXIONES únicas no dirigidas

    // --- Paso 1: Leer Datos, Construir Grafo Dirigido y Recopilar Conexiones Únicas ---
    while (getline(cin, linea)) {
         if (linea.empty()) continue;

        stringstream ss(linea);
        string u_str, v_str;
        string extra;

        if (ss >> u_str >> v_str && !(ss >> extra)) {
            // 1. Insertar vértices en el grafo dirigido
            g.insertarVertice(u_str);
            g.insertarVertice(v_str);
            // 2. Insertar el arco DIRIGIDO original en el grafo
            g.insertarArco(u_str, v_str); // Ignora si falla (ej. arco duplicado en input)

            // 3. Crear y añadir la CONEXIÓN no dirigida normalizada al set
            if (u_str != v_str) {
                Edge temp_edge;
                temp_edge.u = min(u_str, v_str); // Normalizar
                temp_edge.v = max(u_str, v_str); // Normalizar
                aristas_potenciales.insert(temp_edge); // Set maneja duplicados
             }
        } else {
            if (!linea.empty() && linea.find_first_not_of(" \t\n\r\f\v") != string::npos) {
                cerr << "Advertencia: Ignorando línea mal formada: \"" << linea << "\"" << endl;
            }
        }
    }

    // --- Paso 2: Inicialización del Algoritmo (Kruskal sobre Conexiones) ---
    DSU dsu;
    map<string, int> grado; // Grado TOTAL (apariciones en aristas seleccionadas)
    vector<pair<string, string> > arbol_resultante; // Almacena las CONEXIONES seleccionadas
    vector<string> todos_vertices = g.getVertices(); // Obtener vértices del grafo dirigido

    for (vector<string>::const_iterator it = todos_vertices.begin(); it != todos_vertices.end(); ++it) {
        const string& v_nombre = *it;
        dsu.add_vertex(v_nombre);
        grado[v_nombre] = 0;
    }

    int n_vertices = g.getNroVertices();
    if (n_vertices == 0) {
        cout << "--- Arbol Generador Resultante (k=" << k << ") ---" << endl;
        return 0;
    }

    // --- Paso 3: Procesar Conexiones Potenciales (Ordenadas por el set) ---
    for (set<Edge>::const_iterator it = aristas_potenciales.begin(); it != aristas_potenciales.end(); ++it) {
        const Edge& arista = *it; // La conexión normalizada {u, v}
        string u = arista.u; // Vértice normalizado (menor)
        string v = arista.v; // Vértice normalizado (mayor)

        // Verificar si añadir esta CONEXIÓN crea un ciclo (no dirigido)
        if (dsu.find(u) != dsu.find(v)) {
            // Verificar si añadir esta CONEXIÓN viola el grado k
            if (grado[u] < k && grado[v] < k) {
                // Añadir la CONEXIÓN
                dsu.unite(u, v);
                grado[u]++; // Incrementar grado para ambos vértices
                grado[v]++;
                // Guardar la conexión normalizada {u, v} en el resultado
                arbol_resultante.push_back(std::make_pair(u, v));
            }
        }
    }

    // --- Paso 4: Verificación (Opcional) ---
    // ...

    // --- Paso 5: Salida del Resultado ---
    // Imprime las CONEXIONES seleccionadas en formato normalizado
    cout << "--- Arbol Generador Resultante (k=" << k << ") ---" << endl;
    for (vector<pair<string, string> >::const_iterator it = arbol_resultante.begin(); it != arbol_resultante.end(); ++it) {
        cout << it->first << " " << it->second << endl; // Imprime u (menor) v (mayor)
    }

    return 0;
}
