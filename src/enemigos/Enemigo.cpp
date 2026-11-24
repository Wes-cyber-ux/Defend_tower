#include "Enemigo.h"
using namespace std;

Enemigo::Enemigo(const string& nombre, int vidaBase, int velocidadBase,
                 int danioBase, int recompensaBase, int nivel)
    : nombre(nombre), nivel(nivel), posicion(0), turnosCongelado(0),
      estado(EstadoEnemigo::EnEspera), recompensaCobrada(false) {
    // El nivel de la oleada aumenta resistencia, danio y velocidad.
    vidaMax    = vidaBase + vidaBase * 35 * (nivel - 1) / 100;   // +35% de vida por nivel
    vida       = vidaMax;
    danio      = danioBase + danioBase * (nivel - 1) / 4;        // +25% de danio por nivel
    velocidad  = velocidadBase + (nivel >= 7 ? 1 : 0);           // desde la oleada 7 son mas rapidos
    recompensa = recompensaBase + (nivel - 1);
}

// ---------------- Comportamiento por defecto ----------------

int Enemigo::danioEfectivo(int danioBruto) const {
    return danioBruto;                 // sin armadura
}

int Enemigo::pasosPorTurno() const {
    return velocidad;
}

void Enemigo::avanzar() {
    if (estado != EstadoEnemigo::EnCamino) return;
    if (turnosCongelado > 0) {         // congelado por un Mago: pierde el turno
        turnosCongelado--;
        return;
    }
    posicion += pasosPorTurno();       // llamada polimorfica
}

// ---------------- Operadores ----------------

// Sobrecarga de "-": aplica danio al enemigo. Se pide asi en el enunciado
// (seccion 3.3) y se usa como:  *enemigo - danio;
// Devuelve una referencia para poder encadenar:  *e - 10 - 5;
Enemigo& Enemigo::operator-(int danioRecibido) {
    if (estado != EstadoEnemigo::EnCamino || danioRecibido <= 0) return *this;
    vida -= danioEfectivo(danioRecibido);            // llamada polimorfica
    if (vida <= 0) {
        vida = 0;
        estado = EstadoEnemigo::Eliminado;
    }
    return *this;
}

bool Enemigo::operator>(const Enemigo& otro) const {
    return posicion > otro.posicion;
}

ostream& operator<<(ostream& os, const Enemigo& e) {
    os << e.simbolo() << " " << e.nombre
       << " Nv." << e.nivel
       << " | Vida: " << e.vida << "/" << e.vidaMax
       << " | Vel: " << e.velocidad
       << " | Danio a torre: " << e.danio
       << " | Oro: " << e.recompensa
       << " | " << e.habilidad();
    return os;
}

// ---------------- Estado ----------------

void Enemigo::entrarAlCamino() {
    if (estado == EstadoEnemigo::EnEspera) {
        estado = EstadoEnemigo::EnCamino;
        posicion = 0;
    }
}

void Enemigo::congelar(int turnos) {
    if (estado == EstadoEnemigo::EnCamino && turnos > turnosCongelado) {
        turnosCongelado = turnos;
    }
}

void Enemigo::llegarATorre() {
    if (estado == EstadoEnemigo::EnCamino) estado = EstadoEnemigo::LlegoATorre;
}

int Enemigo::cobrarRecompensa() {
    if (estado == EstadoEnemigo::Eliminado && !recompensaCobrada) {
        recompensaCobrada = true;
        return recompensa;
    }
    return 0;
}

bool Enemigo::estaVivo() const     { return vida > 0; }
bool Enemigo::estaEnCamino() const { return estado == EstadoEnemigo::EnCamino; }
bool Enemigo::estaEnEspera() const { return estado == EstadoEnemigo::EnEspera; }
bool Enemigo::haTerminado() const {
    return estado == EstadoEnemigo::Eliminado || estado == EstadoEnemigo::LlegoATorre;
}
