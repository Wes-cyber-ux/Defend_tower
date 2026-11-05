#pragma once
#include <iostream>
#include <string>
using namespace std;

class Enemigo {
protected:
    string nombre;
    int vida;
    int velocidad;
    int danio;
    int recompensa;
    int posicion = 0;


public:
    //orden de los parametros: nombre, vida, velocidad, danio, recompensa
    Enemigo( string a, int b, int c, int d, int e) : nombre(a), vida(b), velocidad(c), danio(d), recompensa(e) {}

    virtual ~Enemigo() {}

    void avanzar() {
        posicion += velocidad;
    }

    bool estaVivo() const {
        if (vida <= 0) {
            return false;
        }
        return true;
    }

    Enemigo& operator-(int dano) {
        vida -= dano;
        if (vida < 0) {
            vida = 0;
        }
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Enemigo& e) {
        os << e.simbolo() << " " << e.nombre
           << " | Vida: " << e.vida;
        return os;               
    }


    // Getters

    string getNombre() const {
        return nombre;
    }

    int getVida() const {
        return vida;
    }

    int getVelocidad() const {
        return velocidad;
    }

    int getDanio() const {
        return danio;
    }

    int getRecompensa() const {
        return recompensa;
    }

    int getPosicion() const {
        return posicion;
    }
    //setter

    void setPosicion(int p){
        posicion = p;
    }

    virtual char simbolo() const = 0;

};
