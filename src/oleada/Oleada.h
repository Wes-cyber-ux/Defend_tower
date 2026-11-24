#pragma once
#include <iostream>
#include <vector>

class Enemigo;
class Mapa;
class Torre;

// Una oleada de enemigos. Los CREA y los DESTRUYE (composicion).
// Su numero define cuantos enemigos trae y su nivel.
class Oleada {
private:
    int numero;
    int cantGoblins;
    int cantOrcos;
    int cantTrolls;
    std::vector<Enemigo*> enemigos;     // punteros a la clase base -> polimorfismo

    void generarEnemigos();

public:
    explicit Oleada(int numero);
    ~Oleada();

    // Es duena de memoria dinamica: no se permite copiarla (regla de tres).
    Oleada(const Oleada&) = delete;
    Oleada& operator=(const Oleada&) = delete;

    void liberarSiguiente();                         // un enemigo nuevo entra al camino
    void avanzar();                                  // todos los enemigos se mueven
    int procesarLlegadas(const Mapa& mapa, Torre& torre);  // devuelve el danio causado
    int cobrarRecompensas(int& bajas);               // oro de los recien eliminados
    bool terminada() const;

    int contarEnCamino() const;
    int contarPendientes() const;
    int getNumero() const   { return numero; }
    int getCantidad() const { return static_cast<int>(enemigos.size()); }

    std::vector<Enemigo*>& getEnemigos()             { return enemigos; }
    const std::vector<Enemigo*>& getEnemigos() const { return enemigos; }

    Enemigo* operator[](int indice) const;           // acceso:  oleada[0]
    friend std::ostream& operator<<(std::ostream& os, const Oleada& o);   // resumen
};
