#pragma once
#include <string>
#include <vector>
#include "../mapa/Mapa.h"
#include "../torre/Torre.h"
#include "../jugador/Jugador.h"
#include "../estadisticas/Estadisticas.h"

class Defensa;
class Oleada;

// Controla el programa completo: menu principal, partidas, oleadas y guardado.
class Juego {
private:
    static constexpr int FILAS          = 10;
    static constexpr int COLUMNAS       = 24;
    static constexpr int TOTAL_OLEADAS  = 10;
    static constexpr int VIDA_TORRE     = 200;
    static constexpr int ORO_INICIAL    = 200;
    static constexpr int PAUSA_TURNO_MS = 220;
    static constexpr int MAX_TURNOS     = 1000;   // proteccion contra bucles infinitos

    // ---- COMPOSICION: estos objetos viven y mueren con Juego ----
    Mapa mapa;
    Torre torre;
    Jugador jugador;
    Estadisticas estadisticas;
    std::vector<Defensa*> defensas;   // Juego las crea con new y las destruye con delete

    // ---- Datos de la partida en curso ----
    int oleadasSuperadas;
    int enemigosEliminados;
    int defensasConstruidas;
    bool modoAnimado;

    void jugar();
    void reiniciarPartida(const std::string& nombre);
    bool faseDePreparacion(const Oleada& oleada);   // false si el jugador se rinde
    void colocarDefensa();
    void mejorarDefensa();
    void listarDefensas() const;
    void verEnemigos(const Oleada& oleada) const;
    void ejecutarOleada(Oleada& oleada);
    void dibujarTurno(const Oleada& oleada, int turno, int danioTurno) const;
    void mostrarCabecera(int numeroOleada) const;
    void mostrarResultado(bool victoria, bool rendido) const;
    void mostrarTitulo() const;
    void liberarDefensas();
    Defensa* crearDefensa(int tipo, int fila, int columna) const;   // fabrica de defensas

public:
    Juego();
    ~Juego();

    // Juego es duenio de punteros (defensas): no se permite copiarlo.
    Juego(const Juego&) = delete;
    Juego& operator=(const Juego&) = delete;

    void menu();
};
