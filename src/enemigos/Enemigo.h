#pragma once
#include <iostream>
#include <string>

// Estados por los que pasa un enemigo durante una oleada
enum class EstadoEnemigo { EnEspera, EnCamino, Eliminado, LlegoATorre };

// Clase base ABSTRACTA de todos los enemigos (no se puede instanciar).
class Enemigo {
protected:
    std::string nombre;
    int nivel;              // nivel = numero de oleada (1..10)
    int vida;
    int vidaMax;
    int velocidad;          // casillas que avanza por turno
    int danio;              // danio que causa a la torre al llegar
    int recompensa;         // oro que gana el jugador al eliminarlo
    int posicion;           // indice de la casilla del camino donde esta
    int turnosCongelado;    // turnos que no se movera (efecto del Mago)
    EstadoEnemigo estado;
    bool recompensaCobrada;

    // Puntos de extension que cada subclase puede redefinir (polimorfismo)
    virtual int danioEfectivo(int danioBruto) const;   // Orco: armadura
    virtual int pasosPorTurno() const;                 // Goblin: huida

public:
    // Los valores "base" corresponden al nivel 1; el constructor los escala segun el nivel.
    Enemigo(const std::string& nombre, int vidaBase, int velocidadBase,
            int danioBase, int recompensaBase, int nivel);
    virtual ~Enemigo() = default;

    // ---- Comportamiento polimorfico ----
    virtual void avanzar();                          // Troll: se regenera al avanzar
    virtual char simbolo() const = 0;                // funcion virtual pura
    virtual std::string habilidad() const = 0;       // funcion virtual pura

    // ---- Sobrecarga de operadores ----
    Enemigo& operator-(int danioRecibido);           // aplica danio:   *enemigo - 15;
    bool operator>(const Enemigo& otro) const;       // esta mas adelantado en el camino
    friend std::ostream& operator<<(std::ostream& os, const Enemigo& e);

    // ---- Cambios de estado ----
    void entrarAlCamino();
    void congelar(int turnos);
    void llegarATorre();
    int cobrarRecompensa();      // devuelve el oro una sola vez si fue eliminado

    // ---- Consultas ----
    bool estaVivo() const;
    bool estaEnCamino() const;
    bool estaEnEspera() const;
    bool haTerminado() const;    // eliminado o ya llego a la torre

    std::string getNombre() const { return nombre; }
    int getNivel() const          { return nivel; }
    int getVida() const           { return vida; }
    int getVidaMax() const        { return vidaMax; }
    int getVelocidad() const      { return velocidad; }
    int getDanio() const          { return danio; }
    int getRecompensa() const     { return recompensa; }
    int getPosicion() const       { return posicion; }
};
