#pragma once
#include "Enemigo.h"

class Troll : public Enemigo 
{
public:
    Troll() : Enemigo("WoodTroll", 120, 1, 25, 40) {}

    char simbolo() const override 
    {
        return 'W';
    }
};