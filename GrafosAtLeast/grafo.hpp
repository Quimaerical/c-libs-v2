#ifndef grafo
#define grafo

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include "nodoArco.hpp"
#include "nodoVertice.hpp"

using namespace std;

template <class elem>
class grafo{
	private:
		nodoVertice<elem>* primero;
		int nVertices;
		int nArcos;
	protected:
	public:
		grafo ();
		grafo (nodoVertice<elem>* prim);
		grafo (nodoVertice<elem>* prim, int nroVertices, int nroArcos);
		void setPrimero(nodoVertice<elem>* prim);
		void setNroVertices(int numV);
		void setNroArcos(int numA);
		nodoVertice<elem>* getPrimero();
		int getNroVertices();
		int getNroArcos();
		vector<elem> getVertices();
		void insertarVertice(elem info);
		void insertarArco(elem v, w);
		void eliminarVertice(elem info);
		void eliminarArco(elem v, w);
		nodoArco<elem>* existeArco (elem v, w);
		float getPesoArco (elem v, w);
		vector<elem> getVecinos (elem v);
		vector<elem> getSucesores (elem v);
		vector<elem> getPredecesores (elem v);
		bool esCompleto();
		bool esConexo();
		bool esNulo();
		int gradoVertice(elem info);
		void vaciar();
		void copiar(grafo<elem>* target); 
};

grafo (){
	primero = 0;
	nVertices = 0;
	nArcos = 0;
}

grafo (nodoVertice<elem>* prim){
	primero = prim;
	nodoVertice<elem> *auxVertices = prim;
	nodoVertice<elem> *auxArcos = auxVertices.getArcos();
	nArcos = 0;
	nVertices = 0;
	while (auxVertices != 0) 
	{
		nVertices += 1
		while (auxArcos != 0)
		{
			nArcos += 1;
			auxArcos = auxArcos.getSiguiente();
		}
		auxVertices = auxVertices.getProx();
	}
}

grafo (nodoVertice<elem>* prim, int nroVertices, int nroArcos){
	primero = prim;
	nVertices = nroVertices;
	nArcos = nroArcos;
}

void setPrimero (nodoVertice<elem>* prim){
	primero = prim;
}

void setNroVertices(int numV){
	nVertices = numV;
}

void setNroArcos(int numA){
	nArcos = numA;
}

nodoVertice<elem>* getPrimero(){
	return (primero);
}

int getNroVertices(){
	return (nVertices);
}

int getNroArcos(){
	return (nArcos);
}

vector<elem> getVertices(){
	nodoVertice<elem>* auxVertice = prim;
	vector<nodoVertice<elem>*> vertices;
	while (auxVertice != 0){
		nVertices += 1;
		vertices.push_back(auxVertice);
		auxVertices = auxVertices.getProx();
	}
}

void insertarVertice (elem info){
	nodoVertice<elem>* ptrVertice = prim;
	if (this.esNulo()){
		this.setPrimero(new nodoVertice<elem>(info))
	}else{
		while (ptrVertice->getProx()!=0) {
			ptrVertice = ptrVertice->getProx();
		}
		ptrVertice->setProx(new nodoVertice<elem>(info))
	}
	

}

#endif