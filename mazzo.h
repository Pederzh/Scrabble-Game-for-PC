#ifndef MAZZO_H
#define MAZZO_H

#include "lettera.h"
#include <stdio.h>
#include <stdlib.h> //per il random
#include <time.h>   //per il seed random
#include <random>

const int MAXl = 22; //numero massimo di elementi mazzo di lettere
extern Lettera mazzo[22];

//-------------------------------------
//FUNZIONI
Lettera estrazione();
#endif // MAZZO_H
