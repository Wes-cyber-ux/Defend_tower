#pragma once
#include <iostream>
#include <vector>
#include "../enemigos/Enemigo.h"
using namespace std;

class Oleada {
private:
    vector<Enemigo*> enemigos;
    int numOleada;
    
public:
    Oleada(int n) : numOleada(n) {}

    ~Oleada() {
        for (Enemigo* e : enemigos) {
            delete e;
        }
    }

    void agregarEnemigo(Enemigo* e) {
        enemigos.push_back(e);
    }





    vector<Enemigo*>& getEnemigos() {
        return enemigos;
    }












    int getNumOleada() const {
        return numOleada;
    }




};