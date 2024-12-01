#ifndef arbol_bin
#define arbol_bin
#include <list>
#include <queue>
#include "nodoArbolBin.hpp"

using namespace std;

template <class tipo>
class ArbolBin {
private:
	//Atributos
	int longitud;
	NodoBin<tipo>* raiz;
	/*//Metodos
	void INORDEN(NodoBin<tipo>* Nodo, list<tipo>& L);
	void PREORDEN(NodoBin<tipo>* Nodo, list<tipo>& L);
	void POSTORDEN(NodoBin<tipo>* Nodo, list<tipo>& L);
	void INSERT(tipo& elem, NodoBin<tipo>* p);
	bool ESTA(tipo& elem, NodoBin<tipo>* p, bool& enc);
	NodoBin<tipo> BUSCAR_NODO(NodoBin<tipo>** padre, NodoBin<tipo>* ptr, tipo elem);
	void ELIMINAR(tipo elem);*/
public:
	ArbolBin();
	ArbolBin(tipo elem);
	ArbolBin(tipo elem, ArbolBin<tipo> aI, ArbolBin<tipo> aD);
	~ArbolBin();
};
		/*
		bool esVacio();
		tipo getRaiz();
		ArbolBin<tipo> hijoIzq();
		ArbolBin<tipo> hijoDer();
		list<tipo> inOrden();
		list<tipo> preOrden();
		list<tipo> postOrden();
		bool esIsomorfo(ArbolBin<tipo> a1, ArbolBin<tipo> a2);
		void insert(tipo elem);
		bool esta (tipo elem); //solo arboles ordenados
		void eliminar (tipo elem);
		list<tipo> recorridoLVL();
		int altura();
		list<tipo> hojas();
		list<tipo> caminoEntreNodos(tipo nodoI, tipo nodoF);
		tipo ancestro(tipo nodo1, tipo nodo2);
		list<tipo> diametro();
		
};

template<class tipo>
void ArbolBin<tipo> ::INORDEN(NodoBin<tipo>* Nodo, list<tipo>& L) {
	INORDEN(Nodo->getHijoIzq(), L);
	L.push_back(Nodo->getElem());
	INORDEN(Nodo->getHijoDer(), L);
}

template<class tipo>
void ArbolBin<tipo> ::PREORDEN(NodoBin<tipo>* Nodo, list<tipo>& L) {
	L.push_back(Nodo->getElem());
	PREORDEN(Nodo->getHijoIzq(), L);
	PREORDEN(Nodo->getHijoDer(), L);
}

template<class tipo>
void ArbolBin<tipo> ::POSTORDEN(NodoBin<tipo>* Nodo, list<tipo>& L) {
	POSTORDEN(Nodo->getHijoIzq(), L);
	POSTORDEN(Nodo->getHijoDer(), L);
	L.push_back(Nodo->getElem());
}

template <class tipo>
void ArbolBin<tipo> ::INSERT(tipo& elem, NodoBin<tipo>* p) {
	if (p != 0) {
		if (p->getElem() > elem) {
			if (p->getHijoIzq() == 0) {
				p->setHijoIzq(new NodoBin<tipo>(elem));
			}
			else {
				INSERT(elem, p->getHijoIzq());
			}
		}
		else {
			if (p->getHijoDer() == 0) {
				p->setHijoDer(new NodoBin<tipo>(elem));
			}
			else {
				INSERT(elem, p->getHijoDer());
			}
		}
	}
}

template<class tipo>
bool ArbolBin<tipo>::ESTA(tipo& elem, NodoBin<tipo>* p, bool& enc) {
	if (p != 0) {
		if (p->getElem() == elem) {
			return enc;
		}
		else if (p->getElem() < elem) {
			return ESTA(elem, p->getHijoDer(), enc);
		}
		else if (p->getElem() > elem) {
			return ESTA(elem, p->getHijoIzq(), enc);
		}
	}
}

template<class tipo>
NodoBin<tipo> ArbolBin<tipo>::BUSCAR_NODO(NodoBin<tipo>** padre, NodoBin<tipo>* ptr, tipo elem) {
	if (ptr != 0) {
		if (ptr->getElem() == elem) {
			return ptr;
		}
		else {
			*padre = ptr;
			if (ptr->getElem() > elem) {
				return BUSCAR_NODO(padre, ptr->getHijoIzq(), elem);
			}
			else {
				return BUSCAR_NODO(padre, ptr->getHijoDer(), elem);
			}
		}
	}
	*padre = 0;
	return 0;
}

*/
template <class tipo>
ArbolBin<tipo> :: ArbolBin() {
	longitud = 0;
	raiz = 0;
}

template<class tipo>
ArbolBin<tipo> :: ArbolBin(tipo elem) {
	longitud = 1;
	raiz = new NodoBin<tipo>(elem);
}

template<class tipo>
ArbolBin<tipo> :: ArbolBin(tipo elem, ArbolBin<tipo> aI, ArbolBin<tipo> aD) {
	longitud = 1;
	raiz = new NodoBin<tipo>(elem);
	raiz->setHijoDer(aD.getRaiz());
	raiz->setHijoIzq(aI.getRaiz());
}
/*
template<class tipo>
bool ArbolBin<tipo> ::esVacio() {
	return (raiz == 0);
}

template<class tipo>
tipo ArbolBin<tipo> ::getRaiz() {
	return (raiz->getElem());
}

template<class tipo>
ArbolBin<tipo> ArbolBin<tipo> ::hijoIzq() {
	return (raiz->getHijoIzq());
}

template<class tipo>
ArbolBin<tipo> ArbolBin<tipo> ::hijoDer() {
	return (raiz->getHijoDer());
}

template<class tipo>
list<tipo> ArbolBin<tipo> ::inOrden() {


	list<tipo> L;
	INORDEN(this->raiz, L);
	return (L);
}

template<class tipo>
list<tipo> ArbolBin<tipo> ::preOrden() {
	list<tipo> L;
	PREORDEN(this->raiz, L);
	return (L);
}

template<class tipo>
list<tipo> ArbolBin<tipo> ::postOrden() {
	list<tipo> L;
	POSTORDEN(this->raiz, L);
	return (L);
}

template<class tipo>
bool ArbolBin<tipo>::esIsomorfo(ArbolBin<tipo> a1, ArbolBin<tipo> a2)
{
	
}

template <class tipo>
void ArbolBin<tipo> ::insert(tipo elem) {
	if (this->raiz == 0) {
		raiz = new NodoBin<tipo>(elem);
	}
	else {
		INSERT(elem, this->raiz);
	}
}

template<class tipo>
bool ArbolBin<tipo>::esta(tipo elem)
{
	bool encontrado = 0;
	return ESTA(elem, this->raiz, encontrado);
}

template<class tipo>
void ArbolBin<tipo>::eliminar(tipo elem)
{
	NodoBin<tipo>* encontrado;
	NodoBin<tipo>* padre = this->raiz;

	encontrado = BUSCAR_NODO(&padre, this->raiz, elem);
	ELIMINAR(encontrado);
}

template<class tipo>
 list<tipo> ArbolBin<tipo>::recorridoLVL()
{

	return list<tipo>();
}

#endif

 template<class tipo>
 inline void ArbolBin<tipo>::ELIMINAR(tipo elem)
 {
 }

 template<class tipo>
 inline int ArbolBin<tipo>::altura()
 {
	 return 0;
 }

 template<class tipo>
 inline list<tipo> ArbolBin<tipo>::hojas()
 {
	 return list<tipo>();
 }

 template<class tipo>
 inline list<tipo> ArbolBin<tipo>::caminoEntreNodos(tipo nodoI, tipo nodoF)
 {
	 return list<tipo>();
 }

 template<class tipo>
 inline tipo ArbolBin<tipo>::ancestro(tipo nodo1, tipo nodo2)
 {
	 return tipo();
 }

 template<class tipo>
 inline list<tipo> ArbolBin<tipo>::diametro()
 {
	 return list<tipo>();
 }
 */
 template<class tipo>
ArbolBin<tipo>::~ArbolBin()
 {

 }
 
 #endif