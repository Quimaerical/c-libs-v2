#ifndef grafo_nodo_arco
#define grafo_nodo_arco

#include <iostream>

using namespace std;

template <class elem> class grafo;

template <class elem>
class nodoArco{
	private:
		float peso;
		nodoVertice<elem>* extremoArco;
		nodoArco<elem>* sig;
	protected:
	private:
		nodoArco();
		nodoArco(nodoVertice<elem>* extremo);
		nodoArco(nodoVertice<elem>* extremo, float p);
		nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco);
		nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco, float p);
		void setPeso(float nPeso);
		void setExtremo(nodoVertice<elem>* extremo);
		void setSiguiente(nodoArco<elem>* siguiente);
		float getPeso();
		nodoVertice<elem>* getExtremo();
		nodoArco<elem>* getSiguiente();
		
};

nodoArco() {
	peso = 0;
	extremoArco = 0;
	sig = 0;
}

nodoArco(nodoVertice<elem>* extremo) {
	peso = 0;
	extremoArco = extremo;
	sig = 0;
}

nodoArco(nodoVertice<elem>* extremo, float p) {
	peso = p;
	extremoArco = extremo;
	sig = 0;
}

nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco) {
	peso = 0;
	extremoArco = extremo;
	sig = arco;
}

nodoArco(nodoVertice<elem>* extremo, nodoArco<elem>* arco, float p) {
	peso = p;
	extremoArco = extremo;
	sig = arco;
}

void setPeso(float nPeso) {
	peso = nPeso;
}

void setExtremoArco(nodoVertice<elem>* extremo){
	extremoArco = extremo;
}

void setSiguiente(nodoArco<elem>* siguiente){
	sig = siguiente;
}

float getPeso(){
	return peso;
}

nodoVertice<elem>* getExtremo(){
	return extremoArco;
}

nodoArco<elem>* getSiguiente(){
	return sig;
}

#endif