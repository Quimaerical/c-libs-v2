
#include <vector>
#include <list>
#include <queue>
#include "nodoArbolBin.hpp"

#ifndef arbol_bin
#define arbol_bin

    ////////////////////////////////
   // Autor: Sebastian Piñango	 //
  //  v 2.0						//	
 ////////////////////////////////

using namespace std;

template <class tipo>
class ArbolBin {
private:
	//ATRIBUTOS
	int peso;
	NodoBin<tipo>* raiz;
	//Metodos Privados
	void ELIMINAR_ARBOL(NodoBin<tipo>* Nodo);
	void INORDEN(NodoBin<tipo>* Nodo, vector<tipo>& L);
	void PREORDEN(NodoBin<tipo>* Nodo, vector<tipo>& L);
	void POSTORDEN(NodoBin<tipo>* Nodo, vector<tipo>& L);
   void NIVELES(NodoBin<tipo>* Nodo, vector<tipo>& L, queue<NodoBin<tipo>*>& cola);
   void BUSCAR_NIVEL(NodoBin<tipo>* Nodo, vector<tipo>& L, queue<NodoBin<tipo>*>& cola, int nivelInic, int nivelFin);
   NodoBin<tipo>* COPIAR_NODOS(NodoBin<tipo>* ptrN);
   NodoBin<tipo>* BUSCAR_NODO(NodoBin<tipo>* nod, tipo* elem);
   void INSERTAR(NodoBin<tipo>* nod, tipo* elem);
   void INSERTAR_SUBARBOL(NodoBin<tipo>* nod, ArbolBin<tipo>& subarbol);
   NodoBin<tipo>* LEER_IN_PRE(vector<tipo>& inOrden, vector<tipo>& preOrden);
   NodoBin<tipo>* BUILD_FROM_POST_IN_RECURSIVE(list<tipo>& postOrderList, list<tipo>& inOrderList);
   void CAMINO_RAIZ_NODO(NodoBin<tipo>* raiz, vector<tipo>& L, tipo& nodo, bool& encontrado);
   void DIAMETRO(NodoBin<tipo>* raiz, vector<tipo>& L, vector<tipo>& cam1, vector<tipo>& cam2, vector<tipo>& diametro);
   void FUNCION_MAGICA(vector<tipo>& L, vector<tipo>& cam1, vector<tipo>& cam2, vector<tipo>& diametro);
   vector<tipo> INTERSECCION(vector<tipo> L1, vector<tipo> L2);
   int ALTURA(NodoBin<tipo>* p);
   void HOJAS(NodoBin<tipo>* p, vector<tipo>& vHojas);
   ArbolBin(tipo elem, NodoBin<tipo>* aI, NodoBin<tipo>* aD);
public:
	ArbolBin();
	ArbolBin(tipo elem);
	ArbolBin(tipo elem, ArbolBin<tipo>* aI, ArbolBin<tipo>* aD);
	ArbolBin(NodoBin<tipo>& raiz);
	~ArbolBin();
	void crearEnPre(vector<tipo> inOrden, vector<tipo> preOrden);
	void crearEnPost(vector<tipo> postOrden, vector<tipo> inOrden);
	NodoBin<tipo>* getRaiz();
	int getPeso();
	vector<ArbolBin<tipo> > hijos();
	//lecturas de arbol/////////////////////////
	vector<tipo> inOrden();                  //
	vector<tipo> preOrden();                //
	vector<tipo> postOrden();              //
	vector<tipo> porNiveles();            //
	///////////////////////////////////////
	vector<tipo> nivel(int nivInicial, int nivFinal);// lista de uno o mas niveles definidos 
	NodoBin<tipo>* buscarNodo(tipo elem);
	ArbolBin<tipo> subarbol(tipo nwRaiz);//devuelve un subarbol insertando un nodo que hara de raiz
	void insertarSubarbol(ArbolBin<tipo>& subArbol);///////////////////////////////////////////////
	vector<tipo> caminoRaizNodo(tipo N);
	vector<tipo> camino(tipo N1, tipo N2);
	vector<tipo> diametro();
	void insertar(tipo elem);
	NodoBin<tipo>* buscar(tipo elem);
	int altura();
	vector<tipo> hojas(); //devuelve una lista de todas las hojas del arbol//
};

template <class tipo>
ArbolBin<tipo> :: ArbolBin() {
	peso = 0;
	raiz = 0;
}

template<class tipo>
ArbolBin<tipo> :: ArbolBin(tipo elem) {
	peso = 1;
	raiz = new NodoBin<tipo>(elem);
}

template<class tipo>
ArbolBin<tipo> :: ArbolBin(tipo elem, ArbolBin<tipo>* aI, ArbolBin<tipo>* aD) {
	peso = 1;
	raiz = new NodoBin<tipo>(elem);
	raiz->setHijoDer(COPIAR_NODOS(aD->getRaiz()));
	raiz->setHijoIzq(COPIAR_NODOS(aI->getRaiz()));
}

template<class tipo>
ArbolBin<tipo> :: ArbolBin(tipo elem, NodoBin<tipo>* aI, NodoBin<tipo>* aD) {
	peso = 1;
	raiz = new NodoBin<tipo>(elem);
	raiz->setHijoDer(aD);
	raiz->setHijoIzq(aI);
}

template<class tipo>
ArbolBin<tipo> :: ArbolBin(NodoBin<tipo>& raiz){
	this->raiz = &raiz; 
}

template<class tipo>
void ArbolBin<tipo> :: ELIMINAR_ARBOL(NodoBin<tipo>* Nodo){
	if (Nodo != 0){
		ELIMINAR_ARBOL(Nodo->getHijoIzq());
		ELIMINAR_ARBOL(Nodo->getHijoDer());
		Nodo->~NodoBin<tipo>();
	}
}

template<class tipo>
ArbolBin<tipo> :: ~ArbolBin(){
	ELIMINAR_ARBOL(this->raiz);
	peso = 0;
}

template <class tipo>
int ArbolBin<tipo>::COUNT_NODES(const NodoBin<tipo>* node) const {
    if (node == NULL) return 0;
    return 1 + COUNT_NODES(node->getLeft()) + COUNT_NODES(node->getRight());
}

template <class tipo>
NodoBin<tipo>* ArbolBin<tipo> :: LEER_IN_PRE(vector<tipo>& inOrden, vector<tipo>& preOrden){
	tipo elem;
	vector<tipo> inOrdenIzq; vector<tipo> preOrdenIzq;
	vector<tipo> inOrdenDer; vector<tipo> preOrdenDer;
	
	if(preOrden.size() == 0){
		return 0;
	}
	if(preOrden.size() == 1){
		peso++;
		return new NodoBin<tipo>(preOrden.front());
	}
	elem = preOrden.front();
	preOrden.erase(preOrden.begin());
	while(inOrden.front() != elem){
		inOrdenIzq.push_back(inOrden.front());
		preOrdenIzq.push_back(preOrden.front());
		inOrden.erase(inOrden.begin());
		preOrden.erase(preOrden.begin());
	}
	inOrden.erase(inOrden.begin());
	while(!preOrden.empty()){
		inOrdenDer.push_back(inOrden.front());
		preOrdenDer.push_back(preOrden.front());
		inOrden.erase(inOrden.begin());
		preOrden.erase(preOrden.begin());
	}	
	return new NodoBin<tipo>(elem, LEER_IN_PRE(inOrdenIzq, preOrdenIzq), LEER_IN_PRE(inOrdenDer, preOrdenDer));
}

template <class tipo>
NodoBin<tipo>* ArbolBin<tipo>::BUILD_FROM_POST_IN_RECURSIVE(list<tipo>& postOrderList, list<tipo>& inOrderList) {
    if (postOrderList.empty() || inOrderList.empty()) return NULL;
    tipo rootInfo = postOrderList.back();
    postOrderList.pop_back();
    NodoBin<tipo>* newNode = new NodoBin<tipo>(rootInfo);
    list<tipo> leftInOrder, rightInOrder, leftPostOrder, rightPostOrder;
    typename list<tipo>::iterator it = inOrderList.begin();
    while (it != inOrderList.end() && *it != rootInfo) leftInOrder.push_back(*it++);
    if (it != inOrderList.end()) it++;
    while (it != inOrderList.end()) rightInOrder.push_back(*it++);
    int leftSize = leftInOrder.size();
    typename list<tipo>::iterator postIt = postOrderList.begin();
    for(int i = 0; i < leftSize && postIt != postOrderList.end(); ++i) leftPostOrder.push_back(*postIt++);
    while(postIt != postOrderList.end()) rightPostOrder.push_back(*postIt++);
    newNode->setRight(BUILD_FROM_POST_IN_RECURSIVE(rightPostOrder, rightInOrder));
    newNode->setLeft(BUILD_FROM_POST_IN_RECURSIVE(leftPostOrder, leftInOrder));
    return newNode;
}

template<class tipo>
void ArbolBin<tipo> :: crearEnPre(vector<tipo> inOrden, vector<tipo> preOrden){
	this->raiz = LEER_IN_PRE(inOrden, preOrden);
}

template <class tipo>
void ArbolBin<tipo>::crearEnPost(vector<tipo> postOrderList, vector<tipo> inOrderList) {
      if (postOrderList.size() != inOrderList.size()) throw runtime_error("Mismatched list sizes in buildFromPostIn");
    this->raiz = BUILD_FROM_POST_IN_RECURSIVE(postOrderList, inOrderList);
    this->peso = COUNT_NODES(this->raiz);
}


template<class tipo>
vector<ArbolBin<tipo> > ArbolBin<tipo> :: hijos(){
	NodoBin<tipo>* aux;
	ArbolBin<tipo> arbolAux;
	vector<ArbolBin<tipo> > L;
	aux = raiz->getHijoIzq();
	if (aux != 0){
		arbolAux.raiz = aux;
		L.push_back(arbolAux);
		aux = raiz->getHijoDer;
	}
	if (aux != 0){
		arbolAux.raiz = aux;
		L.push_back(arbolAux);
	}
	return (L);
}

template<class tipo>
NodoBin<tipo>* ArbolBin<tipo> :: getRaiz(){
	return raiz;
}

template<class tipo>
int ArbolBin<tipo> :: getPeso(){
	return peso;
}

//lecturas de arbol
template<class tipo>
vector<tipo> ArbolBin<tipo> :: inOrden() {
	vector<tipo> L;          
	INORDEN(this->raiz, L);
	return (L);
}

template<class tipo>
vector<tipo> ArbolBin<tipo> :: preOrden() {
	vector<tipo> L;
	PREORDEN(this->raiz, L);
	return (L);
}

template<class tipo>
vector<tipo> ArbolBin<tipo> :: postOrden() {
	vector<tipo> L;
	POSTORDEN(this->raiz, L);
	return (L);
}

template <class tipo>
vector<tipo> ArbolBin<tipo> :: porNiveles(){
	vector<tipo> L;
	queue<NodoBin<tipo>*> C;
	C.push(0);
	NIVELES(this->raiz, L, C);
	return (L);
}

template<class tipo>
void ArbolBin<tipo> :: INORDEN(NodoBin<tipo>* Nodo, vector<tipo>& L) {
	if (Nodo != 0){

		INORDEN(Nodo->getHijoIzq(), L);

		L.push_back(Nodo->getElem());

		INORDEN(Nodo->getHijoDer(), L);
		
	}
}

template<class tipo>
void ArbolBin<tipo> :: PREORDEN(NodoBin<tipo>* Nodo, vector<tipo>& L) {
	if (Nodo != 0){
		
		L.push_back(Nodo->getElem());
	
		PREORDEN(Nodo->getHijoIzq(), L);
	
		PREORDEN(Nodo->getHijoDer(), L);
	
	}
}

template<class tipo>
void ArbolBin<tipo> :: POSTORDEN(NodoBin<tipo>* Nodo, vector<tipo>& L) {
	if (Nodo != 0){	
	
		POSTORDEN(Nodo->getHijoIzq(), L);
	
		POSTORDEN(Nodo->getHijoDer(), L);
	
		L.push_back(Nodo->getElem());
	
	}
}

template<class tipo>
void ArbolBin<tipo> :: NIVELES(NodoBin<tipo>* Nodo, vector<tipo>& L, queue<NodoBin<tipo>*>& cola){
	while(!cola.empty()){
		L.push_back(Nodo->getElem());
		if (Nodo->getHijoIzq() != 0){
			cola.push(Nodo->getHijoIzq());
		}
		if (Nodo->getHijoDer() != 0){
			cola.push(Nodo->getHijoDer());
		}
		cola.pop();
		Nodo = cola.front();
	}
}

template<class tipo>
vector<tipo> ArbolBin<tipo> :: nivel(int nivInicial, int nivFinal){
	vector<tipo> L;
	queue<NodoBin<tipo>*> C;
	BUSCAR_NIVEL(this->raiz, L, C, nivInicial, nivFinal);
	return L;
}

template<class tipo>
void ArbolBin<tipo> :: BUSCAR_NIVEL(NodoBin<tipo>* Nodo, vector<tipo>& L, queue<NodoBin<tipo>*>& cola, int nivelInic, int nivelFin){
	cola.push(Nodo);
	int cont = 1;
	cola.push(0);
	while (!cola.empty() && cont<=nivelFin){
		if (cola.front() == 0){
				cola.push(0);
				cont++;
		}else{
			if(cont >= nivelInic && cont <= nivelFin){
				L.push_back(Nodo->getElem());
			}
			if (Nodo->getHijoIzq() != 0){
				cola.push(Nodo->getHijoIzq());
			}
			if (Nodo->getHijoDer() != 0){
				cola.push(Nodo->getHijoDer());
			}
		}
		cola.pop();
		Nodo = cola.front();
	}
}

template<class tipo>
ArbolBin<tipo> ArbolBin<tipo> :: subarbol(tipo nwRaiz) {
	NodoBin<tipo>* raizAux = BUSCAR_NODO(this->raiz, &nwRaiz);
	NodoBin<tipo>* nRaiz = COPIAR_NODOS(raizAux);
	ArbolBin<tipo> nwArbol;
	if(nRaiz != 0){
		nwArbol.raiz = nRaiz;
	}
	return nwArbol;
}

template<class tipo> 
NodoBin<tipo>* ArbolBin<tipo> :: BUSCAR_NODO(NodoBin<tipo>* nod, tipo* elem){
	if(nod != 0){
		if(nod->getElem() == *elem){
			return nod;
		}else{
			if(*elem > nod->getElem()){
				return BUSCAR_NODO(nod->getHijoDer(), elem);
			}else{
				return BUSCAR_NODO(nod->getHijoIzq(), elem); 
			}
		}
	}
	return 0;
}

template<class tipo> 
NodoBin<tipo>* ArbolBin<tipo> :: buscar(tipo elem){
	return BUSCAR_NODO(this->raiz, &elem);
}

template<class tipo> 
void ArbolBin<tipo> :: INSERTAR(NodoBin<tipo>* nod, tipo* elem){
	if(nod != 0){
		if(*elem > nod->getElem()){
			if(nod->getHijoDer() != 0){
				INSERTAR(nod->getHijoDer(), elem);
			}else{
				nod->setHijoDer(new NodoBin<tipo>(*elem));
			}	
		}else{
			if(nod->getHijoIzq() != 0){
				INSERTAR(nod->getHijoIzq(), elem);
			}else{
				nod->setHijoIzq(new NodoBin<tipo>(*elem));
			}	
		}
	}

}

template<class tipo> 
void ArbolBin<tipo> :: insertar(tipo elem){
	INSERTAR(this->raiz, &elem);
	peso = peso + 1;
}


template<class tipo>
NodoBin<tipo>* ArbolBin<tipo> :: COPIAR_NODOS(NodoBin<tipo>* ptrN){
	NodoBin<tipo>* nwNodo;
	if (ptrN == 0 ){
		return (0);
	}else{
		nwNodo = new NodoBin<tipo>(ptrN->getElem(),COPIAR_NODOS(ptrN->getHijoIzq()),COPIAR_NODOS(ptrN->getHijoDer()));
		return(nwNodo);
	}
}

template<class tipo>
void ArbolBin<tipo> :: INSERTAR_SUBARBOL(NodoBin<tipo>* nod, ArbolBin<tipo>& subArbol){
	if(nod != 0){
		if(subArbol.getRaiz()->getElem() > nod->getElem()){
			if(nod->getHijoDer() != 0){
				INSERTAR_SUBARBOL(nod->getHijoDer(), subArbol);
			}else{
				nod->setHijoDer(subArbol.getRaiz());
			}	
		}else{
			if(nod->getHijoIzq() != 0){
				INSERTAR_SUBARBOL(nod->getHijoIzq(), subArbol);
			}else{
				nod->setHijoIzq(subArbol.getRaiz());
			}	
		}
	}
}

template<class tipo>
void ArbolBin<tipo> :: insertarSubarbol(ArbolBin<tipo>& subArbol){
	INSERTAR_SUBARBOL(this->raiz, subArbol);
}

template<class tipo>
void ArbolBin<tipo> :: CAMINO_RAIZ_NODO(NodoBin<tipo>* raiz, vector<tipo>& L, tipo& nodo, bool& encontrado){
	if (raiz != 0 && !encontrado){
		if (!encontrado){
			L.push_back(raiz->getElem());
			if (raiz->getElem() == nodo){
				encontrado = true;
			}
		}
		
		CAMINO_RAIZ_NODO(raiz->getHijoIzq(), L, nodo, encontrado);
		
		CAMINO_RAIZ_NODO(raiz->getHijoDer(), L, nodo, encontrado);
		
		if (!encontrado){
			L.pop_back();
		}
	}
}

template<class tipo>
vector<tipo> ArbolBin<tipo> :: caminoRaizNodo(tipo N){
	vector<tipo> L;
	bool encontrado = false;
	CAMINO_RAIZ_NODO(this->getRaiz(), L, N, encontrado);
	return L;
}

template<class tipo>
vector<tipo> ArbolBin<tipo> :: camino(tipo N2, tipo N1){
	vector<tipo> L1, L2;
	bool encontrado1 = false, encontrado2 = false;
	tipo aux; 
	CAMINO_RAIZ_NODO(this->raiz, L1, N1, encontrado1);
	CAMINO_RAIZ_NODO(this->raiz, L2, N2, encontrado2);
	while ((L1.front() == L2.front()) && !L1.empty()){
		aux = L1[0];
		L1.erase(L1.begin());
		L2.erase(L2.begin());
	}

	L1.insert(L1.begin(), aux); 
	if (!encontrado1 || !encontrado2){
		L1.clear();
		L2.clear();
	}
	
	while (!L2.empty()){
		L1.insert(L1.begin(), L2.front());
		L2.erase(L2.begin()); 
	}
	return L1;
}

/*template<class tipo>
vector<tipo> ArbolBin<tipo> :: frontera(){
	
}*/

template<class tipo>
void ArbolBin<tipo> :: DIAMETRO(NodoBin<tipo>* raiz, vector<tipo>& L, vector<tipo>& cam1, vector<tipo>& cam2, vector<tipo>& diametro){
	if (raiz != 0){
		L.push_back(raiz->getElem());
		
		if(raiz->getHijoIzq() == 0 && raiz->getHijoDer() == 0){
			FUNCION_MAGICA(L, cam1, cam2, diametro);
		}
		DIAMETRO(raiz->getHijoIzq(), L, cam1, cam2, diametro);
		
		DIAMETRO(raiz->getHijoDer(), L, cam1, cam2, diametro);
		
		L.pop_back();

	}
}

template<class tipo>
void ArbolBin<tipo> :: FUNCION_MAGICA(vector<tipo>& L, vector<tipo>& cam1, vector<tipo>& cam2, vector<tipo>& diametro){
	vector<tipo> diametroAux1, diametroAux2;
	if(cam1.empty() ){
		///////////////////////////////////////////////////////////////////////////////////
		cam1 = L;

	}else{
		if(cam2.empty()){
			cam2 = L;
			
			diametroAux1 = INTERSECCION(cam1, cam2); 

		}else{
			diametroAux1 = INTERSECCION(cam1, L);
			diametroAux2 = INTERSECCION(cam2, L);
			if (diametroAux1.size() <= diametroAux2.size()){
				diametroAux1 = diametroAux2;

			}
		}
		
		if(diametroAux1.size() > diametro.size() ){

			diametro = diametroAux1;
		}
		
		/*while (!diametroAux1.empty()){
			cout << diametroAux1[0] <<endl;
			diametroAux1.erase(diametroAux1.begin()) ;
		}*/
	}
}

template<class tipo>
vector<tipo> ArbolBin<tipo> :: INTERSECCION(vector<tipo> L1, vector<tipo> L2){
	tipo aux; 
	
	while ((L1.front() == L2.front()) && !L1.empty()){
		aux = L1[0];
		L1.erase(L1.begin());
		L2.erase(L2.begin());
	}

	L1.insert(L1.begin(), aux); 
	
	while (!L2.empty()){
		L1.insert(L1.begin(), L2.front());
		L2.erase(L2.begin()); 
	}
	
	return L1;
}


template<class tipo>
vector<tipo> ArbolBin<tipo> :: diametro(){
	vector<tipo> L, cam1, cam2, diametro;
	DIAMETRO(this->raiz, L, cam1, cam2, diametro);
	return diametro;
}

template<class tipo>
int ArbolBin<tipo> :: altura(){
	return ALTURA(this->raiz);
}

template<class tipo>
int ArbolBin<tipo> :: ALTURA(NodoBin<tipo>* p){
	int izq,der;
	if (p != 0){
		izq = ALTURA (p->getHijoIzq());
		der = ALTURA (p->getHijoDer());
		if (izq > der){
			return (izq + 1);
		}else{
			return (der + 1);
		}
	}
	return 0;
}

template<class tipo>
vector<tipo> ArbolBin<tipo> :: hojas(){
	vector<tipo> vHojas;
	HOJAS(this->raiz, vHojas);
	return vHojas;
}

template<class tipo>
void ArbolBin<tipo> :: HOJAS(NodoBin<tipo>* p, vector<tipo>& vHojas){
	if (p != 0){
		HOJAS (p->getHijoIzq(), vHojas);
		HOJAS (p->getHijoDer(), vHojas);
		if (p->getHijoIzq()==0 && p->getHijoDer()==0){
			vHojas.push_back(p->getElem());
		}
	}		
}


#endif
