#ifndef grafo_arcos
#define grafo_arcos

#include <iostream>
#include <vector>
#include "vertices.hpp"

using namespace std;

template <class elem> class vertices;
template <class elem> class arco;

template <class elem>
class arco{
    protected:
        vector<elem>* direccionArco;
        elem peso;
    public:
        arco();
        arco()
};

template <class elem> 
class arcos{
    private:
        vector<arco> listArcos;
    protected:
    public:
        arcos();
        arcos(arco primerArco);
        arcos(vector<arco>)
};
#endif