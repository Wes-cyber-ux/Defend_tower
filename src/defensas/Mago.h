#pragma once
#include "Defensa.h"

// Mago: lanza un rayo de hielo a los 2 enemigos mas adelantados en rango
// y los congela 1 turno (no avanzan).
class Mago : public Defensa {
private:
    static constexpr int MAX_OBJETIVOS = 2;

public:
    static constexpr int COSTO = 90;

    Mago(int fila, int columna);

    int atacar(std::vector<Enemigo*>& enemigos, const Mapa& mapa) override;
    char simbolo() const override;
    std::string descripcion() const override;
};
