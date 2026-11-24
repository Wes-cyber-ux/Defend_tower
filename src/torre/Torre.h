#pragma once
#include <iostream>

// La torre central del reino. Si su vida llega a 0, el jugador pierde.
class Torre {
private:
    int vida;
    int vidaMax;

public:
    explicit Torre(int vidaMax);

    Torre& operator-=(int danio);       // recibe danio:  torre -= 25;
    friend std::ostream& operator<<(std::ostream& os, const Torre& t);   // barra de vida

    void reiniciar();
    bool estaDestruida() const;
    int getVida() const          { return vida; }
    int getVidaMax() const       { return vidaMax; }
    int getDanioRecibido() const { return vidaMax - vida; }
};
