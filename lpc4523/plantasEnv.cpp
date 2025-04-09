#include <iostream>
#include <string>
#include <sstream>
#include "list.h"

using namespace std;

int main(){
    lista<lista<string>* > listaListas;
    string texto, palabra;
    int muertes, columnas, i, j, contador = 0;
    lista<string>* aux;
    bool dying = true;

    cin >> columnas;
    if (columnas == 0){ cout << "ERROR" << endl; return 0;}
    
    cin.ignore();

    for (i=0; i < columnas; i++){
        listaListas.insertar(new lista<string>, i);
    }

    while(getline(cin, texto) && !texto.empty()){
        istringstream ss(texto);
        while(getline(ss, palabra, ' ')){
            if(!palabra.empty()){
                aux = listaListas.consultarRef(contador);
                contador++;
                aux->insertar(palabra, aux->longitud());
            }
        }
        contador = 0;
    }

    for (i=0; i < columnas; i++){
        contador = 1;
        aux = listaListas.consultarRef(i);
        j = aux->longitud();
        while (j > 0 || dying){
            if (j <= 0) {
                j = aux->longitud();
                contador++;
            }
            if (aux->consultar(j) > aux->consultar(j-1)){
                aux->eliminar(j);
                muertes++;
            }
            if (j <= 1){
                if (muertes == 0) dying = false;
                muertes = 0;
            }
        }
        cout << contador << endl;
    }

    for (i=0; i < columnas; i++){
        aux = listaListas.consultarRef(i);
        delete(aux);
    }
}