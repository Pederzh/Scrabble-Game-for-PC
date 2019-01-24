#include "dizionario.h"
#include "stdio.h"
#include "fstream"
#include "QString"
#include "QTextStream"
#include "QMAP"
#include "QVector"
#include "QFile"

typedef QMap<QString, QString> dizionari;
QMap<QString, QString> diz2;
QMap<QString, QString> diz3;
QMap<QString, QString> diz4;
QMap<QString, QString> diz5;
QMap<QString, QString> diz6;
QMap<QString, QString> diz7;
QMap<QString, QString> diz8;
QMap<QString, QString> diz9;
QMap<QString, QString> diz10;
QMap<QString, QString> diz11;
QMap<QString, QString> diz12;
QMap<QString, QString> diz13;
QMap<QString, QString> diz14;
QMap<QString, QString> diz15;
QMap<QString, QString> diz16;
QMap<QString, QString> diz17;

QVector<dizionari*> diz;

void caricaDizionario(){
    QFile file("C:/Users/Luigi/Desktop/Scarabeo/words_italian.txt");
    //QFile file("C:/Users/Luigi/Desktop/Scarabeo/italian.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        //vettore dizionario contenente la map di parole di lunghezze uguale all
        setDizionario(line.length(), line);
    }
    diz.reserve(17);
    for(int i=0;i<(17+1-2);++i){
        diz.push_back(new dizionari);
    }
    //Lo swap del tipo di dato QMap all'interno del QVector è ottimizzato e non comporta la copia di tutto.
    diz[0]->swap(diz2);
    diz[1]->swap(diz3);
    diz[2]->swap(diz4);
    diz[3]->swap(diz5);
    diz[4]->swap(diz6);
    diz[5]->swap(diz7);
    diz[6]->swap(diz8);
    diz[7]->swap(diz9);
    diz[8]->swap(diz10);
    diz[9]->swap(diz11);
    diz[10]->swap(diz12);
    diz[11]->swap(diz13);
    diz[12]->swap(diz14);
    diz[13]->swap(diz15);
    diz[14]->swap(diz16);
    diz[15]->swap(diz17);

}

void setDizionario(int lung, const QString& p){
    switch (lung){
    case 2:
        diz2.insert(p,p);
        break;
    case 3:
        diz3.insert(p,p);
        break;
    case 4:
        diz4.insert(p,p);
        break;
    case 5:
        diz5.insert(p,p);
        break;
    case 6:
        diz6.insert(p,p);
        break;
    case 7:
        diz7.insert(p,p);
        break;
    case 8:
        diz8.insert(p,p);
        break;
    case 9:
        diz9.insert(p,p);
        break;
    case 10:
        diz10.insert(p,p);
        break;
    case 11:
        diz11.insert(p,p);
        break;
    case 12:
        diz12.insert(p,p);
        break;
    case 13:
        diz13.insert(p,p);
        break;
    case 14:
        diz14.insert(p,p);
        break;
    case 15:
        diz15.insert(p,p);
        break;
    case 16:
        diz16.insert(p,p);
        break;
    case 17:
        diz17.insert(p,p);
        break;
    default:
        break;
    }
}
