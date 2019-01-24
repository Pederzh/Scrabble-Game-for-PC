#include "cella.h"
#include "lettera.h"

Cella::Cella()
{
    lett = new Lettera();
    setText((QString)lett->getValore());
    molt_tipo='L';
    molt_punt=1;
    jolly=false;
    inserita=0;

}

Cella::~Cella()
{
    //DISTRUTTORE
    delete lett;
}

//PER LE CASELLE DEL GIOCATORE
void Cella::setCellaGioc(Lettera &letter, int pos){
    lett = new Lettera (letter);
    lett->setOcc(1);
    setText((QString)lett->getValore());
    molt_tipo='L';
    molt_punt=1;
    x = pos;
    y = 0;
    jolly = lett->isJolly();
}

void Cella::setPunt(char tipo, int valore){
    molt_tipo = tipo;
    molt_punt = valore;
}

void Cella::setLett(Lettera &letter){
    lett = new Lettera (letter);
    setText((QString)lett->getValore());
}

bool Cella::moltLett(){
    if (molt_tipo=='L')
        return true;
    else
        return false;
}

void Cella::toEmpty(){
    lett->toEmpty();
    setText((QString)lett->getValore());
    jolly = lett->isJolly();
    inserita=0;
}

bool Cella::isEmpty(){
    if(lett->isEmpty())
        return true;
    else
        return false;
}

void Cella::setup(int row, int col){
    //setto le coordinate
    y = row;
    x = col;

    QPalette pal; //per il colore

    int cont = 17*row+col;

    switch (cont){
    case(0*17+0):
    case(0*17+8):
    case(0*17+16):
    case(8*17+0):
    case(8*17+16):
    case(16*17+0):
    case(16*17+8):
    case(16*17+16):
        pal.setColor(QPalette::Button, QColor(Qt::black));
        setPunt('P',3);
        break;
    case(0*17+4):
    case(0*17+12):
    case(2*17+7):
    case(2*17+9):
    case(3*17+8):
    case(4*17+0):
    case(4*17+16):
    case(7*17+2):
    case(7*17+7):
    case(7*17+9):
    case(7*17+14):
    case(8*17+3):
    case(8*17+13):
    case(16*17+4):
    case(16*17+12):
    case(14*17+7):
    case(14*17+9):
    case(13*17+8):
    case(12*17+0):
    case(12*17+16):
    case(9*17+14):
    case(9*17+9):
    case(9*17+7):
    case(9*17+2):
        pal.setColor(QPalette::Button, QColor(Qt::yellow));
        setPunt('L',2);
        break;
    case(1*17+1):
    case(2*17+2):
    case(3*17+3):
    case(4*17+4):
    case(5*17+5):
    case(1*17+15):
    case(2*17+14):
    case(3*17+13):
    case(4*17+12):
    case(5*17+11):
    case(11*17+11):
    case(12*17+12):
    case(13*17+13):
    case(14*17+14):
    case(15*17+15):
    case(15*17+1):
    case(14*17+2):
    case(13*17+3):
    case(12*17+4):
    case(11*17+5):
        pal.setColor(QPalette::Button, QColor(Qt::blue));
        setPunt('P',2);
        break;
    case(1*17+6):
    case(1*17+10):
    case(6*17+1):
    case(6*17+6):
    case(6*17+10):
    case(6*17+15):
    case(15*17+6):
    case(15*17+10):
    case(10*17+1):
    case(10*17+6):
    case(10*17+10):
    case(10*17+15):
        pal.setColor(QPalette::Button, QColor(Qt::red));
        setPunt('L',3);
        break;
    case(144):
        pal.setColor(QPalette::Button, QColor(Qt::magenta)); //la casella centrale non ha punteggio va solo colorata
        break;
    default:
        pal.setColor(QPalette::Button, QColor(Qt::gray));
    }
    setAutoFillBackground(true);
    setPalette(pal);
}
