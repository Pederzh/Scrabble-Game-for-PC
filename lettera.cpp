#include "lettera.h"

Lettera::Lettera(){
    //COSTRUTTORE SENZA PARAMENTRI
    val = ' ';
    occ = 0;
    punt = 0;
}

Lettera::Lettera(char valore, int occorrenze, int punteggio){
    //COSTRUTTORE CON PARAMETRI
    //inizializzare il mazzo
    val = valore;
    occ = occorrenze;
    punt = punteggio;
}

Lettera::~Lettera(){
    //DISTRUTTORE
}

void Lettera::setValore(char valore){
    val=valore;
    switch(valore){
    case 'A': case 'C': case 'E': case 'I': case 'O': case 'R': case 'S': case 'T':
        punt=1;
        break;
    case 'B': case 'D': case 'F': case 'G': case 'U': case 'V':
        punt=4;
        break;
    case 'P':
        punt=3;
        break;
    case 'Q':
        punt=10;
        break;
    case 'H': case 'Z':
        punt=8;
        break;
    case 'L': case 'M': case 'N':
        punt=2;
        break;

    }
}

int Lettera::getPunteggio(){
    return punt;
}

int Lettera::getOccorrenze(){
    return occ;
}

char Lettera::getValore(){
    return val;
}

void Lettera::toEmpty(){
    if(!isEmpty()){
        val = ' ' ;
        occ = 0;
        punt = 0;
    }
}

bool Lettera::isEmpty(){
    if (getValore()==' ')
        return true;
    else
        return false;
}

bool Lettera::isFinish(){
    if (occ==0){
        return true;
    }
    else{
        return false;
    }
}

bool Lettera::isJolly(){
    const char JOLLY = '*';
    if (getValore()==JOLLY){
        return true;
    }
    else{
        return false;
    }
}

void Lettera::decrementa(){
    if (!isFinish())
        occ--;
}

void Lettera::incrementa(){
    occ++;
}
