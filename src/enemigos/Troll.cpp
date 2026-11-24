#include "Troll.h"
#include <algorithm>
using namespace std;

// nombre, vida, velocidad, danio, recompensa, nivel
Troll::Troll(int nivel) : Enemigo("Troll", 150, 1, 25, 15, nivel) {}

void Troll::avanzar() {
    if (estaEnCamino() && vida < vidaMax) {
        int curacion = max(1, vidaMax * REGENERACION / 100);
        vida = min(vidaMax, vida + curacion);
    }
    Enemigo::avanzar();     // reutiliza el movimiento de la clase base
}

char Troll::simbolo() const {
    return 'W';             // W de "Wood troll" (la T es la torre)
}

string Troll::habilidad() const {
    return "Regenera " + to_string(REGENERACION) + "% de vida por turno";
}
