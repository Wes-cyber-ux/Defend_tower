#pragma once
#include <string>
#include <vector>

class Enemigo;
class Defensa;

// Tablero del juego: una MATRIZ dinamica de caracteres con un camino
// generado al azar desde la entrada (columna 0) hasta la torre (ultima columna).
class Mapa {
private:
    static constexpr char VACIO   = '.';
    static constexpr char CAMINO  = '#';
    static constexpr char TORRE   = 'T';
    static constexpr char ENTRADA = 'E';

    int filas;
    int columnas;
    char** celdas;              // matriz dinamica filas x columnas (solo el terreno)
    int* caminoFila;            // arreglos dinamicos: coordenadas de cada casilla del camino
    int* caminoCol;
    int largoCamino;
    std::vector<Defensa*> defensas;   // AGREGACION: las conoce pero NO las destruye

    int aleatorio(int minimo, int maximo);
    void limpiarCeldas();
    void agregarCelda(int f, int c);
    void avanzarTramo(char direccion, int pasos);
    void generarCamino();
    std::string colorear(char simbolo) const;

public:
    Mapa(int filas, int columnas);
    ~Mapa();

    // Maneja memoria dinamica: se prohibe copiarlo para evitar doble delete (regla de tres).
    Mapa(const Mapa&) = delete;
    Mapa& operator=(const Mapa&) = delete;

    void reiniciar();                           // nuevo camino y sin defensas
    void quitarDefensas();                      // olvida las defensas (no las destruye)

    int getFilas() const       { return filas; }
    int getColumnas() const    { return columnas; }
    int getLargoCamino() const { return largoCamino; }
    int getFilaCamino(int pos) const;
    int getColCamino(int pos) const;
    bool llegoAlFinal(int pos) const;           // la posicion ya alcanzo la torre

    bool esLibre(int f, int c) const;
    bool colocarDefensa(Defensa* defensa);

    void dibujar(const std::vector<Enemigo*>& enemigos) const;
    void mostrarLeyenda() const;
};
