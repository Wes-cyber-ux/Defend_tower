#pragma once
#include "Defensa.h"
using namespace std;

class Arquero : public Defensa {
public:
    Arquero(int f, int c) : Defensa("Arquero", f, c, 15, 4, 1, 50) {}


    void atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) override
    {
        int golpes = 0;

        for (Enemigo* e : enemigos) {
            if (!e->estaVivo()) continue;
            if (enRango(e, mapa)) {
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
