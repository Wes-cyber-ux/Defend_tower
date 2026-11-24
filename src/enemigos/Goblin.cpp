#include "Goblin.h"
using namespace std;

// nombre, vida, velocidad, danio, recompensa, nivel
Goblin::Goblin(int nivel) : Enemigo("Goblin", 30, 2, 5, 4, nivel) {}

int Goblin::pasosPorTurno() const {
    bool enPanico = vida * 2 < vidaMax;      // menos del 50% de vida
    return velocidad + (enPanico ? 1 : 0);
}

char Goblin::simbolo() const {
    return 'G';
}

string Goblin::habilidad() const {
    return "Panico: herido corre +1 casilla";
}
