#include "Mapa.h"
#include "../enemigos/Enemigo.h"
#include "../defensas/Defensa.h"
#include "../consola/Consola.h"
#include <iostream>
#include <iomanip>
#include <random>
using namespace std;

Mapa::Mapa(int filas, int columnas)
    : filas(filas), columnas(columnas), largoCamino(0) {
    celdas = new char*[filas];                  // arreglo de punteros (filas)
    for (int i = 0; i < filas; i++) {
        celdas[i] = new char[columnas];         // cada fila es un arreglo de char
    }
    // El camino nunca puede tener mas casillas que el mapa completo
    caminoFila = new int[filas * columnas];
    caminoCol  = new int[filas * columnas];
    reiniciar();
}

Mapa::~Mapa() {
    for (int i = 0; i < filas; i++) {
        delete[] celdas[i];
    }
    delete[] celdas;
    delete[] caminoFila;
    delete[] caminoCol;
    // No se hace delete de "defensas": pertenecen a Juego (agregacion).
}

void Mapa::reiniciar() {
    quitarDefensas();
    limpiarCeldas();
    generarCamino();
}

void Mapa::quitarDefensas() {
    defensas.clear();
}

int Mapa::aleatorio(int minimo, int maximo) {
    static random_device rd;
    static mt19937 generador(rd());
    uniform_int_distribution<> distribucion(minimo, maximo);
    return distribucion(generador);
}

void Mapa::limpiarCeldas() {
    for (int i = 0; i < filas; i++)
        for (int j = 0; j < columnas; j++)
            celdas[i][j] = VACIO;
}

void Mapa::agregarCelda(int f, int c) {
    if (f < 0 || f >= filas || c < 0 || c >= columnas) return;
    caminoFila[largoCamino] = f;
    caminoCol[largoCamino]  = c;
    celdas[f][c] = CAMINO;
    largoCamino++;
}

// direccion: 'D' = derecha, 'A' = arriba, 'B' = abajo
void Mapa::avanzarTramo(char direccion, int pasos) {
    for (int i = 0; i < pasos; i++) {
        int f = caminoFila[largoCamino - 1];
        int c = caminoCol[largoCamino - 1];
        if (direccion == 'D')      c++;
        else if (direccion == 'A') f--;
        else if (direccion == 'B') f++;
        agregarCelda(f, c);
    }
}

// Algoritmo: tramos horizontales hacia la derecha de 2 a 5 casillas,
// alternados con tramos verticales de 1 a 3 casillas. Como nunca retrocede,
// el camino no se cruza consigo mismo y siempre termina en la ultima columna.
void Mapa::generarCamino() {
    largoCamino = 0;
    int filaInicial = (filas > 2) ? aleatorio(1, filas - 2) : 0;
    agregarCelda(filaInicial, 0);

    while (caminoCol[largoCamino - 1] < columnas - 1) {
        int libresDerecha = columnas - 1 - caminoCol[largoCamino - 1];
        int pasos = aleatorio(2, 5);
        if (pasos > libresDerecha) pasos = libresDerecha;
        avanzarTramo('D', pasos);

        if (caminoCol[largoCamino - 1] >= columnas - 1) break;

        int f = caminoFila[largoCamino - 1];
        char direccion = (aleatorio(0, 1) == 0) ? 'A' : 'B';
        int libres = (direccion == 'A') ? f : filas - 1 - f;
        if (libres == 0) {                      // pegado al borde: ir al otro lado
            direccion = (direccion == 'A') ? 'B' : 'A';
            libres = (direccion == 'A') ? f : filas - 1 - f;
        }
        if (libres > 0) {
            int pasosV = aleatorio(1, 3);
            if (pasosV > libres) pasosV = libres;
            avanzarTramo(direccion, pasosV);
        }
    }
    celdas[caminoFila[0]][caminoCol[0]] = ENTRADA;
    celdas[caminoFila[largoCamino - 1]][caminoCol[largoCamino - 1]] = TORRE;
}

int Mapa::getFilaCamino(int pos) const {
    if (pos < 0 || pos >= largoCamino) return -1;
    return caminoFila[pos];
}

int Mapa::getColCamino(int pos) const {
    if (pos < 0 || pos >= largoCamino) return -1;
    return caminoCol[pos];
}

bool Mapa::llegoAlFinal(int pos) const {
    return pos >= largoCamino - 1;              // la ultima casilla es la torre
}

bool Mapa::esLibre(int f, int c) const {
    if (f < 0 || f >= filas || c < 0 || c >= columnas) return false;
    if (celdas[f][c] != VACIO) return false;    // camino, entrada o torre
    for (const Defensa* d : defensas) {
        if (d->getFila() == f && d->getColumna() == c) return false;
    }
    return true;
}

bool Mapa::colocarDefensa(Defensa* defensa) {
    if (defensa == nullptr || !esLibre(defensa->getFila(), defensa->getColumna())) return false;
    defensas.push_back(defensa);
    return true;
}

string Mapa::colorear(char simbolo) const {
    switch (simbolo) {
        case VACIO:   return Consola::color(simbolo, Consola::GRIS);
        case CAMINO:  return Consola::color(simbolo, Consola::AMARILLO);
        case TORRE:   return Consola::color(simbolo, Consola::VERDE);
        case ENTRADA: return Consola::color(simbolo, Consola::VERDE);
        case 'A':     return Consola::color(simbolo, Consola::CIAN);
        case 'M':     return Consola::color(simbolo, Consola::MAGENTA);
        case 'C':     return Consola::color(simbolo, Consola::AZUL);
        default:      return Consola::color(simbolo, Consola::ROJO);   // enemigos
    }
}

void Mapa::dibujar(const vector<Enemigo*>& enemigos) const {
    // 1) Copia del terreno (se dibuja sobre la copia para no ensuciar "celdas")
    vector<string> vista(filas);
    for (int i = 0; i < filas; i++) vista[i] = string(celdas[i], columnas);

    // 2) Defensas
    for (const Defensa* d : defensas) vista[d->getFila()][d->getColumna()] = d->simbolo();

    // 3) Enemigos que estan en el camino (polimorfismo: cada uno da su simbolo)
    for (const Enemigo* e : enemigos) {
        if (!e->estaEnCamino()) continue;
        int pos = e->getPosicion();
        if (pos < 0 || pos >= largoCamino) continue;
        vista[caminoFila[pos]][caminoCol[pos]] = e->simbolo();
    }

    // 4) Encabezado de columnas (decenas y unidades) para ubicar coordenadas
    cout << "     ";
    for (int j = 0; j < columnas; j++) cout << (j >= 10 ? char('0' + j / 10) : ' ') << ' ';
    cout << "\n     ";
    for (int j = 0; j < columnas; j++) cout << j % 10 << ' ';
    cout << '\n';

    for (int i = 0; i < filas; i++) {
        cout << "  " << setw(2) << i << " ";
        for (int j = 0; j < columnas; j++) cout << colorear(vista[i][j]) << ' ';
        cout << '\n';
    }
}

void Mapa::mostrarLeyenda() const {
    cout << "  Leyenda: " << colorear(ENTRADA) << " entrada  " << colorear(CAMINO) << " camino  "
         << colorear(TORRE) << " torre  " << colorear(VACIO) << " libre\n"
         << "  Defensas: " << colorear('A') << " Arquero  " << colorear('M') << " Mago  "
         << colorear('C') << " Canon   Enemigos: " << colorear('G') << " Goblin  "
         << colorear('O') << " Orco  " << colorear('W') << " Troll\n";
}
