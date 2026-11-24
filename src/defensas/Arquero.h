#pragma once
#include "Defensa.h"

// Arquero: barato y de largo alcance. Fija un objetivo y lo persigue
// mientras siga vivo y en rango (usa la asociacion "objetivo").
class Arquero : public Defensa {
public:
    static constexpr int COSTO = 50;

    Arquero(int fila, int columna);

    int atacar(std::vector<Enemigo*>& enemigos, const Mapa& mapa) override;
    char simbolo() const override;
    std::string descripcion() const override;
};
