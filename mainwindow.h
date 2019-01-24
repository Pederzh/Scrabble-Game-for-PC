#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <giocatore.h>
#include <vector>
#include <cella.h>
#include <lettera.h>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT //macro che converte in un QOBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void inizio();
    void gioco();
    //gestione turni
    int turno(){return _turno;}
    int click(){return _click;}
    int numGioc(){return _numGioc;}
    void setNumGioc(int numGioc){_numGioc = numGioc;}
    void incTurno(){_turno++;}
    void incClick(){_click++;}
    void decClick(){_click--;}
    void azzClick(){_click=1;}
    void azzGiocoiniziato(){_giocoiniziato=false;}
    void setGiocoiniziato(){_giocoiniziato=true;}
    bool isNotGiocoiniziato(){return !_giocoiniziato;}
    //gestione controllo parola
    int controllaPunteggio();
    void haIncrociato();
    void controllaParola();
    void controllaIncroci();
    void trasfDentro();
    void parolaAdd(char c){parola+=c;}
    void parolaToEmpty(){parola="";}
    bool parolaIsEmpty(){if (parola=="") return true; else return false;}
    bool parolaEsiste(QString par);
    void inizioToNull(){_inizio=18;} //valore nullo
    bool inizioIsNull(){if(_inizio==18){return true;} else{return false;}}
    void setInizio(int i){_inizio=i;}
    int parolaInizio(){return _inizio;}
    //gestione punteggi
    void parolaPunteggio(Lettera lett, Cella* &c );
    void bonusPunteggi();
    void molt_totToEmpty(){molt_tot=1;}
    void puntToEmpty(){punt=0;}
    void aggiornaPunteggio(int punteggio);
    int prendiPunteggio(){int g = (turno()-1) % numGioc(); Giocatore& gioc = giocatori[g]; return gioc.getPunteggio();}
    void impostaPunteggio(int punteggio){int g = (turno()-1) % numGioc(); Giocatore& gioc = giocatori[g]; gioc.setPunteggio(punteggio);}
    //gestione inserimento casella
    int selez(){return _selez;}
    void selez(int coordinata){_selez=coordinata;}
    void selezToNull(){_selez=9;} //valore nullo
    bool selezIsNotNull(){ if(_selez==9){return false;} else{return true;}}
    //gestione lettera selezionta e jolly
    void setSelezione(char l);
    void scambiaJolly(Cella* c);
    //gestione griglia di gioco (celle)
    bool adiacentiIsOk();
    bool filaIsOk(int y, int x);
    char fila(){return _fila;}
    void fila(char c){_fila=c;}
    void filaToNull(){_fila=' ';}
    void salvaFila();
    int filaX(){return _filaX;}
    bool filaXIsNotNull(){if (_filaX==18) return false; else return true;}
    void filaX(int coordinata){_filaX=coordinata;}
    void filaXToNull(){_filaX=18;}
    int generaFila(int oldx, int oldy, int x, int y);
    int filaY(){return _filaY;}
    bool filaYIsNotNull(){if (_filaY==18) return false; else return true;}
    void filaY(int coordinata){_filaY=coordinata;}
    void filaYToNull(){_filaY=18;}
    //gestione errori
    void errore(int cod);
    //gestione suggerimento
    void sugInserisciLettera(Cella *c, Lettera &lett);
    void sugControllaFIla(char fil, int crd, QVector<Lettera> &lettGioc);
    bool sugAdiacentiIsOk(QString &par, char fil, int inizio);
    void sugAnnullaTutto();
    void sugBonusPunteggi(QString &par, QVector<Lettera> &l);
    void sugAggiornaPunteggio(int punteggio);
    int lungh(int l){return (l-2);}
    bool maxPunteggio(int p, const QString &par);

private slots:
    void on_actionNuova_Partita_triggered();
    void on_inizioButton_clicked();

public slots:
    void selezionaLettera();
    void selezionaCella();
    void inserisciLettera(Cella *c);
    void annullaInserimento();
    void annullaTutto();
    void terminaTurno();
    void suggerimento();
    void creaGiocatori(int numGioc);
    void jollyToLettera(char lett);
    void fineGioco();

signals:
    void numeroGioc(int numGioc);
    void letteraJolly(char lettera);

private:
    Ui::MainWindow *ui;
    std::vector<Giocatore> giocatori;
    int _numGioc;
    int _turno;
    int _click;
    char _fila;
    int _filaX;
    int _filaY;
    bool _giocoiniziato;
    const int DIM_GRIGLIA=17;
    Cella *cell[17][17];
    const int DIM_LETTERE=8;
    Cella *caselle[8];
    Lettera *selezione;
    int _selez;
    QString parola; //parola letta
    int molt_tot;//moltiplicatore punteggi parola
    int punt;   //punteggio relativo
    int _inizio; //coordinata inizio parola
    bool jollyGiocato; //vede se ho giocato il jolly
    //variabili per il suggerimento
    int bestY; //salvo la coordinata della parola migliore
    int bestX;
    char bestFila;
    int maxPunt;
    int adiacPunt;
    int bestAdiacPunt;
    int vinto;
    QString bestParola; //salvo la parola migliore
    bool sugg;
};

#endif // MAINWINDOW_H
