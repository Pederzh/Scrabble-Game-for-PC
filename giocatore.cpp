#include "giocatore.h"
#include "mazzo.h"
#include "lettera.h"

Giocatore::Giocatore(int numero)
{

    nome[0] = 'G';
    nome[1] = 'i';
    nome[2] = 'o';
    nome[3] = 'c';
    nome[4] = 'a';
    nome[5] = 't';
    nome[6] = 'o';
    nome[7] = 'r';
    nome[8] = 'e';
    nome[9] = '.';
    nome[10] = (char)numero;
    _punteggio = 0;

    for (int i=0; i<8; ++i){
    lettere[i] = new Lettera();
    *lettere[i] = estrazione();
    }
}

Giocatore::~Giocatore(){
    //DISTRUTTORE
    delete[] lettere;
}
void Giocatore::pesca(){
    for (int i=0; i<8; ++i){
        if (lettere[i]->isEmpty()){
        *lettere[i] = estrazione();
        }
    }
}

Lettera Giocatore::getLettera(int pos){
    return *lettere[pos];
}

void Giocatore::removeLettera(int pos){
    lettere[pos]->toEmpty();
}

void Giocatore::addLettera(int pos, Lettera &letter){
    *lettere[pos] = letter;
}
