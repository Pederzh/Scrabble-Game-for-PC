#ifndef GIOCATORE_H
#define GIOCATORE_H

#include "lettera.h"
#include "string"
class Giocatore
{
private:
    char nome[11];
    int _punteggio;
    Lettera *lettere[8];


public:
    Giocatore(int numero);
    ~Giocatore();
    int name(){return nome[10];}
    int punteggio(){return _punteggio;}
    void setPunteggio(int p){_punteggio=p;}
    int getPunteggio(){return _punteggio;}
    Lettera getLettera(int pos);
    void removeLettera(int pos);
    void addLettera(int pos, Lettera &letter);
    void pesca();

};

#endif // GIOCATORE_H
