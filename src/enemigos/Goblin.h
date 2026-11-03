#pragma once
#include "Enemigo.h"
#include <iostream>
using namespace std;


class Goblin : public Enemigo 
{
public:
    Goblin() : Enemigo("Goblin", 30, 2, 5, 10) {}

    char simbolo() const override 
    {
        return 'G';
    }


};