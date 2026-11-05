#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../enemigos/Enemigo.h"    
using namespace std;

class Mapa;

class Defensa {
protected:
    string nombre;
    int fila;
    int columna;
    int danio;
    int rango;
    int nivel;
    int costo;

public:
    Defensa(string a, int b, int c, int d, int e, int f, int g) : nombre(a), fila(b), columna(c), danio(d), rango(e), nivel(f), costo(g) {}
    
    virtual ~Defensa() {}

    virtual void atacar(vector<Enemigo*>& enemigos, const Mapa& mapa) = 0;

    virtual char simbolo() const = 0;


    Defensa& operator+=(int niveles) {
        nivel += niveles;
        danio += 10 * niveles;   
        rango += 1 * niveles;
        return *this;            
    }

   
    friend ostream& operator<<(ostream& os, const Defensa& d) {
        os << d.simbolo() << " " << d.nombre
           << " | Nv." << d.nivel
           << " | Danio: " << d.danio
           << " | Rango: " << d.rango
           << " | Pos: (" << d.fila << "," << d.columna << ")";
        return os;               
    }

    // Getters

    string getNombre() const {
        return nombre;
    }

    int getDanio() const {
        return danio;
    }

    int getRango() const {
        return rango;
    }

    int getNivel() const {
        return nivel;
    }

    int getCosto() const {
        return costo;
    }

    int getFila() const {
        return fila;
    }

    int getColumna() const {
        return columna;
    }


};