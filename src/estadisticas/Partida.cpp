#include "Partida.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;

Partida::Partida()
    : jugador(""), fecha(""), victoria(false), oleadasSuperadas(0), enemigosEliminados(0),
      danioRecibido(0), oroFinal(0), defensasConstruidas(0) {}

Partida::Partida(const string& jugador, bool victoria, int oleadasSuperadas,
                 int enemigosEliminados, int danioRecibido, int oroFinal, int defensasConstruidas)
    : jugador(jugador), victoria(victoria), oleadasSuperadas(oleadasSuperadas),
      enemigosEliminados(enemigosEliminados), danioRecibido(danioRecibido),
      oroFinal(oroFinal), defensasConstruidas(defensasConstruidas) {
    // '|' y ';' son separadores del archivo: no pueden aparecer dentro del nombre
    for (char& c : this->jugador) if (c == ';' || c == '|') c = ',';
    // maximo 20 caracteres para que las columnas del archivo queden alineadas
    if (this->jugador.size() > 20) this->jugador = this->jugador.substr(0, 20);

    // Fecha y hora actuales
    time_t ahora = time(nullptr);
    tm local{};
#ifdef _MSC_VER
    localtime_s(&local, &ahora);       // version segura que exige Visual Studio
#else
    local = *localtime(&ahora);        // GCC / MinGW (CLion, Dev-C++, Linux)
#endif
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &local);
    fecha = buffer;
}

int Partida::puntaje() const {
    return oleadasSuperadas * 100 + enemigosEliminados * 5
         + (victoria ? 500 : 0) - danioRecibido;
}

void Partida::mostrarFila(ostream& os) const {
    string nombre = jugador.size() > 14 ? jugador.substr(0, 14) : jugador;
    os << "  " << left << setw(17) << fecha
       << setw(15) << nombre
       << setw(9)  << (victoria ? "VICTORIA" : "DERROTA")
       << right << setw(7) << oleadasSuperadas
       << setw(10) << enemigosEliminados
       << setw(8)  << danioRecibido
       << setw(9)  << defensasConstruidas
       << setw(8)  << puntaje() << '\n';
}

bool Partida::operator<(const Partida& otra) const {
    return puntaje() < otra.puntaje();
}

// Formato de una linea (columnas alineadas y separadas por '|'):
//   jugador | fecha | resultado | oleadas | eliminados | danio | oro | defensas
// Los anchos coinciden con el encabezado que escribe Estadisticas::registrar.
ostream& operator<<(ostream& os, const Partida& p) {
    os << "  " << left << setw(20) << p.jugador
       << " | " << setw(16) << p.fecha
       << " | " << setw(9)  << (p.victoria ? "VICTORIA" : "DERROTA")
       << right
       << " | " << setw(7)  << p.oleadasSuperadas
       << " | " << setw(10) << p.enemigosEliminados
       << " | " << setw(5)  << p.danioRecibido
       << " | " << setw(5)  << p.oroFinal
       << " | " << setw(8)  << p.defensasConstruidas
       << left;
    return os;
}

// Quita espacios al inicio y al final de un campo
string Partida::recortar(const string& s) {
    size_t ini = s.find_first_not_of(" \t\r");
    if (ini == string::npos) return "";
    size_t fin = s.find_last_not_of(" \t\r");
    return s.substr(ini, fin - ini + 1);
}

istream& operator>>(istream& is, Partida& p) {
    string linea;
    if (!getline(is, linea)) return is;

    // Formato actual usa '|'; se acepta ';' para historiales antiguos
    char sep = (linea.find('|') != string::npos) ? '|' : ';';
    vector<string> campos;
    stringstream ss(linea);
    string campo;
    while (getline(ss, campo, sep)) campos.push_back(Partida::recortar(campo));

    if (campos.size() != 8) {                    // linea mal formada
        is.setstate(ios::failbit);
        return is;
    }
    try {
        p.jugador             = campos[0];
        p.fecha               = campos[1];
        p.victoria            = (campos[2] == "VICTORIA");
        p.oleadasSuperadas    = stoi(campos[3]);
        p.enemigosEliminados  = stoi(campos[4]);
        p.danioRecibido       = stoi(campos[5]);
        p.oroFinal            = stoi(campos[6]);
        p.defensasConstruidas = stoi(campos[7]);
    } catch (...) {                              // algun numero no era valido
        is.setstate(ios::failbit);
    }
    return is;
}
