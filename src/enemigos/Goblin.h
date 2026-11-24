#pragma once
#include "Enemigo.h"

// Goblin: rapido y fragil. Cuando su vida baja de la mitad entra en panico
// y corre una casilla extra por turno.
class Goblin : public Enemigo {
protected:
    int pasosPorTurno() const override;

public:
    explicit Goblin(int nivel);

    char simbolo() const override;
    std::string habilidad() const override;
};
