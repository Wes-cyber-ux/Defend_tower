#pragma once
#include "Defensa.h"
using namespace std;

class Mago : public Defensa {
public:
    Mago(int f, int c) : Defensa("Mago", f, c, 20, 3, 1, 90) {}


    void atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) override
    {
        int golpes = 0;

        for (Enemigo* e : enemigos) {
            if (!e->estaVivo()) continue;
            if (enRango(e, mapa)) {
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
