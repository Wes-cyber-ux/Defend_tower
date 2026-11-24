#pragma once
#include <iostream>
#include <string>

// Registro con el resultado de una partida (una linea del archivo de historial).
class Partida {
private:
    std::string jugador;
    std::string fecha;
    bool victoria;
    int oleadasSuperadas;
    int enemigosEliminados;
    int danioRecibido;
    int oroFinal;
    int defensasConstruidas;

    static std::string recortar(const std::string& s);   // quita espacios de un campo leido

public:
    Partida();
    Partida(const std::string& jugador, bool victoria, int oleadasSuperadas,
            int enemigosEliminados, int danioRecibido, int oroFinal, int defensasConstruidas);

    int puntaje() const;
    void mostrarFila(std::ostream& os) const;        // fila de la tabla de estadisticas

    // ---- Sobrecarga de operadores ----
    bool operator<(const Partida& otra) const;       // compara por puntaje (ranking)
    // Escritura / lectura en el ARCHIVO (formato separado por ';')
    friend std::ostream& operator<<(std::ostream& os, const Partida& p);
    friend std::istream& operator>>(std::istream& is, Partida& p);

    std::string getJugador() const   { return jugador; }
    bool esVictoria() const          { return victoria; }
    int getOleadasSuperadas() const  { return oleadasSuperadas; }
    int getEnemigosEliminados() const{ return enemigosEliminados; }
    int getDanioRecibido() const     { return danioRecibido; }
};
