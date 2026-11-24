#pragma once
#include "Enemigo.h"

// Orco: lento y con armadura. Cada golpe que recibe se reduce en
// "armadura" puntos, por eso conviene atacarlo con golpes fuertes (Canon).
class Orco : public Enemigo {
private:
    int armadura;

protected:
    int danioEfectivo(int danioBruto) const override;

public:
    explicit Orco(int nivel);

    char simbolo() const override;
    std::string habilidad() const override;
};
