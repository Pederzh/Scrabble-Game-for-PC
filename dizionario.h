#ifndef DIZIONARIO_H
#define DIZIONARIO_H

#include "lettera.h"
#include <QString>
#include <QVector>
#include <QMap>

typedef QMap<QString, QString> dizionari;
extern QMap<QString, QString> diz2;
extern QMap<QString, QString> diz3;
extern QMap<QString, QString> diz4;
extern QMap<QString, QString> diz5;
extern QMap<QString, QString> diz6;
extern QMap<QString, QString> diz7;
extern QMap<QString, QString> diz8;
extern QMap<QString, QString> diz9;
extern QMap<QString, QString> diz10;
extern QMap<QString, QString> diz11;
extern QMap<QString, QString> diz12;
extern QMap<QString, QString> diz13;
extern QMap<QString, QString> diz14;
extern QMap<QString, QString> diz15;
extern QMap<QString, QString> diz16;
extern QMap<QString, QString> diz17;

extern QVector<dizionari*> diz;

//-------------------------------------
//FUNZIONI
void caricaDizionario();
void setDizionario(int lung, const QString& p);

#endif // DIZIONARIO_H
