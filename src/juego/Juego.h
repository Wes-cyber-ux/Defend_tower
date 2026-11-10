#pragma once
#include <iostream>
#include <vector>
#include "../mapa/Mapa.h"
#include "../torre/Torre.h"
#include "../oleada/Oleada.h"
#include "../estadisticas/Estadisticas.h"
#include "../defensas/Defensa.h"
#include "../defensas/Arquero.h"
#include "../defensas/Mago.h"
#include "../defensas/Canon.h"
using namespace std;

class Juego {
private:
    Mapa mapa;
    Torre torre;
    Estadisticas estadisticas;
    vector<Defensa*> defensas;
    int oro;
    int maxOleadas;

    
    int leerEntero() {
        int x;
        while (!(cin >> x)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ingresa un numero: ";
        }
        return x;
    }

    Defensa* crearDefensa(int tipo, int f, int c) {
        if (tipo == 1) return new Arquero(f, c);
        if (tipo == 2) return new Mago(f, c);
        if (tipo == 3) return new Canon(f, c);
        return nullptr;
    }

    void comprarDefensa(int tipo) {
        cout << "Fila: ";
        int f = leerEntero();
        cout << "Columna: ";
        int c = leerEntero();

        if (!mapa.esLibre(f, c)) {
            cout << "Esa celda no esta libre." << endl;
            return;
        }

        Defensa* d = crearDefensa(tipo, f, c);
        if (d->getCosto() > oro) {
            cout << "No te alcanza el oro (cuesta " << d->getCosto() << ")." << endl;
            delete d;
            return;
        }

        oro -= d->getCosto();
        mapa.colocarDefensa(f, c, d->simbolo());
        defensas.push_back(d);
        cout << "Colocaste: " << *d << endl;
    }

    void mejorarDefensa() {
        if (defensas.empty()) {
            cout << "No tienes defensas." << endl;
            return;
        }
        for (size_t i = 0; i < defensas.size(); i++) {
            cout << i + 1 << ") " << *defensas[i] << " | Mejorar cuesta: " << 40 * defensas[i]->getNivel() << endl;
        }
        cout << "Tienes " << oro << " de oro. Cual mejorar (0 para volver): ";
        int i = leerEntero() - 1;
        if (i == -1) return;
        if (i < 0 || i >= static_cast<int>(defensas.size())) {
            cout << "Opcion invalida." << endl;
            return;
        }

        int costo = 40 * defensas[i]->getNivel();
        if (costo > oro) {
            cout << "No te alcanza el oro (cuesta " << costo << ")." << endl;
            return;
        }
        oro -= costo;
        *defensas[i] += 1;
        cout << "Mejorada: " << *defensas[i] << endl;
    }

    void faseCompra(int numOleada) {
        int opcion = -1;
        while (opcion != 0) {
            cout << "\n===== Preparacion de la oleada " << numOleada << " =====" << endl;
            mapa.dibujar(vector<Enemigo*>());
            torre.mostrarEstado();
            cout << "Oro: " << oro << endl;
            cout << "1) Arquero (50)  2) Mago (90)  3) Canon (150)" << endl;
            cout << "4) Mejorar defensa  0) Empezar oleada" << endl;
            cout << "Opcion: ";
            opcion = leerEntero();

            if (opcion >= 1 && opcion <= 3) comprarDefensa(opcion);
            else if (opcion == 4) mejorarDefensa();
            else if (opcion != 0) cout << "Opcion invalida." << endl;
        }
    }

    void faseCombate(int numOleada) {
        Oleada oleada(numOleada);
        int turno = 0;

        while (!oleada.terminada() && !torre.estaDestruida()) {
            turno++;
            estadisticas.Registrar_Turno();
            cout << "\n--- Oleada " << numOleada << " | Turno " << turno << " ---" << endl;

            // polimorfismo: cada defensa ataca a su manera
            vector<Enemigo*>& enemigos = oleada.getEnemigos();
            for (Defensa* d : defensas) {
                // guardo la vida de cada enemigo para ver a quien golpeo esta defensa
                vector<int> vidaAntes;
                for (Enemigo* e : enemigos) vidaAntes.push_back(e->getVida());

                d->atacar(enemigos, mapa);

                bool golpeo = false;
                for (size_t i = 0; i < enemigos.size(); i++) {
                    int quitado = vidaAntes[i] - enemigos[i]->getVida();
                    if (quitado > 0) {
                        cout << d->getNombre() << " (" << d->getFila() << "," << d->getColumna() << ") ataco a "
                             << enemigos[i]->getNombre() << ": -" << quitado << " de vida";
                        if (!enemigos[i]->estaVivo()) cout << " -> eliminado! +" << enemigos[i]->getRecompensa() << " oro";
                        cout << endl;
                        golpeo = true;
                    }
                }
                if (!golpeo) cout << d->getNombre() << " (" << d->getFila() << "," << d->getColumna() << ") no alcanzo a nadie" << endl;
            }

            size_t antes = oleada.getEnemigos().size();
            int oroGanado = oleada.eliminarMuertos();
            int muertos = static_cast<int>(antes - oleada.getEnemigos().size());
            oro += oroGanado;
            estadisticas.Registrar_Oro(oroGanado);
            estadisticas.Registrar_Eliminados(muertos);

            oleada.avanzarEnemigos();
            int danio = oleada.procesarLlegadas(torre, mapa);
            estadisticas.Registrar_Danio(danio);
            if (danio > 0) cout << "Los enemigos llegaron a la torre: -" << danio << " de vida!" << endl;

            mapa.dibujar(enemigos);
            torre.mostrarEstado();
            cout << "Oro: " << oro << endl;

            cout << "Enemigos:" << endl;
            for (Enemigo* e : enemigos) {
                cout << "  " << *e;
                if (e->getPosicion() < 0) cout << " | En espera" << endl;
                else cout << " | Faltan " << mapa.getLargoCamino() - e->getPosicion() << " casillas" << endl;
            }
        }

        if (!torre.estaDestruida()) {
            estadisticas.Registrar_Oleada();
            int bono = 20 * numOleada;
            oro += bono;
            cout << "\nOleada " << numOleada << " superada! Bono: " << bono << " de oro" << endl;
        }
    }

public:
    Juego(int oleadas) : mapa(8, 20), torre(100), oro(150), maxOleadas(oleadas) {}

    ~Juego() {
        for (Defensa* d : defensas) {
            delete d;
        }
    }

    void jugar() {
        for (int n = 1; n <= maxOleadas && !torre.estaDestruida(); n++) {
            faseCompra(n);
            faseCombate(n);
        }

        cout << "\n========== FIN DEL JUEGO ==========" << endl;
        cout << (torre.estaDestruida() ? "La torre cayo. Perdiste." : "Defendiste el reino. Ganaste!") << endl;
        cout << estadisticas << endl;
    }
};
