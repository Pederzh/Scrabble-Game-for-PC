#-------------------------------------------------
#
# Project created by QtCreator 2016-12-06T17:13:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProvaGUI-1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog_giocatori.cpp \
    cella.cpp \
    lettera.cpp \
    mazzo.cpp \
    giocatore.cpp \
    dizionario.cpp \
    dialog_jolly.cpp

HEADERS  += mainwindow.h \
    dialog_giocatori.h \
    cella.h \
    mazzo.h \
    lettera.h \
    giocatore.h \
    dizionario.h \
    dialog_jolly.h

FORMS    += mainwindow.ui \
    dialog_giocatori.ui \
    dialog_jolly.ui

RESOURCES += \
    res.qrc
