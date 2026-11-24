#include "Defensa.h"
#include "../enemigos/Enemigo.h"
#include "../mapa/Mapa.h"
#include <algorithm>
#include <cstdlib>
using namespace std;

Defensa::Defensa(const string& nombre, int fila, int columna, int danio, int rango, int costo)
    : nombre(nombre), fila(fila), columna(columna), danio(danio), rango(rango),
      nivel(1), costo(costo), objetivo(nullptr) {}

bool Defensa::enRango(const Enemigo& e, const Mapa& mapa) const {
    if (!e.estaEnCamino()) return false;
    int fe = mapa.getFilaCamino(e.getPosicion());
    int ce = mapa.getColCamino(e.getPosicion());
    if (fe == -1 || ce == -1) return false;
    int distancia = abs(fe - fila) + abs(ce - columna);      // distancia Manhattan
    return distancia <= rango;
}

vector<Enemigo*> Defensa::enemigosEnRango(const vector<Enemigo*>& enemigos,
                                          const Mapa& mapa) const {
    vector<Enemigo*> resultado;
    for (Enemigo* e : enemigos) {
        if (enRango(*e, mapa)) resultado.push_back(e);
    }
    // operator> de Enemigo: primero los mas cercanos a la torre
    sort(resultado.begin(), resultado.end(),
         [](const Enemigo* a, const Enemigo* b) { return *a > *b; });
    return resultado;
}

// Sobrecarga de "+=": sube "niveles" niveles (sin pasar el maximo).
Defensa& Defensa::operator+=(int niveles) {
    for (int i = 0; i < niveles && nivel < NIVEL_MAXIMO; i++) {
        nivel++;
        danio += 10;                    // +10 de danio en cada nivel
        if (nivel % 2 == 0) rango++;    // +1 de alcance en los niveles 2 y 4
    }
    return *this;
}

ostream& operator<<(ostream& os, const Defensa& d) {
    os << d.simbolo() << " " << d.nombre
       << " Nv." << d.nivel
       << " | Danio: " << d.danio
       << " | Rango: " << d.rango
       << " | Pos: (" << d.fila << "," << d.columna << ")";
    return os;
}

int Defensa::costoMejora() const {
    return costo * nivel * 3 / 4;      // cada nivel cuesta mas
}

bool Defensa::puedeMejorar() const {
    return nivel < NIVEL_MAXIMO;
}

void Defensa::olvidarObjetivo() {
    objetivo = nullptr;
}
