#pragma once
#include <iostream>
using namespace std;

class Torre {
private:
    int vida;
    int vidamax;

public:
    Torre(int v) : vida(v), vidamax(v) {}

    void recibirDanio(int danio) {
        vida -= danio;
        if (vida < 0) {
            vida = 0;
        }
    }


    Torre& operator-=(int danio) {
        recibirDanio(danio);
        return *this;
    }




    int getVida() const {
        return vida;
    }

    

    bool estaDestruida() const {
        return vida <= 0;
    }

    void mostrarEstado() const {
        cout << "Vida de la torre: " << vida << "/" << vidamax <<endl;
    }

};