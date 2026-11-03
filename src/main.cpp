#include <iostream>
#include <vector>
#include "mapa/Mapa.h"
#include "enemigos/Goblin.h"
#include "enemigos/Orco.h"
using namespace std;

int main() {
    Mapa mapa(8, 20);
    cout << "Largo del camino: " << mapa.getLargoCamino() << endl;

    vector<Enemigo*> enemigos;
    enemigos.push_back(new Goblin());
    enemigos.push_back(new Orco());

    // Colocar un arquero de prueba en una celda vacia
    if (mapa.colocarDefensa(0, 5, 'A'))
        cout << "Defensa colocada" << endl;
    else
        cout << "Esa celda esta ocupada" << endl;

    // 10 turnos: los enemigos avanzan y se redibuja el mapa
    for (int turno = 1; turno <= 14; turno++) {
        cout << "\n--- Turno " << turno << " ---" << endl;
        mapa.dibujar(enemigos);
        for (Enemigo* e : enemigos) e->avanzar();
    }

    for (Enemigo* e : enemigos) delete e;
    return 0;
}