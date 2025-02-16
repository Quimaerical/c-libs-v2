#ifndef list_
#define list_
#include "nodo.h"

template<class elem>
class lista {
protected:
	nodo<elem>* prim;
	nodo<elem>* ult;
	int longi;
public:
	lista();
	~lista();
	int longitud();
	nodo<elem>* getPrim();
	nodo<elem>* getUlt();
	void setPrim(nodo<elem>* primero);
	void setUlt(nodo<elem>* ultimo);
	void setLongit(int l);
	void insertar(elem dato, int pos);
	void modificar(elem dato, int pos);
	void eliminar(int pos);
	void invertir();
	void ordenar();
	lista<elem> copia();
	elem consultar(int pos);
	bool esVacia();
	void concatenar(lista<elem> b);
	elem operator [] (int pos);
};

template<class elem>
lista<elem>::lista() {
	prim = 0;
	ult = 0;
	longi = 0;
}

template<class elem>
lista<elem>::~lista() {
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
int lista<elem>::longitud() {
	return(longi);
}

template<class elem>
nodo<elem>* lista<elem>::getPrim() {
	if (prim!=nullptr) return(prim); else return(0);
}

template<class elem>
nodo<elem>* lista<elem>::getUlt() {
	if (ult!=nullptr) return(ult); else return (0);
}

template<class elem>
void lista<elem>::setPrim(nodo<elem>* primero) {
	prim = primero;
}

template<class elem>
void lista<elem>::setUlt(nodo<elem>* ultimo) {
	ult = ultimo;
}

template<class elem>
void lista<elem>::setLongit(int l) {
	longi = l;
}

template<class elem>
void lista<elem>::insertar(elem dato, int pos) {
	nodo<elem>* ptrNodo = new nodo<elem>(0,0,dato);
	nodo<elem>* aux1 = prim;
	nodo<elem>* aux2 = 0;

	if (longi == 0) {
		prim = ptrNodo;
		ult = ptrNodo;
	}
	else {
		if (pos <= 1) {
			ptrNodo->setProx(prim);
			prim = ptrNodo;
		}
		else if (pos > longi) {
			if (ult!=nullptr){
				ult->setProx(ptrNodo);
				ptrNodo->setPrev(ult);
			}
			ult = ptrNodo;
		}
		else {
			aux2 = aux1->getProx();
			while (--pos > 1) {
				aux1 = aux2;
				aux2 = aux2->getProx();
			}

			ptrNodo->setProx(aux2);
			ptrNodo->setPrev(aux2->getPrev());
			aux2->setPrev(ptrNodo);
			aux1->setPrev(ptrNodo);
		}
	}
	longi++;
}

template<class elem>
void lista<elem>::modificar(elem dato, int pos) {
	nodo<elem>* ptrNodo;
	int i;
	if (pos <= 1) {
		prim->setDato(dato);
	}
	else if(pos >= longi) {
		ult->setDato(dato);
	}
	else {
		if (pos >= longi / 2) {
			ptrNodo = ult;
			i = longi + 1;
			while (i > pos) {
				ptrNodo = ptrNodo->getPrev();
				i--;
			}
		}
		else {
			ptrNodo = prim;
			i = 0;
			while (i < pos) {
				ptrNodo = ptrNodo->getProx();
				i++;
			}
		}
		ptrNodo->setDato(dato);
	}
}

template<class elem>
void lista<elem>::eliminar(int pos) {
	nodo<elem>* aux1 = prim;
	nodo<elem>* aux2 = 0;
	if (prim == nullptr || pos < 1 || pos > longi) return;
	if (longi > 0) {
		if (longi == 1) {
			delete prim;
			prim = 0;
			ult = 0;
		}
		else if (pos <= 1) {
			prim = prim->getProx();
			delete aux1;
		}
		else {
			aux2 = aux1->getProx();
			while (--pos > 1 && aux2->getProx() != 0) {
				aux1 = aux2;
				aux2 = aux2->getProx();
			}
			if (aux2 == ult) {
				ult = aux1;
			}
			aux1->setProx(aux2->getProx());
			delete aux2;
			aux2 = aux1->getProx();
			aux2->setPrev(aux1);
		}
		longi--;
	}
}


template<class elem>
void lista<elem>::invertir() {
	nodo<elem>* aux;
	aux = this->ult;
	ult = prim;
	prim = aux;
}

template<class elem>
void lista<elem>::ordenar() {
	nodo<elem>* aux1;
	nodo<elem>* aux2;
	nodo<elem>* aux3;
	int l = this->longi, cont = 0, cont1 = 0;

	if (this->prim == nullptr || this->ult == nullptr) return;
	this->ult->setProx(this->prim);
	this->prim = this->prim->getProx();
	aux3 = this->ult->getProx();
	aux3->setProx(0);

	while (l-- > 1) {
		cont++;
		if (this->prim->getDato() < this->ult->getProx()->getDato()) {
			if (l > 1) {
				aux1 = this->prim;
				this->prim = this->prim->getProx();
				aux1->setProx(this->ult->getProx());
				this->ult->setProx(aux1);
			}
		}
		else {
			aux1 = this->ult->getProx();
			aux2 = aux1->getProx();
			while (aux2 != 0 && this->prim->getDato() > aux2->getDato()) {
				aux1 = aux2;
				aux2 = aux2->getProx();
				cont1++;
				if (cont1 > 0) {
					cont1--;
					cont++;
				}
			}
			aux1->setProx(this->prim);
			this->prim = this->prim->getProx();
			aux1->getProx()->setProx(aux2);
			if (aux2 == 0) {
				aux3 = aux1->getProx();
			}
		}
	}
	this->ult = aux3;
}

template<class elem>
lista<elem> lista<elem>::copia() {
	lista<elem> L = lista<elem>();
	nodo<elem>* ptrNodo = this->prim;
	while (ptrNodo != 0) {
		L.insertar(ptrNodo->getDato(), L.longitud() + 1);
		ptrNodo = ptrNodo->getProx();
	}
	return L;
}

template <class elem>
elem lista<elem>::consultar(int pos) {
	nodo<elem>* ptrNodo = prim;

	if (pos <= 1) {
		return prim->getDato();
	}
	else if (pos >= longi) {
		return ult->getDato();
	}
	while (--pos > 0) {
		ptrNodo = ptrNodo->getProx();
	}
	return ptrNodo->getDato();
}

template <class elem>
bool lista<elem>::esVacia() {
	return (longi == 0);
}

template <class elem>
void lista<elem>::concatenar (lista<elem> b){
	this->ult->setProx(b.prim);
	this->ult = b.ult;
}

template <class elem>
elem lista<elem>::operator [] (int pos) {
	int i;
	nodo<elem>* ptrNodo;
	if (pos >= this->longi) {
		ptrNodo = this->ult;
	}
	else if (pos <= 1) {
		ptrNodo = this->prim;
	}
	else if (pos > 1 && pos < this->longi) {
		i = 2;
		ptrNodo = this->prim->getProx();
		while (i < pos){
			ptrNodo = ptrNodo->getProx();
			i++;
		}
	}
	return (ptrNodo->getDato());
}

#endif