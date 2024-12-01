#include <iostream>
#include "Arbol_bin.hpp"

using namespace std;

int main(){
	list<int> l;
	list<int> pre;
	list<int> in;
	list<int> post;
	Arbol_bin<int> a = Arbol_bin<int>();

	
	a.insert(10);
	a.insert(5);
	a.insert(15);
	a.insert(3);
	a.insert(6);
	a.insert(12);
	a.insert(20);
	a.insert(27);
	a.insert(23);
	

	cout << "Altura del arbol: " << a.altura() << endl;
	a.eliminar(10);
	
	
	bool ban = a.esta(11);

	cout << "Arbol vacio: " <<  ban << endl << endl;
	
	l =  a.pre_orden();
	pre = a.pre_orden();
	cout << "Pre orden: ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	l =  a.in_orden();
	in = a.in_orden();
	cout << "In orden: ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	l =  a.post_orden();
	post = a.post_orden();	
	cout << "Post orden: ";
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	l = a.diametro(),
	cout << "diametro: ";
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl << endl;

	
	Arbol_bin<int> b = Arbol_bin<int>();
	b.crear_in_pre_2(in, pre);
	
	
	l = b.pre_orden();
	cout << "Pre orden: ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	l = b.in_orden();
	cout << "In orden: ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	
	l = b.post_orden();
	cout << "Post orden: ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl << endl;
	

	l = b.recorrido_lv();
	cout << "Recorrido por nivel: ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;

	cout << "Altura del arbol: " << b.altura() << endl;
	l = b.hojas();
	cout << "Hojas: ";
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	l = b.camino_nod(27);
	cout << "camino al nodo: ";
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	l = b.camino_nod_nod(5, 20);
	cout << "camino de nodo a nodo: ";
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl;
	
	cout << "ancestro comun: " << b.ancestro(5, 20) << endl;
	
	l = b.diametro(),
	cout << "diametro: ";
	while(!l.empty()){
		cout << l.front() << " ";
		l.erase(l.begin());
	}
	cout << endl << endl;
	
	l = b.recorrido_lv_exacto(-1);
	cout << "Recorrido por niveles (-1): ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
	
	l = b.recorrido_lv_exacto(0);
	cout << "Recorrido por niveles (0): ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
	
	l = b.recorrido_lv_exacto(1);
	cout << "Recorrido por niveles (1): ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
	
	l = b.recorrido_lv_exacto(2);
	cout << "Recorrido por niveles (2): ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
	
	l = b.recorrido_lv_exacto(3);
	cout << "Recorrido por niveles (3): ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
	
	l = b.recorrido_lv_exacto(4);
	cout << "Recorrido por niveles (4): ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
	
	l = b.recorrido_lv_exacto(5);
	cout << "Recorrido por niveles (5): ";	
	while(!l.empty()){
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
	
	
	return 0;
}