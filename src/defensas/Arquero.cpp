#include "Arquero.h"
#include "../enemigos/Enemigo.h"
#include "../mapa/Mapa.h"
using namespace std;

// nombre, fila, columna, danio, rango, costo
Arquero::Arquero(int fila, int columna) : Defensa("Arquero", fila, columna, 15, 4, COSTO) {}

int Arquero::atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) {
    // Si su objetivo murio, llego a la torre o salio del rango, busca otro.
    if (objetivo == nullptr || !enRango(*objetivo, mapa)) {
        vector<Enemigo*> candidatos = enemigosEnRango(enemigos, mapa);
        objetivo = candidatos.empty() ? nullptr : candidatos[0];
    }
    if (objetivo == nullptr) return 0;

    *objetivo - danio;          // operator- sobrecargado
    return 1;
}

char Arquero::simbolo() const {
    return 'A';
}

string Arquero::descripcion() const {
    return "Una flecha por turno a su objetivo fijo. Largo alcance.";
}
