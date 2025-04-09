#include <iostream>
#include <string>
#include <sstream>
#include "list.h"

using namespace std;

int main(){
    lista<lista<string>* > listaListas;
    string texto, palabra;
    int columnas, contador;
    lista<string>* aux;

    cin >> columnas;
    cin.ignore();

    for (int i=0; i < columnas; i++){
        listaListas.insertar(new lista<string>, i);
    }
    
}