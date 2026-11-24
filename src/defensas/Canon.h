#pragma once
#include "Defensa.h"

// Canon: danio en area a TODOS los enemigos en rango, pero necesita
// un turno de recarga entre disparos.
class Canon : public Defensa {
private:
    int turnosRecarga;

public:
    static constexpr int COSTO = 150;

    Canon(int fila, int columna);

    int atacar(std::vector<Enemigo*>& enemigos, const Mapa& mapa) override;
    char simbolo() const override;
    std::string descripcion() const override;
};
