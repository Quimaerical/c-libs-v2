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
		void concatenar(cola<elem> b);
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
	if (prim!=nullptr) return(prim); else return(0);
}

template <class elem> 
nodo<elem>* cola<elem> ::getUlt() {
	if (ult!=nullptr) return(ult); else return (0);
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
		prim = 0;
		ult = 0;
		delete ptrNodo;
		longi--;
	}
	else if (longi < 1) {
		ptrNodo = 0;
		if (prim != nullptr) delete prim;
		prim = 0;
		ult = 0;
		longi = 0;
	}
	else{
		ptrNodo = prim;
		prim = prim->getProx();
		ptrNodo->setProx(0);
		prim->setPrev(0);
		delete ptrNodo;
		longi--;
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
	if (prim != nullptr) return (prim->getDato()); else return(0);
}

template <class elem>
bool cola<elem> :: esVacia(){
	return (longi == 0);
}

template <class elem>
void cola<elem>::concatenar(cola<elem> b) {
    if (b.esVacia()) return;

    if (this->esVacia()) {
        *this = b;  // Si la cola actual está vacía, simplemente la igualamos a b
    } else {
        this->ult->setProx(b.getPrim());
        b.getPrim()->setPrev(this->ult);
        this->ult = b.getUlt();
    }
    this->longi += b.longitud();
}


#endif
