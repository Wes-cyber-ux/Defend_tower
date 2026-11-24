#pragma once
#include <string>

// Utilidades de entrada/salida por consola.
// Todos sus metodos son estaticos: agrupa funciones de apoyo dentro de una
// clase para cumplir la regla "no debe haber funciones sueltas".
class Consola {
private:
    static bool coloresActivos;

public:
    // Codigos de color ANSI
    static constexpr int ROJO     = 31;
    static constexpr int VERDE    = 32;
    static constexpr int AMARILLO = 33;
    static constexpr int AZUL     = 34;
    static constexpr int MAGENTA  = 35;
    static constexpr int CIAN     = 36;
    static constexpr int GRIS     = 90;

    static void inicializar();                       // activa colores ANSI (tambien en Windows)
    static void limpiar();                           // limpia la pantalla
    static void pausar(int milisegundos);            // detiene la ejecucion un momento
    static void esperarEnter();                      // "Presiona ENTER para continuar"
    static int  leerEntero(const std::string& mensaje, int minimo, int maximo);
    static bool leerSiNo(const std::string& mensaje);
    static std::string leerTexto(const std::string& mensaje);
    static std::string color(const std::string& texto, int codigo);
    static std::string color(char caracter, int codigo);
};
