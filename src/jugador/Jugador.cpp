#include "Jugador.h"
#include "../consola/Consola.h"
using namespace std;

Jugador::Jugador() : nombre("Anonimo"), oro(0) {}

void Jugador::reiniciar(const string& nuevoNombre, int oroInicial) {
    nombre = nuevoNombre;
    oro = oroInicial;
}

bool Jugador::puedePagar(int monto) const {
    return monto <= oro;
}

Jugador& Jugador::operator+=(int monto) {
    if (monto > 0) oro += monto;
    return *this;
}

Jugador& Jugador::operator-=(int monto) {
    if (monto > 0 && monto <= oro) oro -= monto;     // nunca queda oro negativo
    return *this;
}

ostream& operator<<(ostream& os, const Jugador& j) {
    os << "Comandante " << j.nombre << " | Oro: " << Consola::color(to_string(j.oro), Consola::AMARILLO);
    return os;
}
