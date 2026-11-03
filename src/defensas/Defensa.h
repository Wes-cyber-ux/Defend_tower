#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "../enemigos/Enemigo.h"
#include "../mapa/Mapa.h"
using namespace std;

class Defensa {
protected:
    string nombre;
    int fila;
    int columna;
    int danio;
    int rango;
    int nivel;
    int costo;

    // Distancia Manhattan entre la defensa y el enemigo (si esta en el camino)
    bool enRango(const Enemigo* e, const Mapa& mapa) const {
        int pos = e->getPosicion();
        int fe = mapa.getFilaCamino(pos);
        int ce = mapa.getColCamino(pos);
        if (fe == -1 || ce == -1) return false;
        return abs(fe - fila) + abs(ce - columna) <= rango;
    }

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