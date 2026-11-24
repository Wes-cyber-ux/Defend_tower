#include "Orco.h"
#include <algorithm>
using namespace std;

// nombre, vida, velocidad, danio, recompensa, nivel
Orco::Orco(int nivel) : Enemigo("Orco", 70, 1, 12, 8, nivel), armadura(2 + nivel) {}

int Orco::danioEfectivo(int danioBruto) const {
    return max(1, danioBruto - armadura);     // siempre recibe al menos 1 de danio
}

char Orco::simbolo() const {
    return 'O';
}

string Orco::habilidad() const {
    return "Armadura: -" + to_string(armadura) + " a cada golpe";
}
