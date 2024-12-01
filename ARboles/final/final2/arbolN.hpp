 
#ifndef arbol_Nario
#define arbol_NArio

#include <vector>
#include <queue>
#include "nodoArbolN.hpp"
 																	  //|||||\\
    //////////////////////////////////////////////                   //       \\
   // Autor: Sebastian Piñango				   //				   ||  ^	^	||
  //  v 2.0								      //				   ||     _     || me wa mata mañana
 //////////////////////////////////////////////				        \\		  //
/*																	   ||   ||    */

using namespace std;

template<class tipo>
class ArbolN{
	private:
		//ATRIBUTOS
		NodoN<tipo>* raiz;
		//Metodos Privados
		void ELIMINAR_ARBOL(NodoN<tipo>* Nodo);//
		void CONTADOR(NodoN<tipo>* Nodo, int& i);//
		void INORDEN(NodoN<tipo>* Nodo, vector<tipo> L);//
		void PREORDEN(NodoN<tipo>* Nodo, vector<tipo>& L);//
		void POSTORDEN(NodoN<tipo>* Nodo, vector<tipo>& L);//
		void NIVELES(NodoN<tipo>* Nodo, vector<tipo>& L, queue<NodoN<tipo>*>& cola);//
		void BUSCAR_NIVEL(NodoN<tipo>* Nodo, vector<tipo>& L, queue<NodoN<tipo>*>& cola, int nivelInic, int nivelFin);//
		NodoN<tipo>* BUSCAR_NODO(NodoN<tipo>* nod, tipo* elem);//
		NodoN<tipo>* COPIAR_NODOS(NodoN<tipo>* ptrN);//
		void INSERTAR(NodoN<tipo>* nod, tipo* elem);//
		void INSERTAR_SUBARBOL(NodoN<tipo>* nod, ArbolN<tipo>& subArbol);//
		void CAMINO_RAIZ_NODO(NodoN<tipo>* raiz, vector<tipo>& L,tipo& nodo, bool& encontrado);//
		int ALTURA(NodoN<tipo>);//
		void HOJAS(NodoN<tipo>* p, vector<tipo> vHojas);//
	public:
		ArbolN();//
		ArbolN(tipo elem);//
		ArbolN(tipo elem, NodoN<tipo>* hijo);//
		ArbolN(NodoN<tipo>* nRaiz);//
		~ArbolN();//
		void crearEnPre(vector<tipo> inOrden, vector<tipo> preOrden);
		void setRaiz(NodoN<tipo>* R);//
		int peso();//
		NodoN<tipo>* getRaiz();//
		vector<tipo> hijos();//
		//lecturas de arbol/////////////////////////
		vector<tipo> inOrden();                  //
		vector<tipo> preOrden();                //
		vector<tipo> postOrden();              //
		vector<tipo> porNiveles();            //
		///////////////////////////////////////
		vector<tipo> nivel(int nivInicial, int nivFinal);// lista de uno o mas niveles definidos 
		NodoN<tipo>* buscar(tipo elem);//
		ArbolN<tipo> subarbol(tipo nwRaiz);//devuelve un subarbol a partir de un nodo que hara de raiz
		void insertar(tipo elem);//
		void insertarSubarbol(ArbolN<tipo>& subArbol);///////////////////////////////////////////////
		vector<tipo> caminoRaizNodo(tipo N);//
		vector<tipo> camino(tipo N1, tipo N2);//
		/*
		vector<tipo> diametro();
		int altura();*/
		vector<tipo> hojas(); //devuelve una lista de todas las hojas del arbol//
};

ArbolN<tipo> A = ArbolN(1);
A.peso();

template <class tipo>
ArbolN<tipo> :: ArbolN(){
	raiz = 0;
}

template <class tipo>
ArbolN<tipo> :: ArbolN(tipo elem){
	raiz = new NodoN<tipo>(elem);
}

template <class tipo>
ArbolN<tipo> :: ArbolN(tipo elem, NodoN<tipo>* hijo){
	raiz = new NodoN<tipo>(elem, hijo);
}

template <class tipo>
ArbolN<tipo> :: ArbolN(NodoN<tipo>* nRaiz){
	raiz = nRaiz;
}

template <class tipo>
void ArbolN<tipo> :: ELIMINAR_ARBOL(NodoN<tipo>* Nodo){
	if (Nodo != 0){
		ELIMINAR_ARBOL(Nodo->getHijo());
		ELIMINAR_ARBOL(Nodo->getHermano());
		Nodo->~NodoN<tipo>();
	}
}

template <class tipo>
ArbolN<tipo> :: ~ArbolN(){
	ELIMINAR_ARBOL(this->raiz);
}

template <class tipo>
void ArbolN<tipo> :: crearEnPre(vector<tipo> inOrden, vector<tipo> preOrden){
	
}

template <class tipo>
void ArbolN<tipo> :: setRaiz(NodoN<tipo>* R){
	raiz = R;
}

template <class tipo>
void ArbolN<tipo> :: CONTADOR(NodoN<tipo>* Nodo, int& i){
	if (Nodo != 0){
		CONTADOR(Nodo->getHijo());
		CONTADOR(Nodo->getHermano());
		i++;
	}
}

template <class tipo>
int ArbolN<tipo> :: peso(){
	int i = 0;
	CONTADOR(this->raiz, i);
	return i;
}

template <class tipo>
NodoN<tipo>* ArbolN<tipo> :: getRaiz(){
	return (this->raiz);
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: hijos(){
	NodoN<tipo>* ptrNodo = this->raiz;
	vector<tipo> out;
	while (ptrNodo->getHermano() != 0){
		out.push_back(ptrNodo->getElem());
		ptrNodo = ptrNodo->getHermano();   
	}
	return out;
}

template <class tipo>
void ArbolN<tipo> :: INORDEN(NodoN<tipo>* Nodo, vector<tipo> L){
	if (Nodo != 0){	
		NodoN<tipo>* aux;	
		INORDEN(Nodo->getHijo(), L, aux);
		
		L.push_back(Nodo->getElem());
		
		aux = Nodo->getHijo()->getHermano();
		while (aux !=0){
			INORDEN(aux, L, aux);
			aux = aux->getHermano();
		}
	}	
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: inOrden(){
	vector<tipo> L;
	INORDEN(this->raiz, L);
	return L;
}

template <class tipo>
void ArbolN<tipo> :: PREORDEN(NodoN<tipo>* Nodo, vector<tipo>& L, NodoN<tipo>* aux){	
	if (Nodo != 0){
		NodoN<tipo>* aux;
		L.push_back(Nodo->getElem()); 
		
		aux = Nodo->getHijo();
		while(aux != 0){
			PREORDEN(Nodo->getHijo(), L);
			aux = aux->getHermanoDerecho();
		}
	}
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: preOrden(){
	vector<tipo> L;
	PREORDEN(this->raiz, L, aux);
	return L;
}

template <class tipo>
void ArbolN<tipo> :: POSTORDEN(NodoN<tipo>* Nodo, vector<tipo>& L){
	if (Nodo != 0){
		NodoN<tipo>* aux = Nodo->getHijo();

		while (aux != 0){ 		
			POSTORDEN(aux, L);
			aux = aux->getHermano();
		}
		
		L.push_back(Nodo->getElem());
	}
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: postOrden(){
	vector<tipo> L;
	POSTORDEN(this->raiz, L);
	return L;
}

template<class tipo>
void ArbolN<tipo> :: NIVELES(NodoN<tipo>* Nodo, vector<tipo>& L, queue<NodoN<tipo>*>& cola){
	cola.push(0); 
	int i=0;//flag para iterar en el vector visitados
	NodoN<tipo>* aux;
	vector<tipo> visitados;//cola de hermanos para no repetir hermanos
	while(!cola.empty() && Nodo != 0){
		L.push_back(Nodo->getElem());
		aux = Nodo; i = 0;
		while (aux->getHermano() != 0){
			if (aux->getElem() == visitados.front()){
				visitados.pop();
			}
			if(aux->getHermano()->getElem() != visitados[i]) {
				cola.push(aux->getHermano());
				visitados.push_back(aux->getHermano()->getElem()); 
			}else{
				i++; 
			}
			aux = aux->getHermano();
		}
		
		if (Nodo->getHijo() != 0){
			cola.push(Nodo->getHijo());
		}

		cola.pop();
		Nodo = cola.front();
	}
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: porNiveles(){
	vector<tipo> L;
	queue<NodoN<tipo>* > cola;
	NIVELES(this->raiz, L, cola);
	return L;
}

template <class tipo>
void ArbolN<tipo> :: BUSCAR_NIVEL(NodoN<tipo>* Nodo, vector<tipo>& L, queue<NodoN<tipo>*>& cola, int nivelInic, int nivelFin){
	cola.push(Nodo);
	int cont = 1;
	cola.push(0);
	NodoN<tipo>* nodoAux;
	while (!cola.empty() && cont<= nivelFin){
		nodoAux = Nodo;
		if (cola.front() == 0){
				cola.push(0);
				cont++;
		}else{
			if(cont >= nivelInic && cont <= nivelFin){
				L.push_back(Nodo->getElem());
			}
			if (Nodo->getHijo() != 0){
				cola.push(Nodo->getHijo());
			}
			while (nodoAux->getHermano() != 0){
				cola.push(nodoAux->getHermano());
			}
		}
		cola.pop();
		Nodo = cola.front();
	}
} 

template <class tipo>
vector<tipo> ArbolN<tipo> :: nivel(int nivInicial, int nivFinal){
	vector<tipo> L;
	queue<tipo> cola;
	
	BUSCAR_NIVEL (this->raiz, L, cola,nivInicial, nivFinal);

	return L;
}

template <class tipo>
NodoN<tipo>* ArbolN<tipo> :: BUSCAR_NODO(NodoN<tipo>* nod, tipo* elem){
	if(nod != 0){
		if(nod->getElem() == *elem){
			return nod;
		}else{
			if(*elem > nod->getElem()){
				return BUSCAR_NODO(nod->getHijo(), elem);
			}else{
				return BUSCAR_NODO(nod->getHermano(), elem);
			}
		}
	}
	return 0;
}

template <class tipo>
NodoN<tipo>* ArbolN<tipo> :: buscar(tipo elem){
	return (BUSCAR_NODO(this->raiz, &elem));
}

template<class tipo>
NodoN<tipo>* ArbolN<tipo> :: COPIAR_NODOS(NodoN<tipo>* ptrN){
	NodoN<tipo>* nwNodo;
	if (ptrN == 0 ){
		return (0);
	}else{
		nwNodo = new NodoN<tipo>(ptrN->getElem(),COPIAR_NODOS(ptrN->getHijo()),COPIAR_NODOS(ptrN->getHermano()));
		return(nwNodo);
	}
}

template <class tipo>
ArbolN<tipo> ArbolN<tipo> :: subarbol(tipo nwRaiz){
	NodoN<tipo>* aux = BUSCAR_NODO(this->raiz, &nwRaiz);
	NodoN<tipo>* nRaiz = COPIAR_NODOS(aux);
	ArbolN<tipo> nwArbol;
	if(nRaiz != 0){
		nwArbol.raiz = nRaiz;
	}
}


template <class tipo>
void ArbolN<tipo> :: INSERTAR_SUBARBOL(NodoN<tipo>* nod, NodoN<tipo>* aux, ArbolN<tipo>& subArbol){
	if(nod != 0){
		if (nod->getHijo() != 0){
			aux = nod->getHijo();
			while (aux->getHermano()!=0){
				aux = aux->getHermano();
			}
			aux->setHermano(COPIAR_NODOS(subArbol.getRaiz()));
		}else{
			nod->setHijo(COPIAR_NODOS(subArbol.getRaiz()));
		}
	}
}

template <class tipo>
void ArbolN<tipo> :: insertarSubarbol(ArbolN<tipo>& subArbol){
	NodoN<tipo>* aux;
	INSERTAR_SUBARBOL(this->raiz, aux, subArbol);
}

template <class tipo>
void ArbolN<tipo> :: CAMINO_RAIZ_NODO(NodoN<tipo>* raiz, vector<tipo>& L,tipo& nodo, bool& encontrado){
	if (raiz != 0 && !encontrado){
		if (!encontrado){
			L.push_back(raiz->getElem());
			if (raiz->getElem() == nodo){
				encontrado = true;
			}
		}
		
		CAMINO_RAIZ_NODO(raiz->getHijo(), L, nodo, encontrado);
		
		CAMINO_RAIZ_NODO(raiz->getHermano(), L, nodo, encontrado);
		
		if (!encontrado){
			L.pop_back();
		}
	}
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: caminoRaizNodo(tipo N){
	vector<tipo> L;
	bool encontrado = false;
	CAMINO_RAIZ_NODO(this->getRaiz(), L, N, encontrado);
	return L;
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: camino(tipo N1, tipo N2){
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
/*
template <class tipo>
vector<tipo> ArbolN<tipo> :: diametro(){
}


template <class tipo>
int ArbolN<tipo> :: ALTURA(NodoN<tipo> p){
	int izq,der;
	if (p != 0){
		izq = ALTURA(p->getHijo());
		der = ALTURA(p->getHermano());
		if (izq > der){
			return (izq + 1);
		}else{
			return (der + 1);
		}
	}
	return 0;
}

template <class tipo>
int ArbolN<tipo> :: altura(){
	return 0;
}
*/
template <class tipo>
void ArbolN<tipo> :: HOJAS(NodoN<tipo>* p, vector<tipo> vHojas){
	if (p != 0){
		HOJAS (p->getHijo(), vHojas);
		HOJAS (p->getHermano(), vHojas);
		if (p->getHijo()==0){
			vHojas.push_back(p->getElem());
		}
	}	
}

template <class tipo>
vector<tipo> ArbolN<tipo> :: hojas(){
	vector<tipo> vHojas;
	HOJAS(this->raiz, vHojas);
	return vHojas;
}

#endif