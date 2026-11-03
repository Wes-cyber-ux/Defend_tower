#pragma once
#include "Enemigo.h"
#include <iostream>
using namespace std;

class Orco : public Enemigo 
{   
    public:
    Orco() : Enemigo("Orco", 60, 1, 10, 40) {}

    char simbolo() const override 
    {
        return 'O';
    }




};