#pragma once
#include <iostream>
#include <string>
#include <vector>

class Enemigo;   // declaraciones anticipadas: evitan inclusiones circulares
class Mapa;

// Clase base ABSTRACTA de las defensas.
class Defensa {
protected:
    std::string nombre;
    int fila;
    int columna;
    int danio;
    int rango;              // distancia Manhattan maxima de ataque
    int nivel;
    int costo;              // precio de construccion
    Enemigo* objetivo;      // ASOCIACION: enemigo al que esta apuntando (no le pertenece)

    bool enRango(const Enemigo& e, const Mapa& mapa) const;
    // Enemigos en el camino y dentro del rango, ordenados del mas adelantado al menos.
    std::vector<Enemigo*> enemigosEnRango(const std::vector<Enemigo*>& enemigos,
                                          const Mapa& mapa) const;

public:
    static constexpr int NIVEL_MAXIMO = 4;

    Defensa(const std::string& nombre, int fila, int columna, int danio, int rango, int costo);
    virtual ~Defensa() = default;

    // ---- Polimorfismo: cada tipo ataca de forma distinta ----
    // Devuelve cuantos enemigos golpeo en este turno.
    virtual int atacar(std::vector<Enemigo*>& enemigos, const Mapa& mapa) = 0;
    virtual char simbolo() const = 0;
    virtual std::string descripcion() const = 0;

    // ---- Sobrecarga de operadores ----
    Defensa& operator+=(int niveles);       // mejora:  *defensa += 1;
    friend std::ostream& operator<<(std::ostream& os, const Defensa& d);

    int costoMejora() const;
    bool puedeMejorar() const;
    void olvidarObjetivo();

    std::string getNombre() const { return nombre; }
    int getFila() const     { return fila; }
    int getColumna() const  { return columna; }
    int getDanio() const    { return danio; }
    int getRango() const    { return rango; }
    int getNivel() const    { return nivel; }
    int getCosto() const    { return costo; }
};
