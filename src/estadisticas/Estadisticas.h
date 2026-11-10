#pragma once
#include <iostream>
using namespace std;

class Estadisticas 
{
    private:
    int oleadas_completas;
    int enemigos_eliminados;
    int oro_total;
    int dano_total;
    int turnos_jugados;


    public:
    // todo empieza en 0
    Estadisticas() : oleadas_completas(0), enemigos_eliminados(0), oro_total(0),
    dano_total(0), turnos_jugados(0) {}

    ~Estadisticas() {}

    void Registrar_Oro(int cantidad) {
        oro_total += cantidad;
    }

    void Registrar_Danio(int danio){
        dano_total += danio;
    }

    void Registrar_Eliminados(int cantidad) {
        enemigos_eliminados += cantidad;
    }

    void Registrar_Oleada() {
        oleadas_completas++;
    }

    void Registrar_Turno() {
        turnos_jugados++;
    }

    friend ostream& operator<<(ostream& os, const Estadisticas& estadisticas) {
        os << "Oleadas completas: " << estadisticas.oleadas_completas
           << ", enemigos eliminados: " << estadisticas.enemigos_eliminados
           << ", oro total: " << estadisticas.oro_total
           << ", dano total: " << estadisticas.dano_total
           << ", turnos jugados: " << estadisticas.turnos_jugados;
        return os;
    }

    

};