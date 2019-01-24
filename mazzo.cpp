#include "mazzo.h"

#include "lettera.h"
#include <stdio.h>
#include <stdlib.h> //per il random
#include <time.h>   //per il seed random

int tot = 130;     //totale lettere disponibili
int maxr = 22;     //intervallo generazione numero random (che si restringe)
bool setSeed = false;

Lettera mazzo[22] = {  Lettera('A', 12, 1),
                       Lettera('B', 4, 4),
                       Lettera('C', 7, 1),
                       Lettera('D', 4, 4),
                       Lettera('E', 12, 1),
                       Lettera('F', 4, 4),
                       Lettera('G', 4, 4),
                       Lettera('H', 2, 8),
                       Lettera('I', 12, 1),
                       Lettera('L', 6, 2),
                       Lettera('M', 6, 2),
                       Lettera('N', 6, 2),
                       Lettera('O', 12, 1),
                       Lettera('P', 4, 3),
                       Lettera('Q', 2, 10),
                       Lettera('R', 7, 1),
                       Lettera('S', 7, 1),
                       Lettera('T', 7, 1),
                       Lettera('U', 4, 4),
                       Lettera('V', 4, 4),
                       Lettera('Z', 2, 8),
                       Lettera('*', 2, 0)};  //il punteggio del jolly viene assegnato in base alla lettera che sostituisce (IMPLEMENTA)


//--------------------------
//FUNZIONI
Lettera estrazione(){
    if(setSeed==false){
    srand(time(NULL));
    setSeed=true;
    }
    if(tot!=0){
        int rnd = rand()%maxr; //genera numero random da 0 a MAXr-1
        if (!(mazzo[rnd].isFinish())){
            mazzo[rnd].decrementa();
            return mazzo[rnd];
        }
        else{
            //scambio la lettera che è finita con quella in ultima posizione dell'intervallo random (maxr-1)
            Lettera* appoggio = new Lettera ();

            *appoggio = mazzo[rnd];
            mazzo[rnd] = mazzo[maxr-1];
            mazzo[maxr-1] = *appoggio;

            maxr--; //riduco il campo di estrazione
            tot--;  //aggiurno il numero di lettere
            delete appoggio;
            appoggio = nullptr;
            return estrazione();
        }
    }
    else{
        return Lettera();
    }
}
