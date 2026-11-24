#include "Estadisticas.h"
#include "../consola/Consola.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

Estadisticas::Estadisticas(const string& rutaArchivo) : rutaArchivo(rutaArchivo) {}

void Estadisticas::cargar() {
    partidas.clear();
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) return;             // primera ejecucion: aun no existe

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;   // comentarios / lineas vacias
        stringstream ss(linea);
        Partida p;
        if (ss >> p) partidas.push_back(p);     // operator>> de Partida
    }
}

bool Estadisticas::registrar(const Partida& partida) {
    bool esNuevo = !ifstream(rutaArchivo).good();
    ofstream archivo(rutaArchivo, ios::app);    // app: agrega al final sin borrar
    if (!archivo.is_open()) {
        cout << Consola::color("  No se pudo abrir " + rutaArchivo + " para guardar.\n", Consola::ROJO);
        return false;
    }
    if (esNuevo) {                              // encabezado de la tabla (solo la primera vez)
        const string linea = "# " + string(101, '=');
        archivo << linea << '\n'
                << "#                              HISTORIAL DE PARTIDAS - DEFENSA DEL REINO\n"
                << linea << '\n'
                << "# Jugador              | Fecha            | Resultado | Oleadas | Eliminados | Danio |   Oro | Defensas\n"
                << "# ---------------------+------------------+-----------+---------+------------+-------+-------+---------\n";
    }
    archivo << partida << '\n';                 // operator<< de Partida (columnas de ancho fijo)
    partidas.push_back(partida);
    return true;
}

void Estadisticas::mostrar() const {
    cout << Consola::color("\n  ===================== HISTORIAL DE PARTIDAS =====================\n\n", Consola::AMARILLO);
    if (partidas.empty()) {
        cout << "  Aun no hay partidas registradas. Juega una!\n";
        return;
    }

    cout << "  " << left << setw(17) << "Fecha" << setw(15) << "Jugador" << setw(9) << "Result."
         << right << setw(7) << "Oleada" << setw(10) << "Elimin." << setw(8) << "Danio"
         << setw(9) << "Defens." << setw(8) << "Puntos" << '\n'
         << "  " << string(83, '-') << '\n';
    for (const Partida& p : partidas) p.mostrarFila(cout);

    // Resumen general
    int victorias = 0, totalEliminados = 0, totalDanio = 0, mejorOleada = 0;
    for (const Partida& p : partidas) {
        if (p.esVictoria()) victorias++;
        totalEliminados += p.getEnemigosEliminados();
        totalDanio += p.getDanioRecibido();
        mejorOleada = max(mejorOleada, p.getOleadasSuperadas());
    }
    int n = getCantidad();
    cout << "\n  Partidas jugadas: " << n
         << " | Victorias: " << victorias << " (" << victorias * 100 / n << "%)"
         << " | Enemigos eliminados: " << totalEliminados
         << "\n  Mejor resultado: " << mejorOleada << " oleadas"
         << " | Danio promedio recibido: " << totalDanio / n << '\n';

    // Ranking: se ordena una copia usando operator< de Partida (de mayor a menor)
    vector<Partida> ranking = partidas;
    sort(ranking.begin(), ranking.end(),
         [](const Partida& a, const Partida& b) { return b < a; });
    cout << Consola::color("\n  TOP 3 COMANDANTES\n", Consola::AMARILLO);
    for (int i = 0; i < 3 && i < n; i++) {
        cout << "  " << i + 1 << ". " << ranking[i].getJugador()
             << " - " << ranking[i].puntaje() << " puntos\n";
    }
}
