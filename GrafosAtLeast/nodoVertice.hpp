#ifndef grafo_nodo_vertice
#define grafo_nodo_vertice

#include <iostream>

using namespace std;

template <class elem> class grafo;

template <class elem> 
class nodoVertice{
	private:
		elem info;
		nodoVertice<elem>* prox;
		nodoArco<elem>* adyacentes;
	protected:	
	public:
		nodoVertice();
		nodoVertice(elem dato);
		nodoVertice(nodoArco<elem>* ady);
		nodoVertice(elem dato, nodoArco<elem>* ady);
		nodoVertice(nodoVertice<elem>* proximo, nodoArco<elem>* ady);
		nodoVertice(elem dato, nodoVertice<elem>* proximo, nodoArco<elem>* ady);
		void setInfo(elem inf);
		void setProx(nodoVertice<elem>* proxima);
		void setArcos(nodoArco<elem>* arco);
		elem getInfo();
		nodoVertice<elem>* getProx();
		nodoArco<elem>* getArcos();	
};

nodoVertice(){
	info = 0;
	prox = 0;
	adyacentes = 0;
}

nodoVertice(elem dato){
	info = dato;
	prox = 0;
	adyacentes = 0;
}

nodoVertice(nodoArco<elem>* ady){
	info = 0;
	prox = 0;
	adyacentes = ady;
}

nodoVertice(elem dato, nodoArco<elem>* ady){
	info=dato;
	prox=0;
	adyacentes = ady;
}

nodoVertice(nodoVertice<elem>* proximo, nodoArco<elem>* ady){
	info = 0;
	prox = proximo;
	adyacentes = ady;
}

nodoVertice(elem dato, nodoVertice<elem>* proximo, nodoArco<elem>* ady){
	info = dato;
	prox = proximo;
	adyacentes = ady;
}

void setInfo(elem inf){
	info = inf;
}

void setProx(nodoVertice<elem>* proxima){
	prox = proxima
}

void setArcos(nodoArco<elem>* arco){
	adyacentes = arco;
}

elem getInfo(){
	return info;
}

nodoVertice<elem>* getProx(){
	return prox;
}

nodoArco<elem>* getArcos(){
	return adyacentes;
}

#endif