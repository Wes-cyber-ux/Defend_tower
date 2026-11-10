#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "../enemigos/Enemigo.h"
using namespace std;

class Mapa {
private:
    static constexpr char VACIO  = '.';
    static constexpr char CAMINO = '#';
    static constexpr char TORRE  = 'T';

    int filas;
    int columnas;
    char** celdas;        
    int* caminoFila;      
    int* caminoCol;       
    int largoCamino;      

    
    int aleatorio(int min, int max) {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    
    void agregarCelda(int f, int c) {
        if (f < 0 || f >= filas || c < 0 || c >= columnas) return;
        caminoFila[largoCamino] = f;
        caminoCol[largoCamino]  = c;
        celdas[f][c] = CAMINO;
        largoCamino++;
    }

    
    void avanzarTramo(char dir, int pasos) {
        for (int i = 0; i < pasos; i++) {
            int f = caminoFila[largoCamino - 1];
            int c = caminoCol[largoCamino - 1];
            if (dir == 'D')      c++;
            else if (dir == 'A') f--;
            else if (dir == 'B') f++;
            agregarCelda(f, c);
        }
    }

   
    void generarCamino() {
        largoCamino = 0;

        int filaIni = (filas > 2) ? aleatorio(1, filas - 2) : 0;
        agregarCelda(filaIni, 0);

        while (caminoCol[largoCamino - 1] < columnas - 1) {

           
            int libresDer = columnas - 1 - caminoCol[largoCamino - 1];
            int pasos = aleatorio(2, 5);
            if (pasos > libresDer) pasos = libresDer;
            if (pasos < 1) pasos = 1;
            avanzarTramo('D', pasos);

            if (caminoCol[largoCamino - 1] >= columnas - 1) break;

           
            int f = caminoFila[largoCamino - 1];
            char dir = (aleatorio(0, 1) == 0) ? 'A' : 'B';
            int libres = (dir == 'A') ? f : filas - 1 - f;

            if (libres == 0) {                     
                dir = (dir == 'A') ? 'B' : 'A';
                libres = (dir == 'A') ? f : filas - 1 - f;
            }
            if (libres > 0) {
                int pasosV = aleatorio(1, 3);
                if (pasosV > libres) pasosV = libres;
                avanzarTramo(dir, pasosV);
            }
        }

       
        celdas[caminoFila[largoCamino - 1]][caminoCol[largoCamino - 1]] = TORRE;
    }

public:
    Mapa(int f, int c) : filas(f), columnas(c), largoCamino(0) {
        celdas = new char*[filas];
        for (int i = 0; i < filas; i++) {
            celdas[i] = new char[columnas];
            for (int j = 0; j < columnas; j++) {
                celdas[i][j] = VACIO;
            }
        }
        caminoFila = new int[filas * columnas];
        caminoCol  = new int[filas * columnas];

        generarCamino();
    }

    ~Mapa() {
        for (int i = 0; i < filas; i++) {
            delete[] celdas[i];
        }
        delete[] celdas;
        delete[] caminoFila;
        delete[] caminoCol;
    }

    int getFilas() const       { return filas; }
    int getColumnas() const    { return columnas; }
    int getLargoCamino() const { return largoCamino; }

    bool llegoAlFinal(int pos) const {
        return pos >= largoCamino;
    }

    int getFilaCamino(int pos) const {
        if (pos < 0 || pos >= largoCamino) return -1;
        return caminoFila[pos];
    }

    int getColCamino(int pos) const {
        if (pos < 0 || pos >= largoCamino) return -1;
        return caminoCol[pos];
    }

   
    bool esLibre(int f, int c) const {
        if (f < 0 || f >= filas || c < 0 || c >= columnas) return false;
        return celdas[f][c] == VACIO;
    }

    
    bool colocarDefensa(int f, int c, char simbolo) {
        if (!esLibre(f, c)) return false;
        celdas[f][c] = simbolo;
        return true;
    }

    
    void dibujar(const vector<Enemigo*>& enemigos) const {
        char** vista = new char*[filas];
        for (int i = 0; i < filas; i++) {
            vista[i] = new char[static_cast<unsigned>(columnas)];
            for (int j = 0; j < columnas; j++) {
                vista[i][j] = celdas[i][j];
            }
        }

        for (size_t k = 0; k < enemigos.size(); k++) {
            if (!enemigos[k]->estaVivo()) continue;
            int pos = enemigos[k]->getPosicion();
            if (pos < 0 || pos >= largoCamino) continue;
            vista[caminoFila[pos]][caminoCol[pos]] = enemigos[k]->simbolo();
        }

        // numeros de columna (decena arriba, unidad abajo) y de fila para ubicar defensas
        cout << "   ";
        for (int j = 0; j < columnas; j++) cout << j / 10 << ' ';
        cout << endl;
        cout << "   ";
        for (int j = 0; j < columnas; j++) cout << j % 10 << ' ';
        cout << endl;
        for (int i = 0; i < filas; i++) {
            cout << i << (i < 10 ? "  " : " ");
            for (int j = 0; j < columnas; j++) {
                cout << vista[i][j] << ' ';
            }
            cout << endl;
        }

        for (int i = 0; i < filas; i++) delete[] vista[i];
        delete[] vista;
    }
};
