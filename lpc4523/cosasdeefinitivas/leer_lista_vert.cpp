#include <iostream>
#include <string>
#include <sstream>
#include "list.h"

using namespace std;

int main(){
    lista<lista<string>* > listaListas;
    string texto, palabra;
    int columnas, contador = 0;
    lista<string>* aux;

    cin >> columnas;
    if (columnas == 0 ||columnas == NULL)   
    
    cin.ignore();

    for (int i=0; i < columnas; i++){
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
}