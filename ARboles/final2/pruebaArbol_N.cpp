#include <iostream>
#include <list>
#include "nodoArbolN.hpp"
#include "arbolN.hpp"

using namespace std;

int main(){
	NodoN<int> a = NodoN<int>(1);
	NodoN<int> b = NodoN<int>(2);
	NodoN<int> c = NodoN<int>(3,&a,&b);
	
	a.setElem(-1);
	
	a.setHermano(&c);
	
	/*ArbolN<int> arbol = ArbolN<int>(0); 
	
	ArbolN<int> izqhijo1 = ArbolN<int>(3);
	ArbolN<int> izqhijo2= ArbolN<int>(4);
	
	ArbolN<int> derhijo1 = ArbolN<int>(5);
	ArbolN<int> derhijo2= ArbolN<int>(6);
	
	ArbolN<int> izq1 = ArbolN<int>(1, izqhijo1.getRaiz(), izqhijo2.getRaiz() );
	ArbolN<int> der1 = ArbolN<int>(2, derhijo1.getRaiz(), derhijo2.getRaiz() );

	ArbolN<int> arbol2 = ArbolN<int>(0, izq1.getRaiz() , der1.getRaiz() );
	
	cout << a.getElem() << c.getHijo()->getElem() << c.getElem() << a.getHermano()->getElem() << endl;*/
	
	ArbolN<int> arbol = ArbolN<int>(2);
	arbol.insertar(-1);
	arbol.insertar(-3);
	arbol.insertar(0);
	arbol.insertar(3);
	arbol.insertar(4);
	arbol.insertar(5);
	arbol.insertar(1); 
}