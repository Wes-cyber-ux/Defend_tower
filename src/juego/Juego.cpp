#include "Juego.h"
#include "../consola/Consola.h"
#include "../defensas/Arquero.h"
#include "../defensas/Mago.h"
#include "../defensas/Canon.h"
#include "../enemigos/Enemigo.h"
#include "../oleada/Oleada.h"
#include <iostream>
using namespace std;

Juego::Juego()
    : mapa(FILAS, COLUMNAS), torre(VIDA_TORRE), estadisticas("historial.txt"),
      oleadasSuperadas(0), enemigosEliminados(0), defensasConstruidas(0), modoAnimado(true) {}

Juego::~Juego() {
    liberarDefensas();
}

// =====================================================================
//  MENU PRINCIPAL
// =====================================================================
void Juego::menu() {
    Consola::inicializar();
    estadisticas.cargar();                       // lee el historial guardado

    int opcion = 0;
    do {
        Consola::limpiar();
        mostrarTitulo();
        cout << "  1. Jugar\n"
             << "  2. Ver estadisticas\n"
             << "  3. Salir\n\n";
        opcion = Consola::leerEntero("  Elige una opcion: ", 1, 3);

        switch (opcion) {
            case 1:
                jugar();
                break;
            case 2:
                Consola::limpiar();
                estadisticas.mostrar();
                Consola::esperarEnter();
                break;
            case 3:
                cout << Consola::color("\n  El reino de Eldoria te lo agradece. Hasta pronto!\n\n", Consola::VERDE);
                break;
        }
    } while (opcion != 3);
}

// =====================================================================
//  PARTIDA
// =====================================================================
void Juego::jugar() {
    Consola::limpiar();
    mostrarTitulo();
    string nombre = Consola::leerTexto("  Nombre del comandante: ");
    if (nombre.empty()) nombre = "Anonimo";
    modoAnimado = Consola::leerSiNo("  Ver cada oleada turno a turno (animado)? (s/n): ");
    reiniciarPartida(nombre);

    bool rendido = false;
    for (int numero = 1; numero <= TOTAL_OLEADAS; numero++) {
        Oleada oleada(numero);                   // se destruye sola al terminar la iteracion
        if (!faseDePreparacion(oleada)) {
            rendido = true;
            break;
        }
        ejecutarOleada(oleada);
        if (torre.estaDestruida()) break;

        oleadasSuperadas++;
        if (numero < TOTAL_OLEADAS) {
            int bonus = 30 + 5 * numero;
            jugador += bonus;                    // operator+= de Jugador
            cout << "  Bonus por resistir la oleada: " << Consola::color("+" + to_string(bonus) + " oro", Consola::AMARILLO) << '\n';
        }
        Consola::esperarEnter();
    }

    bool victoria = !rendido && !torre.estaDestruida();
    mostrarResultado(victoria, rendido);

    Partida partida(jugador.getNombre(), victoria, oleadasSuperadas, enemigosEliminados,
                    torre.getDanioRecibido(), jugador.getOro(), defensasConstruidas);
    if (estadisticas.registrar(partida)) {
        cout << "  Resultado guardado en el historial.\n";
    }
    liberarDefensas();
    Consola::esperarEnter();
}

void Juego::reiniciarPartida(const string& nombre) {
    liberarDefensas();
    mapa.reiniciar();                            // nuevo camino aleatorio
    torre.reiniciar();
    jugador.reiniciar(nombre, ORO_INICIAL);
    oleadasSuperadas = 0;
    enemigosEliminados = 0;
    defensasConstruidas = 0;
}

// Antes de cada oleada el jugador construye y mejora. Devuelve false si se rinde.
bool Juego::faseDePreparacion(const Oleada& oleada) {
    while (true) {
        Consola::limpiar();
        mostrarCabecera(oleada.getNumero());
        mapa.dibujar(oleada.getEnemigos());
        mapa.mostrarLeyenda();
        cout << "\n  Se acerca -> " << oleada << "\n\n"
             << "  1. Colocar defensa\n"
             << "  2. Mejorar defensa\n"
             << "  3. Ver mis defensas\n"
             << "  4. Ver enemigos de la oleada\n"
             << "  5. " << Consola::color("INICIAR OLEADA", Consola::VERDE) << '\n'
             << "  0. Rendirse\n";
        int opcion = Consola::leerEntero("  Opcion: ", 0, 5);

        switch (opcion) {
            case 1: colocarDefensa(); break;
            case 2: mejorarDefensa(); break;
            case 3: listarDefensas(); Consola::esperarEnter(); break;
            case 4: verEnemigos(oleada); Consola::esperarEnter(); break;
            case 5: return true;
            case 0:
                if (Consola::leerSiNo("  Seguro que quieres rendirte? (s/n): ")) return false;
                break;
        }
    }
}

void Juego::colocarDefensa() {
    // Arreglo con los precios de la tienda (indice = opcion del menu)
    const int costos[] = {0, Arquero::COSTO, Mago::COSTO, Canon::COSTO};

    cout << "\n  TIENDA (oro disponible: " << jugador.getOro() << ")\n";
    for (int tipo = 1; tipo <= 3; tipo++) {
        Defensa* muestra = crearDefensa(tipo, 0, 0);   // objeto temporal para mostrar sus datos
        cout << "  " << tipo << ". " << muestra->getNombre() << " (" << muestra->simbolo() << ")"
             << " - " << costos[tipo] << " oro | danio " << muestra->getDanio()
             << " | rango " << muestra->getRango() << "\n     " << muestra->descripcion() << '\n';
        delete muestra;
    }

    int tipo = Consola::leerEntero("  Tipo de defensa (0 = cancelar): ", 0, 3);
    if (tipo == 0) return;
    if (!jugador.puedePagar(costos[tipo])) {
        cout << Consola::color("  No tienes oro suficiente.\n", Consola::ROJO);
        Consola::esperarEnter();
        return;
    }

    int fila    = Consola::leerEntero("  Fila (0-" + to_string(FILAS - 1) + "): ", 0, FILAS - 1);
    int columna = Consola::leerEntero("  Columna (0-" + to_string(COLUMNAS - 1) + "): ", 0, COLUMNAS - 1);
    if (!mapa.esLibre(fila, columna)) {
        cout << Consola::color("  Esa casilla no esta libre (camino, torre u otra defensa).\n", Consola::ROJO);
        Consola::esperarEnter();
        return;
    }

    Defensa* nueva = crearDefensa(tipo, fila, columna);
    defensas.push_back(nueva);                   // Juego es el duenio (composicion)
    mapa.colocarDefensa(nueva);                  // el mapa solo la referencia (agregacion)
    jugador -= costos[tipo];                     // operator-= de Jugador
    defensasConstruidas++;
}

void Juego::mejorarDefensa() {
    if (defensas.empty()) {
        cout << "  Aun no tienes defensas.\n";
        Consola::esperarEnter();
        return;
    }
    listarDefensas();
    int indice = Consola::leerEntero("  Numero de la defensa a mejorar (0 = cancelar): ",
                                     0, static_cast<int>(defensas.size()));
    if (indice == 0) return;

    Defensa* d = defensas[indice - 1];
    if (!d->puedeMejorar()) {
        cout << Consola::color("  Esa defensa ya esta en el nivel maximo.\n", Consola::ROJO);
    } else if (!jugador.puedePagar(d->costoMejora())) {
        cout << Consola::color("  No tienes oro suficiente.\n", Consola::ROJO);
    } else {
        jugador -= d->costoMejora();             // se cobra ANTES de subir de nivel
        *d += 1;                                 // operator+= de Defensa
        cout << Consola::color("  Mejorada! ", Consola::VERDE) << *d << '\n';
    }
    Consola::esperarEnter();
}

void Juego::listarDefensas() const {
    cout << "\n  TUS DEFENSAS (" << defensas.size() << ")\n";
    if (defensas.empty()) cout << "  Ninguna todavia.\n";
    for (size_t i = 0; i < defensas.size(); i++) {
        cout << "  " << i + 1 << ". " << *defensas[i];          // operator<< de Defensa
        if (defensas[i]->puedeMejorar()) cout << " | Mejora: " << defensas[i]->costoMejora() << " oro";
        else cout << " | NIVEL MAXIMO";
        cout << '\n';
    }
}

void Juego::verEnemigos(const Oleada& oleada) const {
    cout << "\n  " << oleada << "\n\n";
    string tiposMostrados;
    for (int i = 0; i < oleada.getCantidad(); i++) {
        const Enemigo* e = oleada[i];                           // operator[] de Oleada
        if (tiposMostrados.find(e->simbolo()) != string::npos) continue;
        tiposMostrados += e->simbolo();
        cout << "  " << *e << '\n';                             // operator<< de Enemigo
    }
}

// =====================================================================
//  OLEADA: bucle de turnos
// =====================================================================
void Juego::ejecutarOleada(Oleada& oleada) {
    int eliminadosAntes = enemigosEliminados;
    int vidaAntes = torre.getVida();
    int turno = 0;

    while (!oleada.terminada() && !torre.estaDestruida() && turno < MAX_TURNOS) {
        turno++;
        oleada.liberarSiguiente();                               // 1. entra un enemigo

        for (Defensa* d : defensas) {                            // 2. atacan las defensas
            d->atacar(oleada.getEnemigos(), mapa);               //    (polimorfismo)
        }

        int bajas = 0;                                           // 3. botin por eliminados
        jugador += oleada.cobrarRecompensas(bajas);
        enemigosEliminados += bajas;

        oleada.avanzar();                                        // 4. se mueven (polimorfismo)
        int danio = oleada.procesarLlegadas(mapa, torre);        // 5. golpean la torre

        if (modoAnimado) {
            dibujarTurno(oleada, turno, danio);
            Consola::pausar(PAUSA_TURNO_MS);
        }
    }

    // Las defensas dejan de apuntar a enemigos que estan por destruirse.
    for (Defensa* d : defensas) d->olvidarObjetivo();

    int eliminados = enemigosEliminados - eliminadosAntes;
    cout << "\n  ------------------------------------------\n";
    if (torre.estaDestruida()) {
        cout << Consola::color("  LA TORRE HA CAIDO en la oleada " + to_string(oleada.getNumero()) + "!\n", Consola::ROJO);
    } else {
        cout << Consola::color("  OLEADA " + to_string(oleada.getNumero()) + " SUPERADA\n", Consola::VERDE);
    }
    cout << "  Enemigos eliminados: " << eliminados << "/" << oleada.getCantidad()
         << " | Danio recibido: " << vidaAntes - torre.getVida()
         << " | Turnos: " << turno << '\n'
         << "  " << torre << '\n';
}

void Juego::dibujarTurno(const Oleada& oleada, int turno, int danioTurno) const {
    Consola::limpiar();
    mostrarCabecera(oleada.getNumero());
    mapa.dibujar(oleada.getEnemigos());
    cout << "  Turno " << turno
         << " | En camino: " << oleada.contarEnCamino()
         << " | Por salir: " << oleada.contarPendientes() << '\n';

    // Vida de los enemigos en camino (maximo 8 para no llenar la pantalla)
    int mostrados = 0;
    cout << "  ";
    for (const Enemigo* e : oleada.getEnemigos()) {
        if (!e->estaEnCamino() || mostrados == 8) continue;
        cout << Consola::color(e->simbolo(), Consola::ROJO) << ":" << e->getVida() << "/" << e->getVidaMax() << "  ";
        mostrados++;
    }
    cout << '\n';
    if (danioTurno > 0) {
        cout << Consola::color("  La torre recibio " + to_string(danioTurno) + " de danio!", Consola::ROJO) << '\n';
    }
}

// =====================================================================
//  PANTALLAS
// =====================================================================
void Juego::mostrarCabecera(int numeroOleada) const {
    cout << Consola::color("  ===== DEFENSA DEL REINO =====", Consola::AMARILLO)
         << "   Oleada " << numeroOleada << "/" << TOTAL_OLEADAS << '\n'
         << "  " << jugador << "   " << torre << "\n\n";       // operator<< de Jugador y Torre
}

void Juego::mostrarResultado(bool victoria, bool rendido) const {
    cout << '\n';
    if (victoria) {
        cout << Consola::color(
            "  *********************************************\n"
            "  *   VICTORIA! La torre resistio 10 oleadas  *\n"
            "  *********************************************\n", Consola::VERDE);
    } else {
        cout << Consola::color(
            "  =============================================\n"
            "  =   DERROTA. Eldoria ha caido...            =\n"
            "  =============================================\n", Consola::ROJO);
        if (rendido) cout << "  (Te rendiste)\n";
    }
    cout << "  Oleadas superadas:    " << oleadasSuperadas << "/" << TOTAL_OLEADAS << '\n'
         << "  Enemigos eliminados:  " << enemigosEliminados << '\n'
         << "  Danio recibido:       " << torre.getDanioRecibido() << '\n'
         << "  Defensas construidas: " << defensasConstruidas << '\n'
         << "  Oro final:            " << jugador.getOro() << '\n';
}

void Juego::mostrarTitulo() const {
    cout << Consola::color(
        "\n"
        "        |>>>                        |>>>\n"
        "        |                           |\n"
        "    _  _|_  _                   _  _|_  _\n"
        "   | |_| |_| |                 | |_| |_| |\n"
        "    \\       /   _   _   _   _   \\       /\n"
        "     |     |_ _| |_| |_| |_| |_ |     |\n"
        "     |     |                    |     |\n"
        "     |     |  DEFENSA DEL REINO |     |\n"
        "     |_____|____________________|_____|\n\n", Consola::AMARILLO)
         << "     Eldoria esta bajo ataque. Protege la torre durante 10 oleadas.\n\n";
}

// =====================================================================
//  MEMORIA
// =====================================================================
Defensa* Juego::crearDefensa(int tipo, int fila, int columna) const {
    switch (tipo) {
        case 1:  return new Arquero(fila, columna);
        case 2:  return new Mago(fila, columna);
        case 3:  return new Canon(fila, columna);
        default: return nullptr;
    }
}

void Juego::liberarDefensas() {
    mapa.quitarDefensas();                       // primero se borran las referencias del mapa
    for (Defensa* d : defensas) delete d;        // luego se destruyen las defensas
    defensas.clear();
}
