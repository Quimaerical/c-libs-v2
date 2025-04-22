#ifndef GRAFO_NODO_ARCO_HPP
#define GRAFO_NODO_ARCO_HPP

#include <iostream>

using namespace std;

template <class elem> class grafo;
template <class elem> class nodoVertice;

template <class elem>
class nodoArco {
private:
    float peso;
		nodoVertice<elem>* extremoArco;
		nodoArco<elem>* sig;

	public:
		// Constructores
		nodoArco();
		nodoArco(nodoVertice<elem>* extremo);
		nodoArco(nodoVertice<elem>* extremo, float p);
		nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco);
		nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco, float p);

		// Setters
		void setPeso(float nPeso);
		void setExtremo(nodoVertice<elem>* extremo);
		void setSiguiente(nodoArco<elem>* siguiente);

		// Getters
		float getPeso() const;
		nodoVertice<elem>* getExtremo() const;
		nodoArco<elem>* getSiguiente() const;
};

// --- Implementaciones ---
// (Necesarias en el .hpp para templates)

template <class elem>
nodoArco<elem>::nodoArco() {
	peso = 0.0;
	extremoArco = 0;
	sig = 0;
}

template <class elem>
nodoArco<elem>::nodoArco(nodoVertice<elem>* extremo) {
	peso = 0.0;
    extremoArco = extremo;
	sig = 0;
}

template <class elem>
nodoArco<elem>::nodoArco(nodoVertice<elem>* extremo, float p) {
    peso = p;
    extremoArco = extremo;
	sig = 0;
}

template <class elem>
nodoArco<elem>::nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco) {
    peso = 0.0f;
    extremoArco = extremo;
    sig = arco;
}

template <class elem>
nodoArco<elem>::nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco, float p) {
    peso = p;
    extremoArco = extremo;
    sig = arco;
}

template <class elem>
void nodoArco<elem>::setPeso(float nPeso) {
    peso = nPeso;
}

template <class elem>
void nodoArco<elem>::setExtremo(nodoVertice<elem>* extremo) { 
    extremoArco = extremo;
}

template <class elem>
void nodoArco<elem>::setSiguiente(nodoArco<elem>* siguiente) {
    sig = siguiente;
}

template <class elem>
float nodoArco<elem>::getPeso() const {
    return peso;
}

template <class elem>
nodoVertice<elem>* nodoArco<elem>::getExtremo() const {
    return extremoArco;
}

template <class elem>
nodoArco<elem>* nodoArco<elem>::getSiguiente() const {
    return sig;
}

#endif