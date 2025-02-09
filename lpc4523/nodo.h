#ifndef node_
#define node_
#include <iostream>
 
template <class elem> class lista;
template <class elem> class pila;
template <class elem> class cola;

template <class elem>
class nodo {
private:
	nodo<elem>* prox;
	nodo<elem>* prev;
	elem dato;
public:
	nodo();
	nodo(nodo<elem>* proximo, nodo<elem>* previo, elem data);
	~nodo();
	void setProx(nodo<elem>* proximo);
	void setPrev(nodo<elem>* previo);
	void setDato(elem data);
	nodo<elem>* getProx();
	nodo<elem>* getPrev();
	elem getDato();
};

template<class elem>
nodo<elem> ::nodo() {
	prox = 0;
	prev = 0;
	dato = 0;
}

template<class elem>
nodo<elem> ::nodo(nodo<elem>* proximo, nodo<elem>* previo, elem data) {
	prox = proximo;
	prev = previo;
	dato = data;
}

template<class elem>
nodo<elem> ::~nodo() {
	prev = 0;
	prox = 0;
}

template<class elem>
void nodo<elem> ::setProx(nodo<elem>* proximo) {
	if (proximo != nullptr){
		prox = proximo;
	} else {
		prox = nullptr;
	}
}

template<class elem>
void nodo<elem> ::setPrev(nodo* previo) {
	if (previo != nullptr){
		prev = previo;
	} else {
		prev = nullptr;
	}
}

template<class elem>
void nodo<elem> ::setDato(elem data) {
	dato = data;
}

template<class elem>
nodo<elem>* nodo<elem> ::getProx() {
	return prox;
}

template<class elem>
nodo<elem>* nodo<elem> ::getPrev() {
	return prev;
}

template<class elem>
elem nodo<elem> ::getDato() {
	return dato;
}

#endif
