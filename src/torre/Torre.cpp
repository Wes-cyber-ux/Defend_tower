#include "Torre.h"
#include "../consola/Consola.h"
#include <string>
using namespace std;

Torre::Torre(int vidaMax) : vida(vidaMax), vidaMax(vidaMax) {}

Torre& Torre::operator-=(int danio) {
    if (danio > 0) {
        vida -= danio;
        if (vida < 0) vida = 0;
    }
    return *this;
}

ostream& operator<<(ostream& os, const Torre& t) {
    const int ANCHO = 20;
    int llenas = t.vida * ANCHO / t.vidaMax;
    int codigo = (t.vida * 3 > t.vidaMax * 2) ? Consola::VERDE
               : (t.vida * 3 > t.vidaMax)     ? Consola::AMARILLO
                                              : Consola::ROJO;
    os << "Torre [" << Consola::color(string(llenas, '#'), codigo)
       << string(ANCHO - llenas, '-') << "] " << t.vida << "/" << t.vidaMax;
    return os;
}

void Torre::reiniciar() {
    vida = vidaMax;
}

bool Torre::estaDestruida() const {
    return vida <= 0;
}
