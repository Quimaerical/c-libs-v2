#ifndef nodo_arbol_bin_h
#define nodo_arbol_bin_h

template <class tipo>
class NodoBin{
	private:
		tipo elem;
		NodoBin<tipo>* hijoIzq;
		NodoBin<tipo>* hijoDer;
	public:
		NodoBin(tipo _elem);
		NodoBin(tipo _elem, NodoBin<tipo>* _izq, NodoBin<tipo>* _der);
		void setElem(tipo _elem);
		void setHijoIzq(NodoBin<tipo>* _izq);
		void setHijoDer(NodoBin<tipo>* _der);
		void setHijos(NodoBin<tipo>* _izq, NodoBin<tipo>* _der);
		tipo getElem();
		NodoBin<tipo>* getHijoIzq();
		NodoBin<tipo>* getHijoDer();
};

template <class tipo>
NodoBin<tipo> :: NodoBin(tipo _elem) {
	elem = _elem;
	hijoIzq = 0;
	hijoDer = 0;
}

template <class tipo>
NodoBin<tipo> :: NodoBin(tipo _elem, NodoBin<tipo>* _izq, NodoBin<tipo>* _der){
	elem = _elem;
	hijoIzq = _izq;
	hijoDer = _der;
}

template <class tipo>
void NodoBin<tipo> :: setElem(tipo _elem){
	elem = _elem;
}

template <class tipo>
void NodoBin<tipo> :: setHijoIzq(NodoBin<tipo>* _izq){
	hijoIzq = _izq;
}

template <class tipo>
void NodoBin<tipo> :: setHijoDer(NodoBin<tipo>* _der){
	hijoDer = _der;
}

template <class tipo>
void NodoBin<tipo> :: setHijos(NodoBin<tipo>* _izq, NodoBin<tipo>* _der){
	hijoIzq = _izq;
	hijoDer = _der;
}

template <class tipo>
tipo NodoBin<tipo> :: getElem(){
	if (this!=0){
		return elem;
	}
	return 0;

}

template <class tipo>
NodoBin<tipo>* NodoBin<tipo> :: getHijoIzq(){
	if (this!=0){
		return hijoIzq;
	}
	return 0;
}

template <class tipo>
NodoBin<tipo>* NodoBin<tipo> :: getHijoDer(){
	if (this!=0){
		return hijoDer;
	}
	return 0;
}

#endif