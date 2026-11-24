#include "Oleada.h"
#include "../enemigos/Goblin.h"
#include "../enemigos/Orco.h"
#include "../enemigos/Troll.h"
#include "../mapa/Mapa.h"
#include "../torre/Torre.h"
#include <stdexcept>
using namespace std;

Oleada::Oleada(int numero)
    : numero(numero), cantGoblins(0), cantOrcos(0), cantTrolls(0) {
    generarEnemigos();
}

Oleada::~Oleada() {
    for (Enemigo* e : enemigos) delete e;      // composicion: muere la oleada, mueren sus enemigos
    enemigos.clear();
}

// Cada oleada trae mas enemigos y de mayor nivel:
//   Goblins: 3 + n   |   Orcos: desde la oleada 2   |   Trolls: desde la oleada 4
void Oleada::generarEnemigos() {
    cantGoblins = 3 + numero;
    cantOrcos   = (numero >= 2) ? numero / 2 + (numero >= 6 ? 1 : 0) : 0;
    cantTrolls  = (numero >= 4) ? (numero - 2) / 2 : 0;

    int goblins = cantGoblins, orcos = cantOrcos, trolls = cantTrolls;

    // Se intercalan para que la oleada sea variada: G, O, G, W, G, O, ...
    while (goblins > 0 || orcos > 0 || trolls > 0) {
        if (goblins > 0) { enemigos.push_back(new Goblin(numero)); goblins--; }
        if (orcos > 0)   { enemigos.push_back(new Orco(numero));   orcos--; }
        if (goblins > 0) { enemigos.push_back(new Goblin(numero)); goblins--; }
        if (trolls > 0)  { enemigos.push_back(new Troll(numero));  trolls--; }
    }
}

void Oleada::liberarSiguiente() {
    for (Enemigo* e : enemigos) {
        if (e->estaEnEspera()) {
            e->entrarAlCamino();
            return;                 // solo uno por turno, para que no salgan amontonados
        }
    }
}

void Oleada::avanzar() {
    for (Enemigo* e : enemigos) e->avanzar();      // llamada polimorfica
}

int Oleada::procesarLlegadas(const Mapa& mapa, Torre& torre) {
    int danioTotal = 0;
    for (Enemigo* e : enemigos) {
        if (e->estaEnCamino() && mapa.llegoAlFinal(e->getPosicion())) {
            torre -= e->getDanio();                // operator-= de Torre
            danioTotal += e->getDanio();
            e->llegarATorre();
        }
    }
    return danioTotal;
}

int Oleada::cobrarRecompensas(int& bajas) {
    int oro = 0;
    for (Enemigo* e : enemigos) {
        int recompensa = e->cobrarRecompensa();
        if (recompensa > 0) {
            oro += recompensa;
            bajas++;
        }
    }
    return oro;
}

bool Oleada::terminada() const {
    for (const Enemigo* e : enemigos) {
        if (!e->haTerminado()) return false;
    }
    return true;
}

int Oleada::contarEnCamino() const {
    int total = 0;
    for (const Enemigo* e : enemigos) if (e->estaEnCamino()) total++;
    return total;
}

int Oleada::contarPendientes() const {
    int total = 0;
    for (const Enemigo* e : enemigos) if (e->estaEnEspera()) total++;
    return total;
}

Enemigo* Oleada::operator[](int indice) const {
    if (indice < 0 || indice >= getCantidad()) throw out_of_range("Indice de enemigo invalido");
    return enemigos[indice];
}

ostream& operator<<(ostream& os, const Oleada& o) {
    os << "Oleada " << o.numero << " (nivel " << o.numero << "): "
       << o.cantGoblins << " Goblins, " << o.cantOrcos << " Orcos, " << o.cantTrolls << " Trolls";
    return os;
}
