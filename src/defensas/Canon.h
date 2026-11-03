#pragma once
#include <iostream>
#include <cstdlib>
#include "Defensa.h"
#include "../mapa/Mapa.h"
using namespace std;

class Canon : public Defensa {
public:
    Canon(int f, int c) : Defensa("Canon", f, c, 30, 2, 1, 150) {}


    void atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) override
    {

        for (Enemigo* e : enemigos) {
            if (!e->estaVivo()) continue;

            
            int pos = e->getPosicion();
            int fe = mapa.getFilaCamino(pos);
            int ce = mapa.getColCamino(pos);
            if (fe == -1 || ce == -1) continue;  

            
            int distancia = abs(fe - fila) + abs(ce - columna);

           
            if (distancia <= rango) {
                *e - danio;
            }

        }
    }

    char simbolo() const override
    {
        return 'C';
    }
};
