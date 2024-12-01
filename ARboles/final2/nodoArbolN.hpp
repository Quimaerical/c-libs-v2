#ifndef nodo_arbol_n_h
#define nodo_arbol_n_h

template<class tipo>
class NodoN{
	private:
		tipo elem;
		NodoN<tipo>* hijo;
		NodoN<tipo>* hermano;
	public:
		NodoN();
		NodoN(tipo _elem);
		NodoN(tipo _elem, NodoN<tipo>* _hijo);
		NodoN(tipo _elem, NodoN<tipo>* _hijo, NodoN<tipo>* _hermano);
		void setElem(tipo _elem);
		void setHijo(NodoN<tipo>* _hijo);
		void setHermano(NodoN<tipo>* _hermano);
		tipo getElem();
		NodoN<tipo>* getHijo();
		NodoN<tipo>* getHermano();
};

template<class tipo>
NodoN<tipo> :: NodoN(){
	elem = 0;
	hijo = 0;
	hermano = 0;
}

template<class tipo>
NodoN<tipo> :: NodoN(tipo _elem){
	elem = _elem;
	hijo = 0;
	hermano = 0;
}

template<class tipo>
NodoN<tipo> :: NodoN(tipo _elem, NodoN<tipo>* _hijo){
	elem = _elem;
	hijo = _hijo;
	hermano = 0;
}

template<class tipo>
NodoN<tipo> :: NodoN(tipo _elem, NodoN<tipo>* _hijo, NodoN<tipo>* _hermano){
	elem = _elem;
	hijo = _hijo;
	hermano = _hermano;
}

template<class tipo>
void NodoN<tipo> :: setElem(tipo _elem){
	elem = _elem;
}

template<class tipo>
void NodoN<tipo> :: setHijo(NodoN<tipo>* _hijo){
	hijo = _hijo;
}

template<class tipo>
void NodoN<tipo> :: setHermano(NodoN<tipo>* _hermano){
	hermano = _hermano;
}

template<class tipo> 
tipo NodoN<tipo> :: getElem(){
	if(this!=0){
		return elem;	
	}
	return 0;
}
	
template<class tipo>
NodoN<tipo>* NodoN<tipo> :: getHijo(){
	if(this != 0){
		return hijo;
	}
	return 0;
}

template<class tipo>
NodoN<tipo>* NodoN<tipo> :: getHermano(){
	if (this != 0){
		return hermano;
	}
	return 0;
}
	
#endif