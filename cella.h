#ifndef CELLA_H
#define CELLA_H

#include <QPushButton>
#include "lettera.h"

class Cella : public QPushButton //l'oggetto cella eredita QPushButton (è un bottone)
{
    Q_OBJECT

public:
    Cella();  //costruttore
    ~Cella();          //distruttore
    void setCellaGioc(Lettera &letter, int pos);
    void setup(int y, int x);
    void setPunt(char tipo, int valore);
    void setLett(Lettera &letter);
    void setJolly(Lettera &letter){jolly=letter.isJolly();}
    Lettera getLett(){return *lett;}
    int getMolt(){return molt_punt;}
    bool moltLett();
    int getX(){return x;}
    int getY(){return y;}
    void toEmpty();
    bool isEmpty();
    bool isJolly(){return jolly;}
    void setInserita(){inserita=1;} //quando inserisco
    void setDentro(){inserita=2;}   //quando finisco il turno
    bool isDefault(){if(inserita==0)return true; else return false;}
    bool isInserita(){if(inserita==1)return true; else return false;} //se è appena stata inserita (nel turno stesso)
    bool isDentro(){if(inserita==2)return true; else return false;}    //se è gia in gioco (con cui incrociare)



private:
    int x;              //coordinata x
    int y;              //coordinata y
    Lettera *lett;       //lettera - (testo della cella)
    char molt_tipo;     //tipo di moltiplicatore, lettera o parola - (colore cella)
    int molt_punt;       //valore del moltiplicatore               - (colore cella)
    bool jolly;         //se è jolly - (sovrascriver il testo della cella con un '*')
    int inserita;      //mi indica la lettera gia presente nella griglia con cui incrociare (0=cella vuota | 1=appena inserita | 2=già in gioco (con cui incrocia)

signals:

public slots:

};

#endif // CELLA_H
