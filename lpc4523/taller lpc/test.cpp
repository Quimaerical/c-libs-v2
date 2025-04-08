#include <iostream>
#include "list.h"
#include "pila.h"
#include "cola.h"

using namespace std;

int main() {
	lista<lista<int>*> a;
	a.insertar(new lista<int>, 1);
	lista<int>* ptr = a.consultar(1);
	ptr->insertar(2, 1);
	ptr->insertar(1, 2); 
	cout << ptr->consultar(1) << ' ';
	cout << ptr->consultar(2) << endl;
	a.eliminar(1);
	
	
	pila<pila<int>*> b;
	b.apilar(new pila<int>);
	pila<int>* ptrP = b.tope();
	ptrP->apilar(2);
	ptrP->apilar(1);
	cout << ptrP->tope() << ' ';
	ptrP->desapilar();
	cout << ptrP->tope() << endl;
	ptrP->desapilar();
	
	cola<cola<int>*> c;
	c.encolar(new cola<int>);
	cola<int>* ptrC = c.frente();
	ptrC->encolar(2);
	ptrC->encolar(1);
	cout << ptrC->frente() << ' ';
	ptrC->desencolar();
	cout << ptrC->frente() << endl;
	ptrC->desencolar();
	
	return(0);
}