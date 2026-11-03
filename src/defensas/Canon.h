#pragma once
#include "Defensa.h"
using namespace std;

class Canon : public Defensa {
public:
    Canon(int f, int c) : Defensa("Canon", f, c, 30, 2, 1, 150) {}


    void atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) override
    {

        for (Enemigo* e : enemigos) {
            if (!e->estaVivo()) continue;
            if (enRango(e, mapa)) {
                *e - danio;
            }

        }
    }

    char simbolo() const override
    {
        return 'C';
    }
};
