#include <iostream>
#include <string>
#include "list.h"

using namespace std;

int calcularAzotes(lista<string>& listaElfos) {  // Eliminado const
    int totalAzotes = 0;
    int longitud = listaElfos.longitud();

    // Crear un diccionario para guardar las posiciones de cada apodo
    lista<string> apodos;
    lista<int> posiciones;

    for (int i = 1; i <= longitud; ++i) {
        string apodoActual = listaElfos.consultar(i);
        bool encontrado = false;
        
        // Buscar si el apodo ya existe en nuestra lista
        for (int j = 1; j <= apodos.longitud(); ++j) {
            if (apodos.consultar(j) == apodoActual) {
                // Calcular la distancia entre las repeticiones
                int distancia = i - posiciones.consultar(j) - 1;
                totalAzotes += distancia;
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            // Si no se encontró, agregar el nuevo apodo
            apodos.insertar(apodoActual, apodos.longitud() + 1);
            posiciones.insertar(i, posiciones.longitud() + 1);
        }
    }

    return totalAzotes;
}

int main() {
    lista<lista<string>> L;
    int n;
    string linea;

    cin >> n;
    cin.ignore(); // Consumir el salto de línea después de n

    // Leer las n líneas siguientes
    for (int i = 0; i < n; ++i) {
        getline(cin, linea);
        lista<string> listaElfos;
        
        string palabra;
        for (char c : linea) {
            if (c == ' ') {
                if (!palabra.empty()) {
                    listaElfos.insertar(palabra, listaElfos.longitud() + 1);
                    palabra.clear();
                }
            } else {
                palabra += c;
            }
        }
        
        if (!palabra.empty()) {
            listaElfos.insertar(palabra, listaElfos.longitud() + 1);
        }
        
        L.insertar(listaElfos, L.longitud() + 1);
    }

    // Calcular y mostrar los azotes para cada lista
    for (int i = 1; i <= L.longitud(); ++i) {
        lista<string>* temp = L.consultar(i);
        int azotes = calcularAzotes(temp);
        cout << azotes << endl;
    }

    return 0;
}