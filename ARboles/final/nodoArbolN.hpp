#ifndef nodo_arbol_n
#define nodo_arbol_n

template <class tipo>
class NodoN{
	private:
		tipo elem;
		NodoN<tipo>* hijo;
		NodoN<tipo>* hermano;
	public:
		NodoN<tipo> (tipo _elem);
		NodoN<tipo> (tipo _elem, NodoN<tipo>* _hijo, NodoN<tipo> _hermano);
		void setElem (tipo _elem);
		void setHijo (NodoN<tipo>* _hijo);
		void setHermano (NodoN<tipo>* _hermano);
		tipo getElem ();
		NodoN<tipo>* getHijo();
		NodoN<tipo>* getHermano();
};

template <class tipo>
NodoN<tipo> :: NodoN<tipo>(tipo _elem){
	elem = _elem;
	hijo = 0;
	hermano = 0;
}

template <class tipo>
NodoN<tipo> :: NodoN<tipo>(tipo _elem, NodoN<tipo>* _hijo, NodoN<tipo> _hermano){
	elem = _elem;
	hijo = _hijo;
	hermano = _hermano;
}

template <class tipo>
void NodoN<tipo> :: setElem (tipo _elem){
	elem = _elem;
}


template <class tipo>
void NodoN<tipo> :: setHijo (NodoN<tipo>* _hijo){
	hijo = _hijo;
} 

template <class tipo>
void NodoN<tipo> :: setHermano (NodoN<tipo>* _hermano){
	hermano = _hermano;
}

template <class tipo>
tipo NodoN<tipo> :: getElem (){
	return elem;
}

template <class tipo>
NodoN<tipo>* NodoN<tipo> :: getHijo (){
	return hijo;
}

template <class tipo>
NodoN<tipo>* NodoN<tipo> :: getHermano (){
	return hermano;
}

#endif