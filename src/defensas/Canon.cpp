#include "Canon.h"
#include "../enemigos/Enemigo.h"
#include "../mapa/Mapa.h"
using namespace std;

// nombre, fila, columna, danio, rango, costo
Canon::Canon(int fila, int columna) : Defensa("Canon", fila, columna, 30, 2, COSTO), turnosRecarga(0) {}

int Canon::atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) {
    if (turnosRecarga > 0) {            // todavia recargando
        turnosRecarga--;
        return 0;
    }
    vector<Enemigo*> enRango = enemigosEnRango(enemigos, mapa);
    if (enRango.empty()) return 0;

    for (Enemigo* e : enRango) {
        *e - danio;                     // explosion: golpea a todos
    }
    objetivo = enRango[0];
    turnosRecarga = 1;                  // dispara un turno si y otro no
    return static_cast<int>(enRango.size());
}

char Canon::simbolo() const {
    return 'C';
}

string Canon::descripcion() const {
    return "Explosion a todos en rango; recarga 1 turno. Ideal contra Orcos.";
}
