#pragma once
#include <iostream>
#include <cstdlib>
#include "Defensa.h"
#include "../mapa/Mapa.h"
using namespace std;

class Arquero : public Defensa {
public:
    Arquero(int f, int c) : Defensa("Arquero", f, c, 15, 4, 1, 50) {}


    void atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) override
    {
        int golpes = 0;

        for (Enemigo* e : enemigos) {
            if (!e->estaVivo()) continue;

            
            int pos = e->getPosicion();
            int fe = mapa.getFilaCamino(pos);
            int ce = mapa.getColCamino(pos);
            if (fe == -1 || ce == -1) continue;  

            
            int distancia = abs(fe - fila) + abs(ce - columna);

           
            if (distancia <= rango) {
                *e - danio;
                golpes++;
            }

            if (golpes == 1) break;
        }
    }

    char simbolo() const override
    {
        return 'A';
    }
};
