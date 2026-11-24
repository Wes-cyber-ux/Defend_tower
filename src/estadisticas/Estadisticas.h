#pragma once
#include <string>
#include <vector>
#include "Partida.h"

// Historial de partidas: lo LEE del archivo al iniciar y AGREGA una linea
// por cada partida terminada.
class Estadisticas {
private:
    std::string rutaArchivo;
    std::vector<Partida> partidas;      // COMPOSICION: objetos Partida por valor

public:
    explicit Estadisticas(const std::string& rutaArchivo);

    void cargar();                          // lectura del archivo
    bool registrar(const Partida& partida); // escritura (modo append)
    void mostrar() const;

    int getCantidad() const { return static_cast<int>(partidas.size()); }
};
