#pragma once
#include <iostream>
#include <cstdlib>
#include "Defensa.h"
#include "../mapa/Mapa.h"
using namespace std;

class Mago : public Defensa {
public:
    Mago(int f, int c) : Defensa("Mago", f, c, 20, 3, 1, 90) {}


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

            if (golpes == 2) break;
        }
    }

    char simbolo() const override
    {
        return 'M';
    }
};
