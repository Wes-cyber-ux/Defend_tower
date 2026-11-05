#pragma once
#include <iostream>
#include <vector>
#include "../enemigos/Enemigo.h"
#include "../enemigos/Goblin.h"
#include "../torre/Torre.h"
#include "../mapa/Mapa.h"
using namespace std;

class Oleada {
private:
    vector<Enemigo*> enemigos;
    int numOleada;
    
    void agregarEnemigo(Enemigo* e) {
        
        e->setPosicion(static_cast<int>(enemigos.size())*(- 2));
        enemigos.push_back(e);
        
    }


public:
    Oleada(int n) : numOleada(n) 
    {
        for (int i = 0; i < 3; ++i) 
            {
            agregarEnemigo(new Goblin());
            }
    }
    

    ~Oleada() {
        for (Enemigo* e : enemigos) {
            delete e;
        }
    }

    void avanzarEnemigos() 
    {
        for (Enemigo* e : enemigos) 
        {
            if (e->estaVivo())
            {
            e->avanzar();
            }
        }

    }   


    int eliminarMuertos(){
    
        int oro = 0;
        for (auto i = enemigos.end(); i != enemigos.begin();)
        {
            --i;
            if (!(*i)->estaVivo()) 
            {
                oro += (*i)->getRecompensa();
                delete *i;
                i = enemigos.erase(i);
            }
        }
        return oro;
    }

    int procesarLlegadas(Torre& torre, const Mapa& mapa){
        int danototal = 0;
        for(auto i = enemigos.end(); i != enemigos.begin();)
        {   
            --i;
            if ((*i)->estaVivo() && mapa.llegoAlFinal((*i)->getPosicion())) 
            {
                torre -= (*i)->getDanio();
                danototal += (*i)->getDanio();
                delete *i;
                i = enemigos.erase(i);
            }
        }
        return danototal;
    }

    bool terminada()const
    {
        for (const Enemigo* enemigo : enemigos) {
            if (enemigo->estaVivo()) {
                return false;
            }
        }
        return true;
    }

    vector<Enemigo*>& getEnemigos() {
        return enemigos;
    }


    int getNumOleada() const {
        return numOleada;
    }




};