#ifndef pila_h
#define pila_h
#include "nodo.h"

template <class elem>
class pila {
	private:
		nodo<elem>* prim;
		nodo<elem>* ult;
		int longi;
	public:
		pila();
		~pila();
		int longitud();
		nodo<elem>* getPrim();
		nodo<elem>* getUlt();
		void setPrim(nodo<elem>* primero);
		void setUlt(nodo<elem>* ultimo);
		void setLongit(int l);
		void apilar(elem e);
		void desapilar();
		void invertir();
		pila<elem> copia();
		pila<elem> subpila(int primer, int ultim);
		elem tope();
		bool esVacia();
		void concatenar(pila<elem> b);
};

template <class elem>
pila<elem> :: pila(){
	prim = 0;
	ult = 0;
	longi = 0;
}

template <class elem>
pila<elem> :: ~pila(){
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

template<class elem>
int pila<elem>::longitud() {
	return(longi);
}

template<class elem>
nodo<elem>* pila<elem>::getPrim() {
	return(prim);
}

template<class elem>
nodo<elem>* pila<elem>::getUlt() {
	return(ult);
}

template<class elem>
void pila<elem>::setPrim(nodo<elem>* primero) {
	prim = primero;
}

template<class elem>
void pila<elem>::setUlt(nodo<elem>* ultimo) {
	ult = ultimo;
}

template<class elem>
void pila<elem>::setLongit(int l) {
	longi = l;
}
template <class elem>
void pila<elem> :: apilar(elem e){
	nodo<elem>* ptrNodo = new nodo<elem>(prim, 0, e);
	
	if (longi == 0) {
		prim = ptrNodo;
		ult = prim;
	}
	else {
		prim->setPrev(ptrNodo);
		prim = ptrNodo;
	}
	longi++;
}

template <class elem>
void pila<elem> :: desapilar(){
	nodo<elem>* ptrNodo = prim;
	if (longi == 1) {
		prim = 0;
		ult = 0;
		delete ptrNodo;	
		longi--;
	}
	else if (longi < 1) {
		ptrNodo = 0;
	}
	else {
		prim = prim->getProx();
		ptrNodo->setProx(0);
		delete ptrNodo;
		longi--;
	}
}

template <class elem>
void pila<elem> :: invertir(){
	nodo<elem>* aux = prim;
	prim = ult;
	ult = aux;
}

template <class elem>
pila<elem> pila<elem> :: copia(){
	pila<elem> pil = pila<elem>();
	nodo<elem>* p = this->prim;
	
	while(p != 0){
		pil.apilar(p->getDato());
		p = p->getProx();
	}
	pil.invertir();
	
	return pil;
}

template <class elem>
pila<elem> pila<elem> :: subpila(int primer, int ultim){
	pila<elem> aux = pila<elem>();
	nodo<elem>* nod = this->pila<elem> :: prim;

	if(primer <= ultim && primer > 0 && ultim <= this->pila<elem> :: longitud()){
		ultim = ultim + 1 - primer;
	
		while(--primer > 0){
			nod = nod->getProx();
		}
		while(ultim-- > 0){
			aux.apilar(nod->getDato());
			nod = nod->getProx();
		}	
	}
	aux.invertir();

	return aux;
}

template <class elem>
elem pila<elem> :: tope(){
	return (prim->getDato());
}

template <class elem>
bool pila<elem> :: esVacia(){
	return (longi == 0);
}

template <class elem>
void concatenar(pila<elem> b){
	this->ult->setProx(b.getPrim());
	this->ult = b.getUlt();
}

#endif