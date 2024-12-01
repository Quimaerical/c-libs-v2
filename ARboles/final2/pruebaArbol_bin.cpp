#include <iostream>
#include <list>
#include "nodoArbolBin.hpp"
#include "arbolBin.hpp"

using namespace std;

int main(){
	NodoBin<int> a = NodoBin<int>(1);
	NodoBin<int> b = NodoBin<int>(2);
	NodoBin<int> c = NodoBin<int>(3,&a,&b);
	
	a.setElem(-1);
	
	a.setHijoDer(&c);
	
	cout << a.getElem() << c.getHijoDer()->getElem() << c.getElem() << a.getHijoDer()->getElem() << endl;
	
	//ArbolBin<int> arbol = ArbolBin<int>(0); 
	
	/*ArbolBin<int> izqhijo1 = ArbolBin<int>(3);
	ArbolBin<int> izqhijo2= ArbolBin<int>(4);
	
	ArbolBin<int> derhijo1 = ArbolBin<int>(5);
	ArbolBin<int> derhijo2= ArbolBin<int>(6);
	
	ArbolBin<int> izq1 = ArbolBin<int>(1, &izqhijo1, &izqhijo2);
	ArbolBin<int> der1 = ArbolBin<int>(2, &derhijo1, &derhijo2);

	ArbolBin<int> arbol = ArbolBin<int>(0, &izq1, &der1);*/
	
	
	ArbolBin<int> arbol = ArbolBin<int>(2);
	arbol.insertar(-1);
	arbol.insertar(-3);
	arbol.insertar(0);
	arbol.insertar(3);
	arbol.insertar(4);
	arbol.insertar(5);
	arbol.insertar(1); 
 
	cout << "peso: " << arbol.getPeso() << "\n";
	
	cout << "altura: " << arbol.altura() << endl; 
	
	vector<int>in = arbol.inOrden();
	cout << "in: "; vector<int> inor = in;
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	
	
	cout << "pre: ";
	in = arbol.preOrden(); 	vector<int> pre = in;
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}

	cout << endl;
	
	ArbolBin<int> arbolConstr = ArbolBin<int>();
	arbolConstr.crearEnPre(inor, pre); 

	in = arbol.postOrden();
	cout << "post: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
		
	in = arbol.porNiveles();
	cout << "niveles: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	in = arbol.nivel(3,3);
	cout << "nivel 3: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	NodoBin<int>* g = arbol.buscar(47);
	cout << "nodo: "<< g->getElem() << endl;
	
	ArbolBin<int> sub = arbol.subarbol(1);

	in = sub.preOrden();	
	cout << "pre sub: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	in = arbol.caminoRaizNodo(-3); 
	cout << "Raiz a 3: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	in = arbol.camino(0,3); 
	cout << "nodonodo: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	in = arbol.hojas();  
	cout << "Hojas: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	in = arbol.diametro();
	cout << "diametro: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	ArbolBin<int> arbol2;
	arbol2.crearEnPre(inor, pre);
	in = arbol2.preOrden();
	cout << "pre2: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	in = arbol2.inOrden();
	cout << "in2: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	in = arbol2.postOrden();
	cout << "post2: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	ArbolBin<int> arbol3;
	arbol3 = arbol.subarbol(0);
	in = arbol3.preOrden();
	cout << "pre subarbol: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	arbol.insertarSubarbol(arbol3);
	in = arbol.preOrden();
	cout << "pre arbol original + subarbol: ";
	while(!in.empty()){
		cout << in.front() << " ";
		in.erase(in.begin());
	}
	cout << endl;
	
	 
	return 0;
}