#include <iostream>
#include "list.h"
#include "pila.h"
#include "cola.h"

using namespace std;

bool esEquilibrada();

int main() {

	if (esEquilibrada()){
		cout << endl << "true" << endl;
	}
	else {
		cout << endl << "flase" << endl;
	}

	return(0);
}

bool esEquilibrada() {
	Pila<char> P;
	Cola<char> C;
 	char dato;
	while (cin >> dato) {
		cout << dato << " ";
		if (dato == '{') {
			C.encolar(dato);
			P.apilar(dato);
		}
		else if (dato == '[') {
			C.encolar(dato);
			P.apilar(dato);
		}
		else if (dato == '(') {
			C.encolar(dato);
			P.apilar(dato);
		}
		else if (dato == '}') {
			if (P.tope() == '{') {
				C.desencolar();
				P.desapilar();
			}
			else {
				C.encolar(dato);
				P.apilar(dato);
			}
		}
		else if (dato == ']') {
			if (P.tope() == '[') {
				C.desencolar();
				P.desapilar();
			}
			else {
				C.encolar(dato);
				P.apilar(dato);
			}
		}
		else if (dato == ')') {
			if (P.tope() == '(') {
				C.desencolar();
				P.desapilar();
			}
			else {
				C.encolar(dato);
				P.apilar(dato);
			}
		}
	}
	return (P.esVacia());
}