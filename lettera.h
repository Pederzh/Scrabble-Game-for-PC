#ifndef LETTERA_H
#define LETTERA_H


class Lettera
{
private:
    char val; //lettera
    int occ;  //occorrenze o lettere rimaste
    int punt; //punteggio della lettera

public:
    Lettera();
    Lettera(char val, int occ, int punt);
    ~Lettera();
    void setup(char val, int occ, int punt);
    char getValore();
    int getOccorrenze();
    int getPunteggio();
    void toEmpty();
    bool isEmpty();
    bool isFinish();
    bool isJolly();
    void decrementa();
    void incrementa();
    void setOcc(int o){occ=o;}
    void setValore(char valore);

};

#endif // LETTERA_H
