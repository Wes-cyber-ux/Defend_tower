#pragma once
#include "Enemigo.h"

// Troll del bosque: muy resistente, lento, golpea fuerte a la torre y
// regenera parte de su vida cada turno que avanza.
class Troll : public Enemigo {
private:
    static constexpr int REGENERACION = 4;    // % de la vida maxima por turno

public:
    explicit Troll(int nivel);

    void avanzar() override;
    char simbolo() const override;
    std::string habilidad() const override;
};
