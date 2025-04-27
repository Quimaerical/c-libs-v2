#ifndef GRAFO_NO_DIRIGIDO_HPP
#define GRAFO_NO_DIRIGIDO_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <stdexcept>
#include <limits>
#include <map>
#include <set>
#include "nodoVertice.hpp"

template <class elem>
class grafoNoDirigido {
private:
    nodoVertice<elem>* primero;
    int nVertices;
    int nArcosInternos; // Almacena el número de arcos dirigidos internos (2 por cada Arco no dirigida)

    // --- Funciones Auxiliares Privadas ---

    nodoVertice<elem>* buscarVertice(const elem& info) const {
        nodoVertice<elem>* actual = primero;
        while (actual != 0) {
            if (actual->getInfo() == info) {
                return actual;
            }
            actual = actual->getProx();
        }
        return 0; // No encontrado
    }

    // Busca un arco dirigido v -> w
    nodoArco<elem>* buscarArcoDirigido(nodoVertice<elem>* origen, nodoVertice<elem>* destino) const {
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

    // Elimina un arco dirigido v -> w (función auxiliar interna)
    bool eliminarArcoDirigidoInterno(const elem& vInfo, const elem& wInfo) {
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
            v->getArcosRef() = arcoActual->getSiguiente();
        } else {
            arcoAnterior->setSiguiente(arcoActual->getSiguiente());
        }

        delete arcoActual; // Liberar memoria del nodo arco
        nArcosInternos--; // Decrementar contador interno
        return true;
    }


public:
    // --- Constructores y Destructor ---
    grafoNoDirigido ();
    ~grafoNoDirigido ();

    // --- Getters ---
    nodoVertice<elem>* getPrimero() const;
    int getNroVertices() const;
    int getNroArco() const; // Devuelve el número de Arco no dirigidas
    int getNroArcosInternos() const; // Devuelve el número de arcos dirigidos almacenados
    std::vector<elem> getVertices() const;

    // --- Modificadores del Grafo ---
    bool insertarVertice(const elem& info);
    bool insertarArco(const elem& vInfo, const elem& wInfo, float peso = 0.0f); // Insertar Arco no dirigida {v, w}
    bool eliminarVertice(const elem& info);
    bool eliminarArco(const elem& vInfo, const elem& wInfo); // Eliminar Arco no dirigida {v, w}

    // --- Consultas sobre Arcos ---
    bool existeArco(const elem& vInfo, const elem& wInfo) const;
    float getPesoArco(const elem& vInfo, const elem& wInfo) const;

    // --- Consultas sobre Vecindad ---
    std::vector<elem> getVecinos(const elem& vInfo) const;

    // --- Propiedades del Grafo ---
    bool esNulo() const;
    bool esCompleto() const;
    bool esConexo() const;

    // --- Propiedades de Vértices ---
    int gradoVertice(const elem& info) const;

    // --- Otras Operaciones ---
    void vaciar();
    void copiar(grafoNoDirigido<elem>* target);
};

// ------------------------------------------------------------------
// --- Implementaciones ---                                          |
// ------------------------------------------------------------------

template <class elem>
grafoNoDirigido<elem>::grafoNoDirigido() : primero(0), nVertices(0), nArcosInternos(0) {}

template <class elem>
grafoNoDirigido<elem>::~grafoNoDirigido() {
    vaciar();
}

template <class elem>
void grafoNoDirigido<elem>::vaciar() {
    nodoVertice<elem>* verticeActual = primero;
    while (verticeActual != 0) {
        nodoArco<elem>* arcoActual = verticeActual->getArcos();
        while (arcoActual != 0) {
            nodoArco<elem>* arcoAEliminar = arcoActual;
            arcoActual = arcoActual->getSiguiente();
            delete arcoAEliminar;
        }
        nodoVertice<elem>* verticeAEliminar = verticeActual;
        verticeActual = verticeActual->getProx();
        delete verticeAEliminar;
    }
    primero = 0;
    nVertices = 0;
    nArcosInternos = 0;
}

template <class elem>
nodoVertice<elem>* grafoNoDirigido<elem>::getPrimero() const {
    return primero;
}

template <class elem>
int grafoNoDirigido<elem>::getNroVertices() const {
    return nVertices;
}

template <class elem>
int grafoNoDirigido<elem>::getNroArcos() const {
    // Cada Arco no dirigida se almacena como dos arcos dirigidos internos
    return nArcosInternos / 2;
}

template <class elem>
int grafoNoDirigido<elem>::getNroArcosInternos() const {
    return nArcosInternos;
}


template <class elem>
std::vector<elem> grafoNoDirigido<elem>::getVertices() const {
    std::vector<elem> verticesInfo;
    nodoVertice<elem>* auxVertice = primero;
    while (auxVertice != 0) {
        verticesInfo.push_back(auxVertice->getInfo());
        auxVertice = auxVertice->getProx();
    }
    return verticesInfo;
}

template <class elem>
bool grafoNoDirigido<elem>::insertarVertice(const elem& info) {
    if (buscarVertice(info) != 0) {
        return false; // Ya existe
    }

    nodoVertice<elem>* nuevoVertice = new nodoVertice<elem>(info);
    if (esNulo()) {
        primero = nuevoVertice;
    } else {
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
bool grafoNoDirigido<elem>::insertarArco(const elem& vInfo, const elem& wInfo, float peso) {
    // No permitir auto-ciclos en grafos no dirigidos simples
    if (vInfo == wInfo) {
        return false;
    }

    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo);

    if (v == 0 || w == 0) {
        return false; // Uno o ambos vértices no existen
    }

    // Verificar si la Arco (en cualquier dirección) ya existe
    if (buscarArcoDirigido(v, w) != 0 || buscarArcoDirigido(w, v) != 0) {
        return false; // La Arco ya existe
    }

    // Crear los dos arcos dirigidos que representan la Arco no dirigida
    nodoArco<elem>* arcoVW = new nodoArco<elem>(w, peso); // Arco v -> w
    nodoArco<elem>* arcoWV = new nodoArco<elem>(v, peso); // Arco w -> v

    // Insertar arco v -> w en la lista de adyacencia de v
    arcoVW->setSiguiente(v->getArcos());
    v->getArcosRef() = arcoVW;

    // Insertar arco w -> v en la lista de adyacencia de w
    arcoWV->setSiguiente(w->getArcos());
    w->getArcosRef() = arcoWV;

    nArcosInternos += 2; // Se añadieron dos arcos internos
    return true;
}

template <class elem>
bool grafoNoDirigido<elem>::eliminarVertice(const elem& info) {
    nodoVertice<elem>* verticeAEliminar = 0;
    nodoVertice<elem>* verticeAnterior = 0;

    // 1. Buscar el vértice y su anterior
    nodoVertice<elem>* actual = primero;
    while (actual != 0 && actual->getInfo() != info) {
        verticeAnterior = actual;
        actual = actual->getProx();
    }

    if (actual == 0) {
        return false; // Vértice no encontrado
    }
    verticeAEliminar = actual;

    // 2. Eliminar arcos "salientes" (v -> x) y actualizar nArcosInternos
    //    Estos representan una mitad de las Arcos incidentes.
    nodoArco<elem>* arcoActual = verticeAEliminar->getArcos();
    while (arcoActual != 0) {
        nodoArco<elem>* arcoAEliminar = arcoActual;
        nodoVertice<elem>* vecino = arcoActual->getExtremo(); // El vértice x
        arcoActual = arcoActual->getSiguiente();

        // ¡Importante! También debemos eliminar el arco en la otra dirección (x -> v)
        // Usamos la función auxiliar interna que ya decrementa nArcosInternos
        eliminarArcoDirigidoInterno(vecino->getInfo(), verticeAEliminar->getInfo());

        delete arcoAEliminar;
        // nArcosInternos--; // Decrementado dentro de eliminarArcoDirigidoInterno
    }
    verticeAEliminar->getArcosRef() = 0; // Limpiar la lista de adyacencia del vértice a eliminar

    // 3. Eliminar arcos "entrantes" (y -> v) - ESTO YA SE HIZO EN EL PASO 2
    //    En la implementación original de grafo dirigido, este paso era necesario.
    //    Aquí, al eliminar v->x, también eliminamos x->v, cubriendo todas las Arcos.
    //    Por lo tanto, el bucle original para eliminar arcos entrantes se puede omitir.

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
bool grafoNoDirigido<elem>::eliminarArco(const elem& vInfo, const elem& wInfo) {
    // Eliminar ambos arcos dirigidos v->w y w->v
    bool eliminadoVW = eliminarArcoDirigidoInterno(vInfo, wInfo);
    bool eliminadoWV = eliminarArcoDirigidoInterno(wInfo, vInfo);

    // Devuelve true si al menos uno existía y fue eliminado.
    // O podrías requerir que ambos existieran: return eliminadoVW && eliminadoWV;
    // O si la Arco realmente existía (ambos arcos presentes):
    // Si uno se eliminó pero el otro no (estado inconsistente), podría ser un error.
    // Por simplicidad, retornamos true si algo se eliminó.
    return eliminadoVW || eliminadoWV;
    // Una semántica más estricta sería:
    // if (eliminadoVW && eliminadoWV) return true; // Ambos eliminados correctamente
    // if (!eliminadoVW && !eliminadoWV) return false; // Ninguno existía
    // // Si uno existía y el otro no, es un estado inconsistente, podrías lanzar una excepción o retornar false.
    // // throw std::runtime_error("Inconsistencia al eliminar Arco no dirigida.");
    // return false; // O retornar false en caso de inconsistencia
}

template <class elem>
bool grafoNoDirigido<elem>::existeArco(const elem& vInfo, const elem& wInfo) const {
    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo);
    // En un grafo no dirigido consistente, si v->w existe, w->v también debería existir.
    // Basta con comprobar uno, pero por seguridad podemos comprobar v->w.
    return buscarArcoDirigido(v, w) != 0;
    // Opcionalmente, comprobar ambos para detectar inconsistencias:
    // return buscarArcoDirigido(v, w) != 0 && buscarArcoDirigido(w, v) != 0;
}

template <class elem>
float grafoNoDirigido<elem>::getPesoArco(const elem& vInfo, const elem& wInfo) const {
    nodoVertice<elem>* v = buscarVertice(vInfo);
    nodoVertice<elem>* w = buscarVertice(wInfo);
    nodoArco<elem>* arco = buscarArcoDirigido(v, w); // Buscar el arco v->w

    if (arco != 0) {
        return arco->getPeso();
    } else {
        // Si v->w no existe, la Arco no existe (asumiendo consistencia)
        throw std::runtime_error("La Arco no existe.");
        // return std::numeric_limits<float>::infinity(); // Alternativa
    }
}

template <class elem>
std::vector<elem> grafoNoDirigido<elem>::getVecinos(const elem& vInfo) const {
    std::vector<elem> vecinos;
    nodoVertice<elem>* v = buscarVertice(vInfo);
    if (v != 0) {
        nodoArco<elem>* arcoActual = v->getArcos(); // Arcos v -> x
        while (arcoActual != 0) {
            vecinos.push_back(arcoActual->getExtremo()->getInfo()); // Añadir x
            arcoActual = arcoActual->getSiguiente();
        }
    }
    return vecinos;
}

template <class elem>
bool grafoNoDirigido<elem>::esNulo() const {
    return nVertices == 0;
}

template <class elem>
int grafoNoDirigido<elem>::gradoVertice(const elem& info) const {
    nodoVertice<elem>* v = buscarVertice(info);
    int grado = 0;
    if (v != 0) {
        nodoArco<elem>* arcoActual = v->getArcos(); // Contar arcos v -> x
        while (arcoActual != 0) {
            grado++;
            arcoActual = arcoActual->getSiguiente();
        }
    } else {
        throw std::runtime_error("Vertice no encontrado para calcular grado.");
        // return -1; // Alternativa
    }
    return grado;
}

template <class elem>
bool grafoNoDirigido<elem>::esCompleto() const {
    // Un grafo no dirigido es completo si existe una Arco entre cada par de vértices distintos.
    // El número de Arcos debe ser nV * (nV - 1) / 2.
    // El número de arcos internos almacenados debe ser nV * (nV - 1).
    if (nVertices <= 1) {
        return true;
    }
    long long expectedInternalArcs = (long long)nVertices * (nVertices - 1);
    return nArcosInternos == expectedInternalArcs;

    /* // Alternativa: Comprobación explícita (más lenta)
    if (nVertices <= 1) return true;
    nodoVertice<elem>* v = primero;
    while (v != 0) {
        nodoVertice<elem>* w = primero;
        while (w != 0) {
            if (v == w) { // No comparar consigo mismo
                 w = w->getProx();
                 continue;
            }
            // Verificar si existe la Arco (comprobando un arco dirigido es suficiente)
            if (buscarArcoDirigido(v, w) == 0) {
                return false; // Falta una Arco
            }
            w = w->getProx();
        }
        v = v->getProx();
    }
    return true;
    */
}

template <class elem>
bool grafoNoDirigido<elem>::esConexo() const {
    if (nVertices <= 1) {
        return true; // Grafo vacío o con un solo vértice es conexo.
    }

    std::map<elem, bool> visitados;
    std::queue<elem> cola;
    int contadorVisitados = 0;

    // Empezar BFS desde el primer vértice (si existe)
    if (primero != 0) {
        elem inicioInfo = primero->getInfo();
        cola.push(inicioInfo);
        visitados[inicioInfo] = true;
        contadorVisitados = 1;
    } else {
        return true; // Grafo vacío es conexo por definición
    }

    while (!cola.empty()) {
        elem uInfo = cola.front();
        cola.pop();
        nodoVertice<elem>* u = buscarVertice(uInfo); // Ineficiente, pero necesario con esta estructura

        if (u == 0) continue; // Seguridad

        // Explorar vecinos (usando la lista de adyacencia que representa los vecinos)
        nodoArco<elem>* arcoAdj = u->getArcos();
        while (arcoAdj != 0) {
            nodoVertice<elem>* v = arcoAdj->getExtremo();
            elem vInfo = v->getInfo();
            // map::count es más portable que usar [] directamente para chequear existencia
            if (visitados.count(vInfo) == 0) { // Si no ha sido visitado
                visitados[vInfo] = true;
                cola.push(vInfo);
                contadorVisitados++;
            }
            arcoAdj = arcoAdj->getSiguiente();
        }
    }

    // Si el número de vértices visitados es igual al total, el grafo es conexo
    return contadorVisitados == nVertices;
}


template <class elem>
void grafoNoDirigido<elem>::copiar(grafoNoDirigido<elem>* target) {
    if (target == 0) {
        throw std::runtime_error("El grafo destino (target) no puede ser nulo.");
    }
    if (target == this) {
        return; // No copiar sobre sí mismo
    }

    target->vaciar();

    // 1. Copiar todos los vértices
    nodoVertice<elem>* vActual = this->primero;
    while (vActual != 0) {
        target->insertarVertice(vActual->getInfo());
        vActual = vActual->getProx();
    }

    // 2. Copiar todas las Arcos (internamente copia los dos arcos dirigidos)
    vActual = this->primero;
    while (vActual != 0) {
        elem vInfo = vActual->getInfo();
        nodoArco<elem>* arcoActual = vActual->getArcos(); // Arcos v -> w
        while (arcoActual != 0) {
            elem wInfo = arcoActual->getExtremo()->getInfo();
            float peso = arcoActual->getPeso();

            // Importante: Solo insertar la Arco una vez por par {v, w}.
            // Si insertamos para v->w y luego para w->v, duplicaríamos.
            // Insertar solo si v < w (o alguna otra ordenación canónica)
            // O, más simple: intentar insertar y dejar que insertarArco maneje duplicados.
            // Necesitamos asegurarnos que el target->buscarVertice funcione correctamente
            // antes de llamar a insertarArco dentro de este bucle.
            // La forma más segura es iterar y añadir los arcos internos directamente,
            // pero eso rompe la encapsulación. Usar insertarArco es mejor si funciona.

            // Para evitar duplicados al iterar, solo insertamos cuando procesamos
            // el arco v->w donde, por ejemplo, vInfo < wInfo (si elem soporta <)
            // O más genéricamente, solo cuando procesamos el "primer" nodo del par en la lista.
            // La forma más robusta es usar insertarArco y confiar en su chequeo interno.
            // ¡Cuidado! Si elem no tiene operator== bien definido, esto puede fallar.

            // Intentar insertar la Arco. Si ya existe (porque se insertó desde w->v),
            // insertarArco devolverá false y no hará nada.
            target->insertarArco(vInfo, wInfo, peso);


            arcoActual = arcoActual->getSiguiente();
        }
        vActual = vActual->getProx();
    }
     // Los contadores nVertices y nArcosInternos de target se actualizan
    // automáticamente por sus métodos insertarVertice/insertarArco.
}


#endif // grafoNoDirigido_HPP
