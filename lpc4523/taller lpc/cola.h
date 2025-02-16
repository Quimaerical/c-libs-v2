#ifndef cola_h
#define cola_h
#include <iostream>
#include "nodo.h"

using namespace std;

template <class elem>
class cola{
	protected:
		nodo<elem>* prim;
		nodo<elem>* ult;
		int longi;
	public:
		cola();
		~cola();
		elem longitud();
		nodo<elem>* getPrim();
		nodo<elem>* getUlt();
		void setLongi(int L);
		void setPrim(nodo<elem>* primero);
		void setUlt(nodo<elem>* ultimo);
		void encolar(elem e);
		void desencolar();
		void invertir();
		cola<elem> copia();
		cola<elem> subcola(int primer, int ultim);
		elem frente();
		bool esVacia();
};

template <class elem>
cola<elem> :: cola(){
	prim = 0;
	ult = 0;
	longi = 0;
}

template <class elem>
cola<elem> :: ~cola(){ 
	nodo<elem>* act = prim;
	nodo<elem>* sig = 0;

	while (act != 0) {
		sig = act->getProx();
		delete act;
		act = sig;
	}

	prim = 0;
	ult = 0;
	longi = 0;
}

template <class elem>
elem cola<elem> ::longitud() {
	return (longi);
}

template <class elem>
nodo<elem>* cola<elem> ::getPrim() {
	return (prim);
}

template <class elem> 
nodo<elem>* cola<elem> ::getUlt() {
	return(ult);
}

template <class elem>
void cola<elem> ::setLongi(int L) {
	longi = L;
}

template <class elem> 
void cola<elem> ::setPrim(nodo<elem>* primero) {
	prim = primero;
}

template<class elem> 
void cola<elem> ::setUlt(nodo<elem>* ultimo) {
	ult = ultimo;
}

template <class elem>
void cola<elem> ::encolar(elem e){
	nodo<elem>* ptrNodo = new nodo<elem>(0, ult, e);
	if (longi == 0) {
		ult = ptrNodo;
		prim = ult;
	}
	else {
		ult->setProx(ptrNodo);
		ult = ptrNodo;
	}

	longi++;
}

template <class elem>
void cola<elem> :: desencolar(){
	nodo<elem>* ptrNodo;
	if (longi == 1) {
		ptrNodo = prim;
		delete ptrNodo;
		longi--;
	}
	else if (longi > 1) {
		ptrNodo = prim;
		prim = prim->getProx();
		ptrNodo->setProx(0);
		prim->setPrev(0);
		delete ptrNodo;
	}
}

template <class elem>
void cola<elem> :: invertir(){
	nodo<elem>* aux = prim;
	prim = this->getUlt();
	ult = aux;
}		

template <class elem>
cola<elem> cola<elem> :: copia(){
	cola<elem> col = cola<elem>();
	nodo<elem>* p = prim;
	
	while(p != 0){
		col.encolar(p->getDato());
		p = p->getProx();
	}
	
	return col;
}


template <class elem>
cola<elem> cola<elem> :: subcola(int primer, int ultim){
	cola<elem> aux = cola<elem>();
	nodo<elem>* nod = prim;

	if(primer <= ultim && primer > 0 && ultim <= this->longitud()){
		ultim = ultim + 1 - primer;
	
		while(--primer > 0){
			nod = nod->getProx();
		}
		while(ultim-- > 0){
			aux.encolar(nod->getDato());
			nod = nod->getProx();
		}	
	}

	return aux;
}

template <class elem>
elem cola<elem> :: frente(){
	return(prim->getDato());
}

template <class elem>
bool cola<elem> :: esVacia(){
	return (longi == 0);
}

#endif