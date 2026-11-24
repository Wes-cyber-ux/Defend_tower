#include "Mago.h"
#include "../enemigos/Enemigo.h"
#include "../mapa/Mapa.h"
using namespace std;

// nombre, fila, columna, danio, rango, costo
Mago::Mago(int fila, int columna) : Defensa("Mago", fila, columna, 20, 3, COSTO) {}

int Mago::atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) {
    vector<Enemigo*> enRango = enemigosEnRango(enemigos, mapa);
    int golpes = 0;
    for (Enemigo* e : enRango) {
        if (golpes == MAX_OBJETIVOS) break;
        *e - danio;
        e->congelar(1);                 // pierde su proximo movimiento
        golpes++;
    }
    objetivo = enRango.empty() ? nullptr : enRango[0];
    return golpes;
}

char Mago::simbolo() const {
    return 'M';
}

string Mago::descripcion() const {
    return "Rayo de hielo a 2 enemigos: los congela 1 turno.";
}
