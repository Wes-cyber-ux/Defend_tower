#include <iostream>
#include <vector>
#include "mapa/Mapa.h"
#include "torre/Torre.h"
#include "oleada/Oleada.h"
#include "enemigos/Goblin.h"
#include "enemigos/Orco.h"
#include "enemigos/Woodtroll.h"
#include "defensas/Arquero.h"
#include "defensas/Mago.h"
#include "defensas/Canon.h"
using namespace std;

// Prueba de una oleada completa con un arquero (despues esto lo hara Juego)
int main() {
    Mapa mapa(8, 20);
    Torre torre(100);
    Oleada oleada(1);

    // Buscar una celda libre pegada al camino para el arquero
    int filaA = -1, colA = -1;
    for (int pos = 3; pos < mapa.getLargoCamino() && filaA == -1; pos++) {
        int f = mapa.getFilaCamino(pos);
        int c = mapa.getColCamino(pos);
        if (mapa.colocarDefensa(f - 1, c, 'A'))      { filaA = f - 1; colA = c; }
        else if (mapa.colocarDefensa(f + 1, c, 'A')) { filaA = f + 1; colA = c; }
    }
    Arquero arquero(filaA, colA);
    cout << arquero << endl;

    int oro = 0;
    int danioRecibido = 0;
    int turno = 0;

    while (!oleada.terminada() && !torre.estaDestruida() && turno < 50) {
        turno++;
        arquero.atacar(oleada.getEnemigos(), mapa);
        oro += oleada.eliminarMuertos();
        oleada.avanzarEnemigos();
        danioRecibido += oleada.procesarLlegadas(torre, mapa);

        cout << "\n--- Turno " << turno << " ---" << endl;
        mapa.dibujar(oleada.getEnemigos());
        torre.mostrarEstado();
        for (Enemigo* e : oleada.getEnemigos()) {
            cout << *e << " | Faltan " << mapa.getLargoCamino() - e->getPosicion() << " casillas" << endl;
        }
    }

    cout << "\n=== Fin de la oleada ===" << endl;
    cout << (torre.estaDestruida() ? "Perdiste" : "La torre resistio") << endl;
    cout << "Turnos: " << turno << endl;
    cout << "Oro ganado: " << oro << endl;
    cout << "Danio recibido: " << danioRecibido << endl;
    return 0;
}
