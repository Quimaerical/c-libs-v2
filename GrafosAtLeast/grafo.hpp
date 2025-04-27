#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <stdexcept>
#include <limits>
#include <map> // <- Añadir para esConexo (mapa de visitados)
#include "nodoVertice.hpp"


template <class elem>
class grafo {
	private:
		nodoVertice<elem>* primero;
		int nVertices;
		int nArcos;

		nodoVertice<elem>* buscarVertice(const elem& info) const {
			nodoVertice<elem>* actual = primero;
			while (actual != 0) {
				// Asume que elem tiene sobrecargado operator==
				if (actual->getInfo() == info) {
					return actual;
				}
				actual = actual->getProx();
			}
			return 0; // No encontrado  
		}

		nodoArco<elem>* buscarArco(nodoVertice<elem>* origen, nodoVertice<elem>* destino) const {
			if (!origen || !destino) return 0;	
			nodoArco<elem>* arcoActual = origen->getArcos();
			while (arcoActual != 0) {
				if (arcoActual->getExtremo() == destino) {
					return arcoActual;
				}
				arcoActual = arcoActual->getSiguiente();
			}
			return 0;
		}
	public:
		// Constructores y Destructor
		grafo ();
		~grafo ();

		// Setters
		void setPrimero(nodoVertice<elem>* prim);
		void setNroVertices(int numV);
		void setNroArcos(int numA);

		// Getters
		nodoVertice<elem>* getPrimero() const;
		int getNroVertices() const;
		int getNroArcos() const; 
		std::vector<elem> getVertices() const;

		// Modificadores del Grafo
		bool insertarVertice(const elem& info);
		bool insertarArco(const elem& vInfo, const elem& wInfo, float peso = 0.0f); // Peso opcional
		bool insertarArcoNoDirigido(const elem& vInfo, const elem& wInfo, float peso = 0.0f);
        bool eliminarVertice(const elem& info);
		bool eliminarArco(const elem& vInfo, const elem& wInfo);
        bool eliminarArcoNoDirigido(const elem& vInfo, const elem& wInfo);

		// Consultas sobre Arcos
		nodoArco<elem>* existeArco(const elem& vInfo, const elem& wInfo) const;
		float getPesoArco(const elem& vInfo, const elem& wInfo) const;

		// Consultas sobre Vecindad (grafos no dirigido)
		std::vector<elem> getVecinos(const elem& vInfo) const;

		// Consultas sobre Vecindad (para grafos dirigidos)
		std::vector<elem> getSucesores(const elem& vInfo) const;
		std::vector<elem> getPredecesores(const elem& vInfo) const;

		// --- Propiedades del Grafo ---
		bool esNulo() const;
		bool esCompleto() const;
		bool esConexo() const;
		
		// --- Propiedades de Vértices ---
		int gradoSalidaVertice(const elem& info) const;
		int gradoEntradaVertice(const elem& info) const;
		int gradoVertice(const elem& info) const;

		// Otras Operaciones
		void vaciar();
		void copiar(grafo<elem>* target); 
};

// Implementaciones

template <class elem>
grafo<elem>::grafo() {
	primero = 0;
	nVertices = 0;
	nArcos = 0;
}

template <class elem>
grafo<elem>::~grafo() {
    vaciar();
}

template <class elem>
void grafo<elem>::vaciar() {
    nodoVertice<elem>* verticeActual = primero;
    while (verticeActual != 0) {  
        nodoArco<elem>* arcoActual = verticeActual->getArcos();
        while (arcoActual != 0) {  
            nodoArco<elem>* arcoAEliminar = arcoActual;
            arcoActual = arcoActual->getSiguiente();
            delete arcoAEliminar; // Liberar memoria del arco
        }
        // Importante: obtener el siguiente *antes* de eliminar el actual
        nodoVertice<elem>* verticeAEliminar = verticeActual;
        verticeActual = verticeActual->getProx();
        delete verticeAEliminar; // Liberar memoria del vértice
    }
    primero = 0;  
    nVertices = 0;
    nArcos = 0;
}

template <class elem>
nodoVertice<elem>* grafo<elem>::getPrimero() const {
    return primero;
}

template <class elem>
int grafo<elem>::getNroVertices() const {
    return nVertices;
}

template <class elem>
int grafo<elem>::getNroArcos() const {
    return nArcos;
}

template <class elem>
std::vector<elem> grafo<elem>::getVertices() const {
    std::vector<elem> verticesInfo;
    nodoVertice<elem>* auxVertice = primero;
    while (auxVertice != 0) {  
        verticesInfo.push_back(auxVertice->getInfo());
        auxVertice = auxVertice->getProx();
    }
    return verticesInfo;
}

template <class elem>
bool grafo<elem>::insertarVertice(const elem& info) {
    // Verificar si ya existe
    if (buscarVertice(info) != 0) {  
        return false; // Ya existe
    }

    // Crear nuevo nodo vértice
    nodoVertice<elem>* nuevoVertice = new nodoVertice<elem>(info);
    if (esNulo()) { // Si el grafo está vacío
        primero = nuevoVertice;
    } else {
        // Ir al final de la lista de vértices (O(V))
        nodoVertice<elem>* ptrVertice = primero;
        while (ptrVertice->getProx() != 0) {  
            ptrVertice = ptrVertice->getProx();
        }
        ptrVertice->setProx(nuevoVertice);
    }
    nVertices++;
    return true;
}

template <class elem>
bool grafo<elem>::insertarArco(const elem& vInfo, const elem& wInfo, float peso) {
    // 1. Buscar los vértices origen y destino
    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo);

    // 2. Verificar que ambos vértices existen
    if (v == 0 || w == 0) {  
        return false; // Uno o ambos vértices no existen
    }

    // 3. Verificar si el arco ya existe (para no duplicarlo)
    if (buscarArco(v, w) != 0) { 
        return false; // El arco ya existe
    }

    // 4. Crear el nuevo nodo arco
    nodoArco<elem>* nuevoArco = new nodoArco<elem>(w, peso); // El arco apunta a 'w'

    // 5. Insertar el arco en la lista de adyacencia de 'v' (al principio)
    nuevoArco->setSiguiente(v->getArcos());
    // v->setArcos(nuevoArco); // Necesitamos modificar adyacentes, usamos getArcosRef()
    v->getArcosRef() = nuevoArco; // Actualizar la cabecera de la lista de arcos de v

    nArcos++;
    return true;
}

template <class elem>
bool grafo<elem>::insertarArcoNoDirigido(const elem& vInfo, const elem& wInfo, float peso) {
    // 1. Buscar los vértices origen y destino
    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo);

    // 2. Verificar que ambos vértices existen
    if (v == 0 || w == 0) {  
        return false; // Uno o ambos vértices no existen
    }

    // 3. Verificar si el arco ya existe (para no duplicarlo)
    if (buscarArco(v, w) != 0) { 
        return false; // El arco ya existe
    }
    if (buscarArco(w, v) != 0) { 
        return false; // El arco ya existe
    }

    // 4. Crear los dos nuevos nodos arcos
    nodoArco<elem>* nuevoArcoV = new nodoArco<elem>(w, peso); // El arco apunta a 'w
    nodoArco<elem>* nuevoArcoW = new nodoArco<elem>(v, peso); // El arco apunta a 'v

    // 5. Insertar los arcos en las listas de adyacencia de 'v' y '
    nuevoArcoV->setSiguiente(v->getArcos());
    // v->setArcos(nuevoArcoV);
    v->getArcosRef() = nuevoArcoV;

    nuevoArcoW->setSiguiente(w->getArcos());
    // w->setArcos(nuevoArcoW);
    w->getArcosRef() = nuevoArcoW;

    nArcos += 2;
    return true;
}

template <class elem>
bool grafo<elem>::eliminarVertice(const elem& info) {
    nodoVertice<elem>* verticeAEliminar = 0;  
    nodoVertice<elem>* verticeAnterior = 0;  

    // 1. Buscar el vértice y su anterior en la lista principal
    nodoVertice<elem>* actual = primero;
    while (actual != 0 && actual->getInfo() != info) {  
        verticeAnterior = actual;
        actual = actual->getProx();
    }

    if (actual == 0) {  
        return false; // Vértice no encontrado
    }
    verticeAEliminar = actual;

    // 2. Eliminar arcos salientes del vértice a eliminar
    nodoArco<elem>* arcoActual = verticeAEliminar->getArcos();
    while (arcoActual != 0) {  
        nodoArco<elem>* arcoAEliminar = arcoActual;
        arcoActual = arcoActual->getSiguiente();
        delete arcoAEliminar;
        nArcos--; // Decrementar contador de arcos
    }
    // verticeAEliminar->setArcos(0); // Necesitamos modificar, usamos getArcosRef()
    verticeAEliminar->getArcosRef() = 0; // Limpiar la lista de adyacencia

    // 3. Eliminar arcos entrantes hacia el vértice a eliminar (¡Parte costosa!)
    actual = primero;
    while (actual != 0) {  
        if (actual != verticeAEliminar) { // No procesar el vértice que estamos eliminando
             nodoArco<elem>* arcoAdj = actual->getArcos();
             nodoArco<elem>* arcoAdjAnterior = 0;  
             while(arcoAdj != 0) {  
                if (arcoAdj->getExtremo() == verticeAEliminar) { // Arco apunta al vértice a eliminar
					nodoArco<elem>* arcoAEliminar = arcoAdj;
					if (arcoAdjAnterior == 0) { // Es el primer arco de la lista  
						// actual->setArcos(arcoAdj->getSiguiente()); // Necesitamos modificar
						actual->getArcosRef() = arcoAdj->getSiguiente();
					} else {
						arcoAdjAnterior->setSiguiente(arcoAdj->getSiguiente());
					}
					arcoAdj = arcoAdj->getSiguiente(); // Avanzar antes de borrar
					delete arcoAEliminar;
					nArcos--;
				} else {
					// Avanzar normalmente
					arcoAdjAnterior = arcoAdj;
					arcoAdj = arcoAdj->getSiguiente();
				}
			}
		}
		actual = actual->getProx();
	}

	// 4. Eliminar el vértice de la lista principal
	if (verticeAnterior == 0) { // Era el primer vértice  
		primero = verticeAEliminar->getProx();
   	} else {
	   	verticeAnterior->setProx(verticeAEliminar->getProx());
   	}

   // 5. Liberar memoria del nodo vértice
   delete verticeAEliminar;
   nVertices--;

   return true;
}

template <class elem>
bool grafo<elem>::eliminarArco(const elem& vInfo, const elem& wInfo) {
    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo); // Necesario para comparar punteros

    if (v == 0 || w == 0) {  
        return false; // Vértices no existen
    }

    nodoArco<elem>* arcoActual = v->getArcos();
    nodoArco<elem>* arcoAnterior = 0;  

    // Buscar el arco en la lista de adyacencia de v
    while (arcoActual != 0 && arcoActual->getExtremo() != w) {  
        arcoAnterior = arcoActual;
        arcoActual = arcoActual->getSiguiente();
    }

    if (arcoActual == 0) {  
        return false; // Arco no encontrado
    }

    // Eliminar el arco de la lista
    if (arcoAnterior == 0) { // El arco a eliminar es el primero  
        // v->setArcos(arcoActual->getSiguiente()); // Necesitamos modificar
        v->getArcosRef() = arcoActual->getSiguiente();
    } else {
        arcoAnterior->setSiguiente(arcoActual->getSiguiente());
    }

    delete arcoActual; // Liberar memoria del nodo arco
    nArcos--;
    return true;
}

template <class elem>
bool grafo<elem>::eliminarArcoNoDirigido(const elem& vInfo, const elem& wInfo) {
    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo); // Necesario para comparar punteros

    if (v == 0 || w == 0) {  
        return false; // Vértices no existen
    }
    
    // Eliminar el arco v->w
    if (!eliminarArco(vInfo, wInfo)) {
        return false; // Arco no encontrado o error
    }
    // Eliminar el arco w->v
    if (!eliminarArco(wInfo, vInfo)) {
        // Si no se pudo eliminar el arco w->v, se deshace la eliminación de v->w
        insertarArco(vInfo, wInfo);
        return false; // Arco no encontrado o error
    }
    return true;
}


template <class elem>
nodoArco<elem>* grafo<elem>::existeArco(const elem& vInfo, const elem& wInfo) const {
    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo);
    return buscarArco(v, w); // Reutilizar el helper privado
}

template <class elem>
float grafo<elem>::getPesoArco(const elem& vInfo, const elem& wInfo) const {
    nodoArco<elem>* arco = existeArco(vInfo, wInfo);
    if (arco != 0) {  
        return arco->getPeso();
    } else {
        // Opción 1: Lanzar excepción (stdexcept está en  )
        throw std::runtime_error("El arco no existe.");
        // Opción 2: Devolver un valor especial (ej. infinito, NaN, -1 si los pesos son >= 0)
        // return std::numeric_limits<float>::infinity(); // Requiere <limits>
        // return -1.0f;
    }
}

template <class elem>
std::vector<elem> grafo<elem>::getSucesores(const elem& vInfo) const {
    std::vector<elem> sucesores;
    nodoVertice<elem>* v = buscarVertice(vInfo);
    if (v != 0) {  
        nodoArco<elem>* arcoActual = v->getArcos();
        while (arcoActual != 0) {  
            sucesores.push_back(arcoActual->getExtremo()->getInfo());
            arcoActual = arcoActual->getSiguiente();
        }
    }
    // Si v == 0, devuelve un vector vacío, lo cual es razonable
    return sucesores;
}

template <class elem>
std::vector<elem> grafo<elem>::getPredecesores(const elem& vInfo) const {
    std::vector<elem> predecesores;
    nodoVertice<elem>* w = buscarVertice(vInfo); // El vértice destino
    if (w == 0) return predecesores; // Si el vértice no existe, no tiene predecesores  

    nodoVertice<elem>* verticeActual = primero;
    while (verticeActual != 0) {  
        nodoArco<elem>* arcoActual = verticeActual->getArcos();
        while (arcoActual != 0) {  
            // Si un arco desde verticeActual apunta a w
            if (arcoActual->getExtremo() == w) {
                predecesores.push_back(verticeActual->getInfo());
                break; // No necesitamos seguir buscando en los arcos de este vértice
            }
            arcoActual = arcoActual->getSiguiente();
        }
        verticeActual = verticeActual->getProx();
    }
    return predecesores;
}

template <class elem>
std::vector<elem> grafo<elem>::getVecinos(const elem& vInfo) const {
    // Asumiendo grafo dirigido por la implementación actual: vecinos = sucesores.
    // Si fuera no dirigido, necesitarías insertar arcos (v,w) y (w,v)
    // o combinar sucesores y predecesores aquí.
    return getSucesores(vInfo);
}

template <class elem>
bool grafo<elem>::esNulo() const {
    return nVertices == 0; // O también primero == 0  
}

template <class elem>
int grafo<elem>::gradoSalidaVertice(const elem& info) const {
    nodoVertice<elem>* v = buscarVertice(info);
    int grado = 0;
    if (v != 0) {  
        nodoArco<elem>* arcoActual = v->getArcos();
        while (arcoActual != 0) {  
            grado++;
            arcoActual = arcoActual->getSiguiente();
        }
    } else {
        // Vértice no encontrado, lanzar excepción o devolver error
        throw std::runtime_error("Vertice no encontrado para calcular grado de salida.");
        // return -1; // Alternativa si no se usan excepciones
    }
    return grado;
}

template <class elem>
int grafo<elem>::gradoEntradaVertice(const elem& info) const {
    
    int grado = 0;
    nodoVertice<elem>* w = buscarVertice(info); // El vértice destino
    if (w == 0) {  
        throw std::runtime_error("Vertice no encontrado para calcular grado de entrada.");
        // return -1; // Alternativa
    }

    nodoVertice<elem>* verticeActual = primero;
    while (verticeActual != 0) {  
        nodoArco<elem>* arcoActual = verticeActual->getArcos();
        while (arcoActual != 0) {  
            if (arcoActual->getExtremo() == w) {
                grado++;
            }
            arcoActual = arcoActual->getSiguiente();
        }
        verticeActual = verticeActual->getProx();
    }
    return grado;
}

template <class elem>
int grafo<elem>::gradoVertice(const elem& info) const {
    // Para grafos dirigidos, usualmente es gradoSalida + gradoEntrada
    // Para no dirigidos, es el número de arcos incidentes.
    // Calcularemos suma de entrada y salida por ahora.
    try {
        // Nota: buscarVertice se llama 3 veces (una aquí implícita, una en gradoSalida, una en gradoEntrada)
        // Se podría optimizar buscando el vértice una sola vez al principio.
        nodoVertice<elem>* v = buscarVertice(info);
        if (v == 0) {  
            throw std::runtime_error("Vertice no encontrado para calcular grado.");
        }
        // Si ya sabemos que existe, podemos llamar a las otras funciones
        // o calcular ambos grados aquí mismo para evitar búsquedas repetidas.
        // Por simplicidad, llamamos a las otras funciones:
        return gradoSalidaVertice(info) + gradoEntradaVertice(info);
    } catch (const std::runtime_error& e) {
        // Propagar la excepción o devolver un valor de error
        // std::cerr << e.what() << std::endl; // Opcional: imprimir error
        throw; // Re-lanzar la excepción
        // return -1; // Alternativa
    }
}

template <class elem>
bool grafo<elem>::esCompleto() const {
    // Un grafo dirigido es completo si para cada par de vértices distintos (v, w),
    // existen los arcos (v, w) y (w, v).
    // Casos base: 0 o 1 vértice son completos.
    if (nVertices <= 1) {
        return true;
    }

    nodoVertice<elem>* v = primero;
    while (v != 0) {
        nodoVertice<elem>* w = primero;
        while (w != 0) {
            if (v == w) { // No comparar un vértice consigo mismo
                w = w->getProx();
                continue;
            }

            // Verificar si existe arco v -> w
            bool arcoVW = (buscarArco(v, w) != 0);
            // Verificar si existe arco w -> v
            bool arcoWV = (buscarArco(w, v) != 0);

            if (!arcoVW || !arcoWV) {
                // Si falta alguno de los dos arcos para este par, no es completo
                return false;
            }
            w = w->getProx();
        }
        v = v->getProx();
    }

    // Si se revisaron todos los pares y todos tenían ambos arcos, es completo
    return true;
}

template <class elem>
bool grafo<elem>::esConexo() const {
    // Verifica si el grafo es débilmente conexo usando BFS.
    // Tratamos las aristas como no dirigidas durante el recorrido.
    // Nota: Esta implementación es ineficiente debido a la búsqueda de predecesores.

    if (nVertices <= 1) {
        // Grafo vacío o con un solo vértice se considera conexo.
        return true;
    }

    std::map<elem, bool> visitados; // Mapa para registrar vértices visitados
    std::queue<elem> cola;          // Cola para BFS
    int contadorVisitados = 0;

    // Empezar BFS desde el primer vértice (si existe)
    if (primero != 0) {
        elem inicioInfo = primero->getInfo();
        cola.push(inicioInfo);
        visitados[inicioInfo] = true;
        contadorVisitados = 1;
    } else {
        return true; // Grafo vacío es conexo
    }


    while (!cola.empty()) {
        elem uInfo = cola.front();
        cola.pop();
        nodoVertice<elem>* u = buscarVertice(uInfo); // O(V) - Ineficiente pero necesario aquí

        if (u == 0) continue; // Seguridad, aunque no debería pasar si está en la cola

        // 1. Explorar Sucesores (arcos salientes u -> v)
        nodoArco<elem>* arcoSuc = u->getArcos();
        while (arcoSuc != 0) {
            nodoVertice<elem>* v = arcoSuc->getExtremo();
            elem vInfo = v->getInfo();
            // 'count' verifica si la clave existe en C++98 map
            if (visitados.count(vInfo) == 0) { // Si no ha sido visitado
                visitados[vInfo] = true;
                cola.push(vInfo);
                contadorVisitados++;
            }
            arcoSuc = arcoSuc->getSiguiente();
        }

        // 2. Explorar Predecesores (arcos entrantes p -> u) - ¡Parte MUY ineficiente! O(V*E)
        nodoVertice<elem>* pVert = primero;
        while (pVert != 0) {
            nodoArco<elem>* arcoPred = pVert->getArcos();
            while (arcoPred != 0) {
                if (arcoPred->getExtremo() == u) { // pVert es un predecesor de u
                    elem pInfo = pVert->getInfo();
                    if (visitados.count(pInfo) == 0) { // Si no ha sido visitado
                        visitados[pInfo] = true;
                        cola.push(pInfo);
                        contadorVisitados++;
                    }
                    // Encontrado el arco p->u, no necesitamos seguir mirando arcos de pVert
                    break;
                }
                arcoPred = arcoPred->getSiguiente();
            }
            pVert = pVert->getProx();
        }
    }

    // Si el número de vértices visitados es igual al total, el grafo es (débilmente) conexo
    return contadorVisitados == nVertices;
}


template <class elem>
void grafo<elem>::copiar(grafo<elem>* target) {
    // Realiza una copia profunda (deep copy) de este grafo en target.
    // Asume que target es un puntero válido a un objeto grafo.

    if (target == 0) {
        // Opcional: lanzar excepción o simplemente retornar si target es nulo
        throw std::runtime_error("El grafo destino (target) no puede ser nulo.");
        // return;
    }
    if (target == this) {
        return; // No hacer nada si se intenta copiar sobre sí mismo
    }

    // 1. Vaciar el grafo destino para empezar limpio
    target->vaciar();

    // 2. Copiar todos los vértices
    nodoVertice<elem>* vActual = this->primero;
    while (vActual != 0) {
        target->insertarVertice(vActual->getInfo());
        vActual = vActual->getProx();
    }

    // 3. Copiar todos los arcos
    vActual = this->primero;
    while (vActual != 0) {
        elem vInfo = vActual->getInfo();
        nodoArco<elem>* arcoActual = vActual->getArcos();
        while (arcoActual != 0) {
            elem wInfo = arcoActual->getExtremo()->getInfo();
            float peso = arcoActual->getPeso();
            // Insertar el arco correspondiente en el grafo destino
            target->insertarArco(vInfo, wInfo, peso);
            arcoActual = arcoActual->getSiguiente();
        }
        vActual = vActual->getProx();
    }

    // Los contadores nVertices y nArcos de target se actualizan
    // automáticamente por sus métodos insertarVertice/insertarArco.
    // Si tuvieras setters públicos y quisieras usarlos (no recomendado):
    // target->setNroVertices(this->nVertices);
    // target->setNroArcos(this->nArcos);
}

#endif // GRAFO_HPP