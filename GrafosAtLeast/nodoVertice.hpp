#ifndef GRAFO_NODO_VERTICE_HPP
#define GRAFO_NODO_VERTICE_HPP

#include "nodoArco.hpp"

template <class elem> class grafo;

template <class elem> 
class nodoVertice {
	private:
		elem info;
		nodoVertice<elem>* prox;
		nodoArco<elem>* adyacentes;
	
	public:
		// Constructores
		nodoVertice();
		nodoVertice(const elem& dato);
		nodoVertice(nodoArco<elem>* ady);
		nodoVertice(elem dato, nodoArco<elem>* ady);
		nodoVertice(nodoVertice<elem>* proximo, nodoArco<elem>* ady);
		nodoVertice(elem dato, nodoVertice<elem>* proximo, nodoArco<elem>* ady);
		// Setters
		void setInfo(const elem& inf); 
		void setProx(nodoVertice<elem>* proximo);
		void setArcos(nodoArco<elem>* arco);

		// Getters
		elem getInfo() const; 
		elem& getInfoRef(); 
		nodoVertice<elem>* getProx() const; 
		nodoArco<elem>* getArcos() const; 
		nodoArco<elem>*& getArcosRef();
};

// --- Implementaciones ---
// (Necesarias en el .hpp para templates, fuera de la clase)


template <class elem>
nodoVertice<elem>::nodoVertice() {
    prox = 0;
    adyacentes = 0;
}

template <class elem>
nodoVertice<elem>::nodoVertice(const elem& dato) {
    info = dato; // Asume que elem tiene operador de asignación o constructor de copia
    prox = 0;
    adyacentes = 0;
}

template <class elem>
nodoVertice<elem>::nodoVertice(nodoArco<elem>* ady) {
	info = 0;
	prox = 0;
	adyacentes = ady;
}

template <class elem>
nodoVertice<elem>::nodoVertice(elem dato, nodoArco<elem>* ady) {
	info=dato;
	prox=0;
	adyacentes = ady;
}

template <class elem>
nodoVertice<elem>::nodoVertice(nodoVertice<elem>* proximo, nodoArco<elem>* ady) {
	info = 0;
	prox = proximo;
	adyacentes = ady;
}

template <class elem>
nodoVertice<elem>::nodoVertice(elem dato, nodoVertice<elem>* proximo, nodoArco<elem>* ady) {
	info = dato;
	prox = proximo;
	adyacentes = ady;
}

template <class elem>
void nodoVertice<elem>::setInfo(const elem& inf) {
    info = inf;
}

template <class elem>
void nodoVertice<elem>::setProx(nodoVertice<elem>* proximo) {
    prox = proximo;
}

template <class elem>
void nodoVertice<elem>::setArcos(nodoArco<elem>* arco) {
    adyacentes = arco;
}

template <class elem>
elem nodoVertice<elem>::getInfo() const {
    return info;
}

template <class elem>
elem& nodoVertice<elem>::getInfoRef() {
    return info;
}

template <class elem>
nodoVertice<elem>* nodoVertice<elem>::getProx() const {
    return prox;
}

template <class elem>
nodoArco<elem>* nodoVertice<elem>::getArcos() const {
    return adyacentes;
}

template <class elem>
nodoArco<elem>*& nodoVertice<elem>::getArcosRef() {
    return adyacentes;
}

#endif // GRAFO_NODO_VERTICE_HPP
