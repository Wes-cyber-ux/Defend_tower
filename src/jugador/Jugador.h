#pragma once
#include <iostream>
#include <string>

// El comandante: tiene un nombre y el oro disponible para construir y mejorar.
class Jugador {
private:
    std::string nombre;
    int oro;

public:
    Jugador();

    void reiniciar(const std::string& nombre, int oroInicial);
    bool puedePagar(int monto) const;

    Jugador& operator+=(int monto);     // gana oro:   jugador += 20;
    Jugador& operator-=(int monto);     // gasta oro:  jugador -= 50;
    friend std::ostream& operator<<(std::ostream& os, const Jugador& j);

    std::string getNombre() const { return nombre; }
    int getOro() const            { return oro; }
};
