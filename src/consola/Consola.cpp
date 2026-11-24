#include "Consola.h"
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#else
    #include <chrono>
    #include <thread>
#endif

using namespace std;

bool Consola::coloresActivos = true;

void Consola::inicializar() {
#ifdef _WIN32
    // En la consola clasica de Windows hay que activar las secuencias ANSI.
    HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(salida, &modo)) {
        coloresActivos = SetConsoleMode(salida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
    }
    // Si GetConsoleMode falla, la salida va a un IDE (CLion, VS Code) que ya entiende ANSI.
#endif
}

void Consola::limpiar() {
    if (coloresActivos) {
        cout << "\033[2J\033[H" << flush;
    } else {
        for (int i = 0; i < 40; i++) cout << '\n';
    }
}

void Consola::pausar(int milisegundos) {
    cout << flush;
#ifdef _WIN32
    Sleep(milisegundos);
#else
    this_thread::sleep_for(chrono::milliseconds(milisegundos));
#endif
}

void Consola::esperarEnter() {
    cout << "\n  Presiona ENTER para continuar...";
    string basura;
    if (!getline(cin, basura)) {
        cout << "\n  Entrada finalizada.\n";
        exit(0);
    }
}

int Consola::leerEntero(const string& mensaje, int minimo, int maximo) {
    while (true) {
        cout << mensaje;
        string linea;
        if (!getline(cin, linea)) {          // fin de la entrada (Ctrl+Z / Ctrl+D)
            cout << "\n  Entrada finalizada.\n";
            exit(0);
        }
        try {
            size_t usados = 0;
            int valor = stoi(linea, &usados);
            // se aceptan espacios al final, pero no letras ("12abc")
            bool soloEspacios = linea.find_first_not_of(" \t\r", usados) == string::npos;
            if (soloEspacios && valor >= minimo && valor <= maximo) return valor;
        } catch (...) {
            // stoi lanza excepcion si el texto no es un numero
        }
        cout << color("  Valor invalido. Ingresa un numero entre ", ROJO)
             << minimo << " y " << maximo << ".\n";
    }
}

bool Consola::leerSiNo(const string& mensaje) {
    while (true) {
        string respuesta = leerTexto(mensaje);
        if (respuesta == "s" || respuesta == "S") return true;
        if (respuesta == "n" || respuesta == "N") return false;
        cout << color("  Responde s o n.\n", ROJO);
    }
}

string Consola::leerTexto(const string& mensaje) {
    cout << mensaje;
    string linea;
    if (!getline(cin, linea)) {
        cout << "\n  Entrada finalizada.\n";
        exit(0);
    }
    // quitar espacios y retornos de carro de los extremos
    size_t ini = linea.find_first_not_of(" \t\r");
    size_t fin = linea.find_last_not_of(" \t\r");
    if (ini == string::npos) return "";
    return linea.substr(ini, fin - ini + 1);
}

string Consola::color(const string& texto, int codigo) {
    if (!coloresActivos) return texto;
    return "\033[" + to_string(codigo) + "m" + texto + "\033[0m";
}

string Consola::color(char caracter, int codigo) {
    return color(string(1, caracter), codigo);
}
