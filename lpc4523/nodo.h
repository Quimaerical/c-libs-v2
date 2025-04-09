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
	elem& getDato();
};

template<class elem>
nodo<elem>::nodo() : prox(NULL), prev(NULL) {}

template<class elem>
nodo<elem>::nodo(nodo<elem>* proximo, nodo<elem>* previo, elem data) : prox(proximo), prev(previo), dato(data) {}

template<class elem>
nodo<elem> ::~nodo() {
	prev = NULL;
	prox = NULL;
}

template<class elem>
void nodo<elem> ::setProx(nodo<elem>* proximo) {
	if (proximo != NULL){
		prox = proximo;
	} else {
		prox = NULL;
	}
}

template<class elem>
void nodo<elem> ::setPrev(nodo<elem>* previo) {
	if (previo != NULL){
		prev = previo;
	} else {
		prev = NULL;
	}
}

template<class elem>
void nodo<elem> ::setDato(elem data) {
	dato = data;
}

template<class elem>
nodo<elem>* nodo<elem> ::getProx() {
	if (prox!=NULL) return prox; else return NULL;
}

template<class elem>
nodo<elem>* nodo<elem> ::getPrev() {
	if (prev!=NULL) return prev; else return NULL;
}

template<class elem>
elem& nodo<elem> ::getDato() {
	return dato;
}

#endif
