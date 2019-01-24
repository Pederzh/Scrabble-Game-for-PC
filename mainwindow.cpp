#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_giocatori.h"
#include "dialog_jolly.h"
#include "cella.h"
#include "lettera.h"
#include "mazzo.h"
#include "giocatore.h"
#include "qmessagebox.h"
#include "dizionario.h"
#include <QVector>
#include <algorithm>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _turno = 1;
    _click = 1;
    bestX = -1;
    bestY = -1;
    adiacPunt = 0;
    bestAdiacPunt = 0;
    bestFila = ' ';
    maxPunt = 0;
    bestParola = " ";
    sugg=false;
    jollyGiocato=false;
    inizioToNull();
    selezToNull();
    filaToNull();
    filaXToNull();
    filaYToNull();
    azzGiocoiniziato();
    molt_totToEmpty();
    puntToEmpty();
    parolaToEmpty();
    caricaDizionario();
    //errore(100);
    selezione = new Lettera();
    ui->setupUi(this);
    ui->frameGioco->hide();
    ui->frameInizio->show();
    connect(this, SIGNAL(numeroGioc(int)), this, SLOT(creaGiocatori(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inizio(){
    int g = (turno()-1) % numGioc();
    Giocatore& gioc = giocatori[g];

    //CONNETTO I PULSANTI GIA' PRESENTI
    connect(ui->annullaButton, SIGNAL(pressed()), this, SLOT(annullaInserimento()));
    connect(ui->annullatuttoButton, SIGNAL(pressed()), this, SLOT(annullaTutto()));
    connect(ui->terminaButton, SIGNAL(pressed()), this, SLOT(terminaTurno()));
    connect(ui->suggerimentoButton, SIGNAL(pressed()), this, SLOT(suggerimento()));
    connect(ui->fineButton, SIGNAL(pressed()), this, SLOT(fineGioco()));

    //AGGIORNO VALORI
    ui->turnoLabel->setText(QString::number(turno()));
    ui->giocatoreLabel->setText(QString::number(gioc.name()));
    ui->punteggiogiocatoreLabel->setText(QString::number(gioc.punteggio()));
    ui->progressBar->hide();

    //CREAZIONE GRIGLIA DI GIOCO
    for (int y=0; y<DIM_GRIGLIA; ++y)
    {
        for (int x=0; x<DIM_GRIGLIA; ++x)
        {
            cell[y][x] = new Cella;
            cell[y][x]->setup(y,x);
            //cell[y][x]->setMinimumWidth(30);
            //cell[y][x]->setMinimumHeight(30);
            ui->grigliaLayout->addWidget(cell[y][x],y,x);
            connect(cell[y][x], SIGNAL(pressed()), this, SLOT(selezionaCella()));
        }
    }
    //----------------------------

    //CREAZIONE LETTERE GIOCATORE
    Lettera *lett = new Lettera();
    for (int i=0; i<DIM_LETTERE; ++i){
        *lett = gioc.getLettera(i); //prendo la lettera dall'insieme estratte del giocatore
        caselle[i] = new Cella();
        caselle[i]->setCellaGioc(*lett, i);
        ui->lettereLayout->addWidget(caselle[i],i);
        connect(caselle[i], SIGNAL(pressed()), this, SLOT(selezionaLettera()));
    }
    //----------------------------
}

void MainWindow::gioco(){
    int g = (turno()-1) % numGioc();
    Giocatore& gioc = giocatori[g];
    gioc.pesca();

    //AGGIORNO VALORI
    ui->turnoLabel->setText(QString::number(turno()));
    ui->giocatoreLabel->setText(QString::number(gioc.name()));
    ui->punteggiogiocatoreLabel->setText(QString::number(gioc.punteggio()));

    //reset suggerimento
    inizioToNull();
    bestX = -1;
    bestY = -1;
    bestFila = ' ';
    maxPunt = 0;
    adiacPunt = 0;
    bestAdiacPunt = 0;
    bestParola = " ";
    sugg=false;

    Lettera *lett = new Lettera();
    for (int i=0; i<DIM_LETTERE; ++i){
        *lett = gioc.getLettera(i); //prendo la lettera dall'insieme estratte del giocatore
        caselle[i]->setCellaGioc(*lett, i);
    }
}

void MainWindow::on_actionNuova_Partita_triggered()
{
    Dialog_Giocatori *gioc = new Dialog_Giocatori (this);
    gioc->setModal(true);
    connect(gioc, SIGNAL(numeroGioc(int)), this, SLOT(creaGiocatori(int)));
    gioc->exec();
}

void MainWindow::on_inizioButton_clicked(){
    //controllo se il dato inserito è valido e se il numero di giocatori è <=4
    //passa il numero di giocatori a..
    if (ui->spinBox->value()<=4)
        emit numeroGioc(ui->spinBox->value());
    else
        errore(4);
}

void MainWindow::fineGioco(){
    int p=0;
    vinto=5;
    for(int g=0; g<numGioc(); ++g){
        Giocatore& gioc = giocatori[g];
        if(gioc.getPunteggio()>p){
            p=gioc.getPunteggio();
            vinto=g;
        }

    }
    if(vinto==5){
        errore(101); //nessuno
    }else{
        errore(100);  //ha vinto qualcuno
        close();
    }
}

void MainWindow::creaGiocatori(int numGioc){
    setNumGioc(numGioc);
    for (int i=0; i<numGioc; ++i){
        Giocatore *gioc = new Giocatore(i+1);
        giocatori.push_back(*gioc);
    }
    ui->frameGioco->show();
    ui->frameInizio->hide();
    inizio();
}

void MainWindow::selezionaLettera(){
    Cella* lettGioc = qobject_cast<Cella*>(sender());
    selez(lettGioc->getX());
}

void MainWindow::selezionaCella(){
    Cella* c = qobject_cast<Cella*>(sender()); //sender è un tipo QObject e devo fare il casting nel tipo che conosco
    int x = c->getX();
    int y = c->getY();

    //TURNO 1
    if (isNotGiocoiniziato()){
        //DOVE POSSIBILE INSERIRE LA PRIMA LETTERA IL PRIMO TURNO (cella centrale)
        if (click()==1){
            if (x==8 && y==8){
                inserisciLettera(c);
                incClick();
                //salvo le coordinate
                filaX(x);
                filaY(y);
                fila('z'); //per la prima lettera
            }
            else
                errore(5);
        }
        //DOVE POSSIBILE INSERIRE LA SECONDA LETTERA IL PRIMO TURNO (croce)
        else if (click()==2){
            //salvo la coordinata di quello che sarà il senso della parola
            int app = generaFila(filaX(),filaY(),x,y);
            if (fila()=='x'){
                inserisciLettera(c);
                incClick();
                filaX(app);
            }
            else if(fila()=='y'){
                inserisciLettera(c);
                incClick();
                filaY(app);
            }
            else
                errore(7);
        }
        //TUTTE LE ALTRE LETTERE IL PRIMO TURNO(o orizzontale o verticale)
        else{
            if(fila()=='x'){
                if(x==filaX()){
                    inserisciLettera(c);
                    incClick();
                }
                else
                    errore(7);
            }
            else if(fila()=='y'){
                if(y==filaY()){
                    inserisciLettera(c);
                    incClick();
                }
                else
                    errore(7);
            }
        }
    }
    //TUTTI GLI ALTRI TURNI
    else if (!(isNotGiocoiniziato())){
        //DOVE POSSIBILE INSERIRE LA PRIMA LETTERA GLI ALTRI TURNI (qualunque riga o colonna in cui è presente una cella da incrociare)
        if (click()==1){
            inserisciLettera(c);
            incClick();
            //salvo le coordinate
            filaX(x);
            filaY(y);
            fila('z'); //per la prima lettera
        }
        //DOVE POSSIBILE INSERIRE LA SECONDA LETTERA CHE MI DIRA' SE STA INSERENDO LA PAROLA IN ORIZZONTALE O VERTICALE (croce)
        else if(click()==2){
            //salvo la coordinata di quello che sarà il senso della parola
            int app = generaFila(filaX(),filaY(),x,y);
            if (fila()=='x'){
                inserisciLettera(c);
                incClick();
                filaX(app);
            }
            else if(fila()=='y'){
                inserisciLettera(c);
                incClick();
                filaY(app);
            }
            else if(fila()==' ')
                errore(7);
        }
        //TUTTE LE ALTRE LETTERE GLI ALTRI TURNI(o orizzontale o verticale)
        else{
            if(fila()=='x'){
                if(x==filaX()){
                    inserisciLettera(c);
                    incClick();
                }
                else
                    errore(7);
            }
            else if(fila()=='y'){
                if(y==filaY()){
                    inserisciLettera(c);
                    incClick();
                }
                else if(fila()==' ')
                    errore(7);
            }
        }
    }
}

void MainWindow::inserisciLettera(Cella* c){
    if (selezIsNotNull()){ //se ho selezionato una casella
        *selezione = caselle[selez()]->getLett();
        if(!(selezione->isEmpty())){ //se la casella selezionata non è vuota
            if (c->isEmpty()){ //se la cella del campo da gioco è vuota
                c->setJolly(*selezione);  //setto se è jolly o no
                if (selezione->isJolly()){
                    //se è jolly setto la lettera dal
                    //dialog
                    Dialog_jolly *jol = new Dialog_jolly (this);
                    jol->setModal(true);
                    connect(jol, SIGNAL(letteraJolly(char)), this, SLOT(jollyToLettera(char)));
                    jol->exec();
                    jollyGiocato=true;
                }
                //inserisce la lettera
                c->setLett(*selezione);
                if((isNotGiocoiniziato()) && (click()==1))
                    c->setDentro();     //solo per la prima lettera inserita
                else
                    c->setInserita();   //per tutte le altre
                //alla fine tolgo la casella inserita da quelle disponibili al giocatore
                //dai dati giocatore
                int g = (turno()-1) % numGioc();
                Giocatore& gioc = giocatori[g];
                gioc.removeLettera(selez());
                //dalla grafica
                selezione->toEmpty();
                caselle[selez()]->toEmpty();
            }
            else{
                decClick();
                //se la cella non è vuota controllo se è jolly e se la lettera corrisponde li scambio
                if(c->isJolly())
                    scambiaJolly(c);
                else
                    errore(3);
            }
        }
        else{
            decClick();
            errore(2);
        }
    }
    else{
        decClick();
        errore(1);
    }
}

//SCAMBIA IL JOLLY DALLA GRIGLIA ALLA MANO DEL GIOCATORE
void MainWindow::scambiaJolly(Cella* c){
    if(selezione->getValore()==c->getLett().getValore()){
        c->setJolly(*selezione);
        c->setLett(*selezione);
        Lettera *jolly = new Lettera('*',2,0);
        caselle[selez()]->setCellaGioc(*jolly, selez());
        //alla fine tolgo la casella inserita da quelle disponibili al giocatore
        //dai dati giocatore
        int g = (turno()-1) % numGioc();
        Giocatore& gioc = giocatori[g];
        gioc.removeLettera(selez());
        gioc.addLettera(selez(),*jolly);
        //dalla grafica

    }
    else{
        errore(10);
    }
}

//TRASFORMA IL JOLLY NELLA LETTERA RICHIESTA DAL GIOCATORE
void MainWindow::jollyToLettera(char lett){
    selezione->setValore(lett);
}

void MainWindow::annullaInserimento(){
    selezToNull();
}

void MainWindow::annullaTutto(){
    //qui annulli tutto: Se la parola non va bene | Se preme annulla tutto
    if(click()>2){
        jollyGiocato=false;
        int x = filaX();
        int y = filaY();
        if(fila()=='x'){       //se è colonna
            if(x==filaX()){
                //colonna è fissa (x) riga cambia (i)
                for(int i=0; i<DIM_GRIGLIA; ++i){
                    if(cell[i][x]->isInserita() || (isNotGiocoiniziato() && cell[i][x]->isDentro())){
                        if (isNotGiocoiniziato())  //per il passo al primo turno
                            azzClick();
                        for (int k=0; k<DIM_LETTERE; ++k){
                            //Se la casella del giocatore è vuota rimetto la lettera inserita precedentemente
                            if (caselle[k]->isEmpty()){
                                if(cell[i][x]->isJolly()){
                                    //se ho jolly rimposto il jolly
                                    Lettera *jolly = new Lettera('*',1,0);
                                    caselle[k]->setCellaGioc(*jolly, k);
                                    delete jolly;
                                    jolly = nullptr;
                                    break;
                                }
                                else{
                                    //se è lettera rimposto lettera
                                    Lettera *lett = new Lettera();
                                    *lett = cell[i][x]->getLett();
                                    caselle[k]->setCellaGioc(*lett, k);
                                    delete lett;
                                    lett = nullptr;
                                    break;
                                }
                            }
                        }
                        cell[i][x]->toEmpty();
                    }
                }
            }
        }
        else if(fila()=='y'){   //se è riga
            if(y==filaY()){
                //riga è fissa (y) colonna cambia (i)
                for(int i=0; i<DIM_GRIGLIA; ++i){
                    if(cell[y][i]->isInserita() || (isNotGiocoiniziato() && cell[y][i]->isDentro())){
                        if (isNotGiocoiniziato())
                            azzClick();
                        for (int k=0; k<DIM_LETTERE; ++k){
                            //Se la casella del giocatore è vuota rimetto la lettera inserita precedentemente
                            if (caselle[k]->isEmpty()){
                                if(cell[y][i]->isJolly()){
                                    //se ho jolly rimposto il jolly
                                    Lettera *jolly = new Lettera('*',1,0);
                                    caselle[k]->setCellaGioc(*jolly, k);
                                    delete jolly;
                                    jolly = nullptr;
                                    break;
                                }
                                else{
                                    //se è lettera rimposto lettera
                                    Lettera *lett = new Lettera();
                                    *lett = cell[y][i]->getLett();
                                    caselle[k]->setCellaGioc(*lett, k);
                                    delete lett;
                                    lett = nullptr;
                                    break;
                                }
                            }
                        }
                        cell[y][i]->toEmpty();
                    }
                }
            }
        }
    }
    else{
        int x = filaX();
        int y = filaY();
        //lettera singola
        for (int k=0; k<DIM_LETTERE; ++k){
            if (caselle[k]->isEmpty()){
                if(cell[y][x]->isJolly()){
                    //se ho jolly rimposto il jolly
                    Lettera *jolly = new Lettera('*',1,0);
                    caselle[k]->setCellaGioc(*jolly, k);
                    delete jolly;
                    jolly = nullptr;
                    break;
                }
                else{
                    //se è lettera rimposto lettera
                    Lettera *lett = new Lettera();
                    *lett = cell[y][x]->getLett();
                    caselle[k]->setCellaGioc(*lett, k);
                    delete lett;
                    lett = nullptr;
                    break;
                }
            }
        }
        if(filaYIsNotNull() && filaYIsNotNull() && sugg==false)
            cell[y][x]->toEmpty();
    }
    azzClick();
    filaToNull();
    filaXToNull();
    filaYToNull();
    selezToNull();
}

void MainWindow::trasfDentro(){
    //trasforma le lettere inserite, in lettere dentro nel gioco (per il prox turno)
    int x = filaX();
    int y = filaY();
    if(fila()=='x'){      //se è colonna
        if(x==filaX())
            for(int i=0; i<DIM_GRIGLIA; ++i)
                if(cell[i][x]->isInserita())
                    cell[i][x]->setDentro();
    }
    else if(fila()=='y'){ //se è riga
        if(y==filaY())
            for(int i=0; i<DIM_GRIGLIA; ++i)
                if(cell[y][i]->isInserita())
                    cell[y][i]->setDentro();
    }
    else if(fila()=='z') //se è lettera singola
        cell[y][x]->setDentro();
}

void MainWindow::terminaTurno(){
    int punteggio = prendiPunteggio();
    if(click()==1){
        //PASSA IL TURNO
        incTurno();
        azzClick();
        inizioToNull();
        filaToNull();
        filaXToNull();
        filaYToNull();
        selezToNull();
        gioco();
    }
    else{
        //prima controlla la fromazione della parola
        if(filaIsOk(filaY(),filaX()) && parolaEsiste(parola)){
            //controlla se la parola esiste
            bonusPunteggi();
            aggiornaPunteggio(punt);
            //------------------------
            jollyGiocato=false;
            //controllare gli adiacenti (che controlla anche se le parole adiacenti esistono)
            if(adiacentiIsOk()){
                errore(1000); //non è un errore ma comunica il puynteggio fatto
                trasfDentro();
                incTurno();
                azzClick();
                inizioToNull();
                filaToNull();
                filaXToNull();
                filaYToNull();
                selezToNull();
                molt_totToEmpty();
                puntToEmpty();
                parolaToEmpty();
                setGiocoiniziato();
                gioco(); //QUI RIESEGUIRAI / RICHIAMERAI LA FUNZIONE CHE GESTISCE I TURNI
            }
            else{
                impostaPunteggio(punteggio); //rimette il punteggio del giocatore ad inizio turno
                molt_totToEmpty();
                puntToEmpty();
                parolaToEmpty();
                annullaTutto();
            }
        }
        else{
            impostaPunteggio(punteggio); //rimette il punteggio del giocatore ad inizio turno
            molt_totToEmpty();
            puntToEmpty();
            parolaToEmpty();
            annullaTutto();
        }
    }

}
bool MainWindow::adiacentiIsOk(){
    //FUNZIONE CHE CONTROLLA TUTTI GLI INCROCI CON LE ALTRE PAROLE
    //E SE ANDANDO AD INCROCIARE LETTERE ADIACENTI VIENE FORMATA UNA NUOVA PAROLA, LA CONTO NEL PUNTEGGIO
    int x = filaX();
    int y = filaY();
    int ok=0;
    QString par = parola;   //salvo la parola iniziale del giocatore
    int start = parolaInizio();   //salvo l'inizio della parola

    if(fila()=='x'){      //se è colonna
        //scambio riga per colonna per far controllare gli adiacenti
        fila('y');
        //colonna è fissa (x) riga cambia (i)
        for(int i=start; i<(start+par.length()); ++i){
            //controlli per la lettera se ha non lettere adiacenti
            if(x==0){                   //bordo alto
                if(cell[i][x+1]->isDefault())
                    ++ok;
                else{
                    bool dentro=false;
                    if(cell[i][x]->isDentro()){
                        cell[i][x]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(i,filaX()) && parolaEsiste(parola)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            aggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok; //conta
                    }

                    if(dentro==true)
                        cell[i][x]->setDentro();
                }
            }
            else if(x==DIM_GRIGLIA-1){  //bordo basso
                if(cell[i][x-1]->isDefault())
                    ++ok;
                else{
                    bool dentro=false;
                    if(cell[i][x]->isDentro()){
                        cell[i][x]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(i,filaX()) && parolaEsiste(parola)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            aggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok; //conta
                    }

                    if(dentro==true)
                        cell[i][x]->setDentro();
                }
            }
            else{                       //qualsiasi altro punto
                if(cell[i][x-1]->isDefault() && cell[i][x+1]->isDefault())
                    ++ok;
                else{
                    bool dentro=false;
                    if(cell[i][x]->isDentro()){
                        cell[i][x]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(i,filaX()) && parolaEsiste(parola)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            aggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok; //conta
                    }
                    if(dentro==true)
                        cell[i][x]->setDentro();
                }
            }

        }
        fila('x'); //rimetto apposto la fila
    }
    else if(fila()=='y'){  //se è riga
        //scambio riga per colonna per far controllare gli adiacenti
        fila('x');
        //riga è fissa (y) colonna cambia (i)
        for(int i=start; i<(start+par.length()); ++i){
            //controlli per la lettera se ha non lettere adiacenti
            if(y==0){
                if(cell[y+1][i]->isDefault())
                    ++ok;
                else {
                    bool dentro=false;
                    if(cell[y][i]->isDentro()){
                        cell[y][i]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(filaY(),i) && parolaEsiste(parola)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            aggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok;
                    }
                    if(dentro==true)
                        cell[y][i]->setDentro();
                }
            }
            else if(y==DIM_GRIGLIA-1){
                if(cell[y-1][i]->isDefault())
                    ++ok;
                else {
                    bool dentro=false;
                    if(cell[y][i]->isDentro()){
                        cell[y][i]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(filaY(),i) && parolaEsiste(parola)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            aggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok;
                    }

                    if(dentro==true)
                        cell[y][i]->setDentro();
                }
            }
            else{
                if(cell[y-1][i]->isDefault() && cell[y+1][i]->isDefault())
                    ++ok;
                else {
                    bool dentro=false;
                    if(cell[y][i]->isDentro()){
                        cell[y][i]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(filaY(),i) && parolaEsiste(parola)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            aggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok;
                    }

                    if(dentro==true)
                        cell[y][i]->setDentro();
                }
            }
        }
        fila('y'); //rimetto apposto la fila
    }
    //se ok è uguale al numero di lettere della parola va bene
    if(ok==par.length())
        return true;
    else{
        errore(11);
        return false;
    }

}

bool MainWindow::filaIsOk(int y, int x){
    //se la fila è ok, intanto mi trova gia la parola, e mi calcola il punteggio
    inizioToNull();
    parolaToEmpty();
    bool ok=false;
    bool incrociata=false;
    bool inserita=false;
    bool parolaIniziata=false;
    bool fine=false;
    if(fila()=='x' || fila()=='z'){      //se è colonna
        //colonna è fissa (x) riga cambia (i)
        for(int i=0; i<DIM_GRIGLIA; ++i){
            if(cell[i][x]->isDentro()){
                incrociata=true;
                if (sugg==false)
                    parolaPunteggio(cell[i][x]->getLett(), cell[i][x]);
                parolaAdd(cell[i][x]->getLett().getValore());
                if(inizioIsNull())
                    setInizio(i);
            }
            if(cell[i][x]->isInserita()){
                inserita=true;
                if (sugg==false)
                    parolaPunteggio(cell[i][x]->getLett(), cell[i][x]);
                parolaAdd(cell[i][x]->getLett().getValore());
                if(inizioIsNull())
                    setInizio(i);
            }
            if(cell[i][x]->isDefault() && inserita==false){
                incrociata=false;
                inizioToNull();
                parolaToEmpty();
                molt_totToEmpty();
            }
            if(incrociata==true && inserita==true){
                parolaIniziata=true;
            }
            if((cell[i][x]->isDefault() && parolaIniziata==true)||(!cell[i][x]->isDefault() && parolaIniziata==true && i==DIM_GRIGLIA-1)){
                //fine
                fine=true;
                ok=true;
                if (fila()=='z')
                    fila('x');
                break;
            }
            else if((fine==true && cell[i][x]->isInserita())||(cell[i][x]->isDefault() && inserita==true && parolaIniziata==false)||(parolaIniziata==false && i==DIM_GRIGLIA-1)){
                //errore ha inserito piu lettere sparse sulla stessa riga dopo o prima della parola
                ok=false;
                incrociata=false;
                inserita=false;
                parolaIniziata=false;
                fine=false;
                break;
            }
        }
    }

    if((fila()=='y') || (fila()=='z' && ok==false)){ //se è riga
        //riga è fissa (y) colonna cambia (i)
        for(int i=0; i<DIM_GRIGLIA; ++i){
            if(cell[y][i]->isDentro()){
                incrociata=true;
                if (sugg==false)
                    parolaPunteggio(cell[y][i]->getLett(), cell[y][i]);
                parolaAdd(cell[y][i]->getLett().getValore());
                if(inizioIsNull())
                    setInizio(i);
            }
            if(cell[y][i]->isInserita()){
                inserita=true;
                if (sugg==false)
                    parolaPunteggio(cell[y][i]->getLett(), cell[y][i]);
                parolaAdd(cell[y][i]->getLett().getValore());
                if(inizioIsNull())
                    setInizio(i);
            }
            if(cell[y][i]->isDefault() && inserita==false){
                incrociata=false;
                inizioToNull();
                parolaToEmpty();
                molt_totToEmpty();
            }
            if(incrociata==true && inserita==true){
                parolaIniziata=true;
            }
            if((cell[y][i]->isDefault() && parolaIniziata==true)||(!cell[y][i]->isDefault() && parolaIniziata==true && i==DIM_GRIGLIA-1)){
                //fine
                fine=true;
                ok=true;
                if (fila()=='z')
                    fila('y');
                break;
            }else if((fine==true && cell[y][i]->isInserita())||(cell[y][i]->isDefault() && inserita==true && parolaIniziata==false)||(parolaIniziata==false && i==DIM_GRIGLIA-1)){
                //errore ha inserito piu lettere sparse sulla stessa riga dopo o prima della parola
                ok=false;
                incrociata=false;
                inserita=false;
                parolaIniziata=false;
                fine=false;
                break;
            }
        }
    }
    if(ok==true){
        if(sugg==false) //se è qui dal suggerimento non deve aggiornare il punteggio
            punt = punt*molt_tot;
        return true;
    }
    else{
        return false;
        errore(8);
        inizioToNull();
        parolaToEmpty();
        molt_totToEmpty();
    }
}

bool MainWindow::parolaEsiste(QString par){
    return diz[lungh(par.length())]->contains(par.toLower());
}

void MainWindow::parolaPunteggio(Lettera lett, Cella* &c){
    if (c->moltLett()) //Se il moltiplicatore è lettera
        punt += lett.getPunteggio() * c->getMolt();
    else{ //se il moltiplicatore è parola
        molt_tot*=c->getMolt();
        punt += lett.getPunteggio();
    }
}

void MainWindow::bonusPunteggi(){
    if (jollyGiocato==false)
        punt+=10;
    //---------------------
    if (parola.length()==6)
        punt+=10;
    else if(parola.length()==7)
        punt+=30;
    else if(parola=="scarabeo" || parola=="scarabei")
        punt+=100;
    else{
        bool vuoto=true;
        for(int k=0; k<DIM_LETTERE; ++k){
            //se trovo una lettera nelle caselle del giocatore significa che non le ha usate tutte
            if(!caselle[k]->isEmpty()){
                vuoto=false;
                break;
            }
        }
        if(vuoto==true)
            punt+=50;
    }
}

void MainWindow::aggiornaPunteggio(int punteggio){
    int g = (turno()-1) % numGioc();
    Giocatore& gioc = giocatori[g];
    gioc.setPunteggio(gioc.getPunteggio()+punteggio);
    //svuota le variabili locali
    molt_totToEmpty();
    puntToEmpty();
}

int MainWindow::generaFila(int oldx, int oldy, int x, int y){
    if(x==oldx){
        fila('x');
        return x;
    }
    else if(y==oldy){
        fila('y');
        return y;
    }
    else{
        fila(' ');
        return ' ';
    }
}

void MainWindow::errore(int cod){
    QMessageBox messageBox;
    switch (cod)
    {
    case(1):
        messageBox.critical(0,"Errore","Seleziona una lettera!");
        break;
    case(2):
        messageBox.critical(0,"Errore","Seleziona una casella non vuota!");
        break;
    case(3):
        messageBox.critical(0,"Errore","Seleziona una cella vuota!");
        break;
    case(4):
        messageBox.critical(0,"Errore","Numero massimo di giocatori è 4!");
        break;
    case(5):
        messageBox.critical(0,"Errore","La prima lettera va inserita in centro!");
        break;
    case(6):
        messageBox.critical(0,"Errore","La tua parola deve incrociare una lettera gia presente!");
        break;
    case(7):
        messageBox.critical(0,"Errore","La tua parola deve essere composta sulla stessa riga/colonna!");
        break;
    case(8):
        messageBox.critical(0,"Errore","Hai inserito lettere sparse OPPURE non hai incrociato una lettera già presente!");
        break;
    case(10):
        messageBox.critical(0,"Errore","Per prendere il jolly devi scambiarlo con la stessa lettera che rappresenta!");
        break;
    case(11):
        messageBox.critical(0,"Errore","Una delle parola adiacenti non esiste!");
        break;
    case(20):
        messageBox.critical(0,"Errore","Non puoi usare il suggerimento il primo turno");
        break;
    case(1000):
        messageBox.about(0,"Comunicazione","Il tuo nuovo punteggio: " + QString("%1")
                            .arg(QString::number(prendiPunteggio())));
        break;
    case(1001):
        messageBox.about(0,"Comunicazione","Parola suggerita: " + QString("%1")
                            .arg(bestParola));
        break;
    case(1002):
        messageBox.critical(0,"Errore","Non esiste alcuna parola inseribile!");
        break;
    case(100):
        messageBox.about(0,"Gioco", "Ha vinto il giocatore: "+ QString("%1")
                         .arg(giocatori[vinto].name()));
        break;
    }
    messageBox.setFixedSize(500,200);
}

//----------------------------------------------------------------------------------------------
//FUNZIONI PER IL SUGGERIMENTO
void MainWindow::suggerimento(){
    ui->progressBar->reset();
    ui->progressBar->show();
    if(!(isNotGiocoiniziato())){
        //sto usando il suggerimento
        sugg=true;
        //punteggio del giocatore
        int punteggio=0;

        //vettore lettere del giocatroe
        QVector<Lettera> lettGioc;
        for(int d=0; d<DIM_LETTERE; ++d){
            bool incr=false;
            for (int i = 0; i < lettGioc.size(); ++i) {
                if (lettGioc[i].getValore() == caselle[d]->getLett().getValore()){
                    lettGioc[i].incrementa(); //Se è gia presente incremento l'occorenza
                    incr=true;
                    break;
                }
            }
            if(incr==false){
                lettGioc.push_back(caselle[d]->getLett()); //prendo la lettera dall'insieme estratte del giocatore
            }

        }
        //inizio a cercare per ogni fila verticale e orizzontale (con lettere)
        for(int c=0; c<DIM_GRIGLIA; ++c){
            filaToNull();
            filaXToNull();
            filaYToNull();
            ui->progressBar->setValue(c);
            //verticale
            for(int i=0; i<DIM_GRIGLIA; ++i){
                //se nella fila vi è anche solo una lettera, cerca
                if(cell[i][c]->isDentro()){
                    sugControllaFIla('x', c, lettGioc); //passa fila e coordinate delle lunghezze
                    break;
                }
            }
            //orizzontale
            for(int i=0; i<DIM_GRIGLIA; ++i){
                //se nella fila vi è anche solo una lettera, cerca
                if(cell[c][i]->isDentro()){
                    sugControllaFIla('y', c, lettGioc); //passa fila e coordinate delle lunghezze
                    break;
                }
            }

            if(punteggio<maxPunt)
                punteggio=maxPunt;
        }
        //quando ha finito
        if(punteggio>0){
            //sommo il punteggio degli adiacenti
            punteggio += bestAdiacPunt;
            //esiste la parola
            aggiornaPunteggio(punteggio);
            errore(1001);
            errore(1000);
            //inserisco la parola
            if(bestFila=='x'){
                int crd = bestX;
                int i = bestY;
                for(int j=0; j<bestParola.length(); ++j){
                    if(cell[i+j][crd]->isDefault()){
                        //se vuota guardo le lettere del giocatroe
                        for(int k=0; k<DIM_LETTERE; ++k){
                            //controllo se la lettera
                            if(bestParola[j].toUpper()==caselle[k]->getLett().getValore()){
                                //inserisco la lettera
                                Lettera* l = new Lettera();
                                l->setValore(bestParola.toUpper().at(j).toLatin1());
                                sugInserisciLettera(cell[i+j][crd], *l);
                                delete l;
                                l = nullptr;
                                //alla fine tolgo la casella inserita da quelle disponibili al giocatore
                                //dai dati giocatore
                                int g = (turno()-1) % numGioc();
                                Giocatore& gioc = giocatori[g];
                                gioc.removeLettera(k);
                                //dalla grafica
                                caselle[k]->toEmpty();
                                break;
                            }
                            else if(k==DIM_LETTERE-1){
                                //uso il jolly
                                for(int s=0; s<DIM_LETTERE; ++s){
                                    //setto il jolly
                                    if(caselle[s]->isJolly()){
                                        *selezione = caselle[s]->getLett();
                                        cell[i+j][crd]->setJolly(*selezione);
                                        //inseirsco la lettera
                                        Lettera* l = new Lettera();
                                        l->setValore(bestParola.toUpper().at(j).toLatin1());
                                        sugInserisciLettera(cell[i+j][crd], *l);
                                        delete l;
                                        l = nullptr;
                                        //alla fine tolgo la casella inserita da quelle disponibili al giocatore
                                        //dai dati giocatore
                                        int g = (turno()-1) % numGioc();
                                        Giocatore& gioc = giocatori[g];
                                        gioc.removeLettera(s);
                                        //dalla grafica
                                        caselle[s]->toEmpty();
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                fila('x');
                filaX(crd);
                filaY(i);
                trasfDentro();
            }
            else if(bestFila=='y'){
                int i = bestX;
                int crd = bestY;
                for(int j=0; j<bestParola.length(); ++j){
                    if(cell[crd][i+j]->isDefault()){
                        //se vuota guardo le lettere del giocatroe
                        for(int k=0; k<DIM_LETTERE; ++k){
                            //controllo se la lettera
                            if(bestParola[j].toUpper()==caselle[k]->getLett().getValore()){
                                //inserisco la lettera
                                Lettera* l = new Lettera();
                                l->setValore(bestParola.toUpper().at(j).toLatin1());
                                sugInserisciLettera(cell[crd][i+j], *l);
                                delete l;
                                l = nullptr;
                                //alla fine tolgo la casella inserita da quelle disponibili al giocatore
                                //dai dati giocatore
                                int g = (turno()-1) % numGioc();
                                Giocatore& gioc = giocatori[g];
                                gioc.removeLettera(k);
                                //dalla grafica
                                caselle[k]->toEmpty();
                                break;
                            }
                            else if(k==DIM_LETTERE-1){
                                //uso il jolly
                                for(int s=0; s<DIM_LETTERE; ++s){
                                    //setto il jolly
                                    if(caselle[s]->isJolly()){
                                        *selezione = caselle[s]->getLett();
                                        cell[crd][i+j]->setJolly(*selezione);
                                        //inseirsco la lettera
                                        Lettera* l = new Lettera();
                                        l->setValore(bestParola.toUpper().at(j).toLatin1());
                                        sugInserisciLettera(cell[crd][i+j], *l);
                                        delete l;
                                        l = nullptr;
                                        //alla fine tolgo la casella inserita da quelle disponibili al giocatore
                                        //dai dati giocatore
                                        int g = (turno()-1) % numGioc();
                                        Giocatore& gioc = giocatori[g];
                                        gioc.removeLettera(s);
                                        //dalla grafica
                                        caselle[s]->toEmpty();
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                fila('y');
                filaX(i);
                filaY(crd);
                trasfDentro();
            }
        }
        else{
            //non esiste alcuna parola inseribile
            errore(1002);
        }

    }
    else{
        //Non puoi usare il suggerimento il primo turno
        errore(20);
    }
    ui->progressBar->hide();
}

void MainWindow::sugControllaFIla(char fil, int crd, QVector<Lettera> &lettGioc){
    fila(fil);
    if(fila()=='x'){
        filaX(crd);
        filaY(0);
    }
    else if(fila()=='y'){
        filaY(crd);
        filaX(0);
    }
    const int INVALID = 100;
    int jolly=INVALID; //salvo la coordinata del jolly (se c'è) (100 is invalid)
    //salvo la coordinata del jolly se c'è
    for(int i=0; i<lettGioc.size();++i){
        if(lettGioc[i].isJolly())
            jolly=i;
    }

    if(fila()=='x'){
        //partendo da ogni cella (della colonna)
        for(int i=0; i<DIM_GRIGLIA; ++i){
            bool isOk=false;
            //se la cella di partenza è preceduta da una lettera inserita in una cella non va bene
            if ((i!=0 && cell[i-1][crd]->isDefault())||(i==0)){
                //partendo dalla posizione della mia cella di partenza cerco a che distanza vi è una lettera inserita in una cella, cosi da doverla incrociare
                for(int m=i; m<DIM_GRIGLIA; ++m){
                    if(cell[m][crd]->isDentro() && isOk==false){
                        isOk=true;
                    }
                    if(isOk==true){
                        if((m<DIM_GRIGLIA-1 && cell[m+1][crd]->isDefault())||(m==DIM_GRIGLIA-1)){
                            //per ogni lunghezza minima dell parola
                            int lung = m+1-i;
                            if(lung>2){
                                jollyGiocato=false;
                                //cerco le parole lunghe lung-2
                                //SELEZIONA PAROLA DALLA MAPPA PER LA LUNGHEZZA
                                foreach (QString parolung, *diz[lungh(lung)]){
                                    QVector<Lettera> lett(lettGioc);
                                    bool inserita = false;
                                    bool ok=false;
                                    azzClick();
                                    for(int j=0; j<lung; ++j){
                                        if(cell[i+j][crd]->isDefault()){
                                            //se vuota guardo le lettere del giocatroe
                                            for(int s=0; s<lett.size(); ++s){
                                                //controllo se la lettera
                                                if(parolung[j].toUpper()==lett[s].getValore() && lett[s].getOccorrenze()>0){
                                                    //inserisco la lettera
                                                    sugInserisciLettera(cell[i+j][crd], lett[s]);
                                                    incClick();
                                                    lett[s].decrementa();
                                                    //setto che va bene
                                                    if(inserita==false)
                                                        filaY(i+j);
                                                    inserita=true;
                                                    ok=true;
                                                    parolaPunteggio(lett[s], cell[i+j][crd]);
                                                    break;
                                                }
                                                else if(s==lett.size()-1 && jolly!=INVALID && lett[jolly].getOccorrenze()>0){
                                                    //Se non ha trovato riscontro nelle lettere del giocatore ma ha 1 o 2 jolly usa quello
                                                    jollyGiocato=true;
                                                    Lettera* jol = new Lettera();
                                                    jol->setValore(parolung.toUpper().at(j).toLatin1());
                                                    //inserisco la lettera
                                                    sugInserisciLettera(cell[i+j][crd], *jol);
                                                    incClick();
                                                    lett[jolly].decrementa();
                                                    //setto che va bene
                                                    if(inserita==false)
                                                        filaY(i+j);
                                                    inserita=true;
                                                    ok=true;
                                                    parolaPunteggio(*jol, cell[i+j][crd]); //utilizza la lettera della parola (siccome è jolly)
                                                    delete jol;
                                                    jol = nullptr;
                                                    break;
                                                }
                                                else if((s==lett.size()-1 && jolly!=INVALID && lett[jolly].getOccorrenze()==0)||(s==lett.size()-1 && jolly==INVALID)){
                                                    ok=false;
                                                    break;
                                                }
                                            }
                                            //appena trovo la prima lettera della parola che non corrisponde a quelle che ha il giocatore: passo alla prossima parola
                                            if(ok==false)
                                                break;
                                        }
                                        else{
                                            //se non lo è confronto la lettera nella stessa posizione
                                            if(cell[i+j][crd]->getLett().getValore()==parolung[j].toUpper()){
                                                parolaPunteggio(cell[i+j][crd]->getLett(), cell[i+j][crd]); // uso la lettera presente per il punteggio di quella casella
                                                ok=true;
                                            }
                                            else{
                                                //se la lettera presente nella griglia non è presente nella stessa posizione nella parola: passo alla prossima parola
                                                ok=false;
                                                break;
                                            }

                                        }
                                    }
                                    //-----------------------------------------
                                    //se la parola si puo formare
                                    if (ok==true && inserita==true){
                                        //la parola si puo formare
                                        punt = punt*molt_tot;
                                        sugBonusPunteggi(parolung,lett);
                                        adiacPunt = 0;
                                        //CONTROLLO LE ADIACENTI
                                        if (sugAdiacentiIsOk(parolung, fila(), i)==true){
                                            //punteggio
                                            if(maxPunteggio(punt,parolung)==true){
                                                bestY=i;
                                                bestX=filaX();
                                                bestFila='x';
                                                bestAdiacPunt = adiacPunt;
                                            }
                                            puntToEmpty();
                                            molt_totToEmpty();
                                            sugAnnullaTutto();
                                        }
                                        else{
                                            adiacPunt = 0;
                                            puntToEmpty();
                                            molt_totToEmpty();
                                            sugAnnullaTutto();
                                        }
                                    }
                                    else{
                                        //non si puo formare
                                        puntToEmpty();
                                        molt_totToEmpty();
                                        sugAnnullaTutto();
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }
    }
    //la stessa cosa per la y
    if(fila()=='y'){
        //partendo da ogni cella (della colonna)
        for(int i=0; i<DIM_GRIGLIA; ++i){
            bool isOk=false;
            //se la cella di partenza è preceduta da una lettera inserita in una cella non va bene
            if ((i!=0 && cell[crd][i-1]->isDefault())||(i==0)){
                //partendo dalla posizione della mia cella di partenza cerco a che distanza vi è una lettera inserita in una cella, cosi da doverla incrociare
                for(int m=i; m<DIM_GRIGLIA; ++m){
                    if(cell[crd][m]->isDentro() && isOk==false){
                        isOk=true;
                    }
                    if(isOk==true){
                        if((m<DIM_GRIGLIA-1 && cell[crd][m+1]->isDefault())||(m==DIM_GRIGLIA-1)){
                            //per ogni lunghezza minima dell parola
                            int lung = m+1-i;
                            if(lung>2){
                                jollyGiocato=false;
                                //cerco le parole lunghe lung-2
                                //SELEZIONA PAROLA DALLA MAPPA PER LA LUNGHEZZA
                                foreach (QString parolung, *diz[lungh(lung)]){
                                    QVector<Lettera> lett(lettGioc);
                                    bool inserita = false;
                                    bool ok=false;
                                    azzClick();
                                    for(int j=0; j<lung; ++j){
                                        if(cell[crd][i+j]->isDefault()){
                                            //se vuota guardo le lettere del giocatroe
                                            for(int s=0; s<lett.size(); ++s){
                                                //controllo se la lettera
                                                if(parolung[j].toUpper()==lett[s].getValore() && lett[s].getOccorrenze()>0){
                                                    //inserisco la lettera
                                                    sugInserisciLettera(cell[crd][i+j], lett[s]);
                                                    incClick();
                                                    lett[s].decrementa();
                                                    //setto che va bene
                                                    if(inserita==false)
                                                        filaX(i+j);
                                                    inserita=true;
                                                    ok=true;
                                                    parolaPunteggio(lett[s], cell[crd][i+j]);
                                                    break;
                                                }
                                                else if(s==lett.size()-1 && jolly!=INVALID && lett[jolly].getOccorrenze()>0){
                                                    //Se non ha trovato riscontro nelle lettere del giocatore ma ha 1 o 2 jolly usa quello
                                                    jollyGiocato=true;
                                                    Lettera* jol = new Lettera();
                                                    jol->setValore(parolung.toUpper().at(j).toLatin1());
                                                    //inserisco la lettera
                                                    sugInserisciLettera(cell[crd][i+j], *jol);
                                                    incClick();
                                                    lett[jolly].decrementa();
                                                    //setto che va bene
                                                    if(inserita==false)
                                                        filaX(i+j);
                                                    inserita=true;
                                                    ok=true;
                                                    parolaPunteggio(*jol, cell[crd][i+j]); //utilizza la lettera della parola (siccome è jolly)
                                                    delete jol;
                                                    jol = nullptr;
                                                    break;
                                                }
                                                else if((s==lett.size()-1 && jolly!=INVALID && lett[jolly].getOccorrenze()==0)||(s==lett.size()-1 && jolly==INVALID)){
                                                    ok=false;
                                                    break;
                                                }
                                            }
                                            //appena trovo la prima lettera della parola che non corrisponde a quelle che ha il giocatore: passo alla prossima parola
                                            if(ok==false)
                                                break;
                                        }
                                        else{
                                            //se non lo è confronto la lettera nella stessa posizione
                                            if(cell[crd][i+j]->getLett().getValore()==parolung[j].toUpper()){
                                                parolaPunteggio(cell[crd][i+j]->getLett(), cell[crd][i+j]); // uso la lettera presente per il punteggio di quella casella
                                                ok=true;
                                            }
                                            else{
                                                //se la lettera presente nella griglia non è presente nella stessa posizione nella parola: passo alla prossima parola
                                                ok=false;
                                                break;
                                            }

                                        }
                                    }
                                    //-----------------------------------------
                                    //se la parola si puo formare
                                    if (ok==true && inserita==true){
                                        //la parola si puo formare
                                        punt = punt*molt_tot;
                                        sugBonusPunteggi(parolung,lett);
                                        adiacPunt = 0;
                                        //CONTROLLO LE ADIACENTI
                                        if (sugAdiacentiIsOk(parolung, fila(), i)==true){
                                            //punteggio
                                            if(maxPunteggio(punt,parolung)==true){
                                                bestX=i;
                                                bestY=filaY();
                                                bestFila='y';
                                                bestAdiacPunt = adiacPunt;
                                            }
                                            puntToEmpty();
                                            molt_totToEmpty();
                                            sugAnnullaTutto();
                                        }
                                        else{
                                            adiacPunt = 0;
                                            puntToEmpty();
                                            molt_totToEmpty();
                                            sugAnnullaTutto();
                                        }
                                    }
                                    else{
                                        //non si puo formare
                                        puntToEmpty();
                                        molt_totToEmpty();
                                        sugAnnullaTutto();
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }
    }
}

bool MainWindow::maxPunteggio(int p, const QString &par){
    //il punteggio p è comprendente anche di quelle create con gli incroci adiacenti (in caso)
    if(p>maxPunt){
        //se il punteggio appena totalizzato è maggiore
        maxPunt=p;
        bestParola=par;
        return true;
    }
    else if(p==maxPunt && par.length()<bestParola.length()){
        //se i punteggi sono uguali seleziona la parola piu corta
        maxPunt=p;
        bestParola=par;
        return true;
    }
    else
        return false;
}

//funzioni inserimento copiate (preferisco non adattare quelle gia presneti al suggerimento
//quindi rifaccio quelle che mi servono copiando e incollando e adattando
void MainWindow::sugInserisciLettera(Cella* c, Lettera& lett){
    *selezione = lett;
    if(!(selezione->isEmpty())){ //se la casella selezionata non è vuota
        if (c->isEmpty()){ //se la cella del campo da gioco è vuota
            //inserisce la lettera
            c->setLett(*selezione);
            c->setInserita();   //per tutte le altre
        }
    }
    else{
        decClick();
    }
}

bool MainWindow::sugAdiacentiIsOk(QString &par, char fil, int inizio){
    //FUNZIONE CHE CONTROLLA TUTTI GLI INCROCI CON LE ALTRE PAROLE
    //E SE ANDANDO AD INCROCIARE LETTERE ADIACENTI VIENE FORMATA UNA NUOVA PAROLA, LA CONTO NEL PUNTEGGIO
    fila(fil);
    int x = filaX();
    int y = filaY();
    int ok=0;
    int start = inizio;   //salvo l'inizio della parola

    if(fila()=='x'){      //se è colonna
        //scambio riga per colonna per far controllare gli adiacenti
        fila('y');
        //colonna è fissa (x) riga cambia (i)
        for(int i=start; i<(start+par.length()); ++i){
            //controlli per la lettera se ha non lettere adiacenti
            if(x==0){                   //bordo alto
                if(cell[i][x+1]->isDefault())
                    ++ok;
                else{
                    bool dentro=false;
                    if(cell[i][x]->isDentro()){
                        cell[i][x]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(i,filaX())){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            sugAggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok; //conta
                    }

                    if(dentro==true)
                        cell[i][x]->setDentro();
                }
            }
            else if(x==DIM_GRIGLIA-1){  //bordo basso
                if(cell[i][x-1]->isDefault())
                    ++ok;
                else{
                    bool dentro=false;
                    if(cell[i][x]->isDentro()){
                        cell[i][x]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(i,filaX())){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            sugAggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok; //conta
                    }

                    if(dentro==true)
                        cell[i][x]->setDentro();
                }
            }
            else{                       //qualsiasi altro punto
                if(cell[i][x-1]->isDefault() && cell[i][x+1]->isDefault())
                    ++ok;
                else{
                    bool dentro=false;
                    if(cell[i][x]->isDentro()){
                        cell[i][x]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(i,filaX())){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            sugAggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok; //conta
                    }
                    if(dentro==true)
                        cell[i][x]->setDentro();
                }
            }

        }
        fila('x'); //rimetto apposto la fila
    }
    else if(fila()=='y'){  //se è riga
        //scambio riga per colonna per far controllare gli adiacenti
        fila('x');
        //riga è fissa (y) colonna cambia (i)
        for(int i=start; i<(start+par.length()); ++i){
            //controlli per la lettera se ha non lettere adiacenti
            if(y==0){
                if(cell[y+1][i]->isDefault())
                    ++ok;
                else {
                    bool dentro=false;
                    if(cell[y][i]->isDentro()){
                        cell[y][i]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(filaY(),i)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            sugAggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok;
                    }
                    if(dentro==true)
                        cell[y][i]->setDentro();
                }
            }
            else if(y==DIM_GRIGLIA-1){
                if(cell[y-1][i]->isDefault())
                    ++ok;
                else {
                    bool dentro=false;
                    if(cell[y][i]->isDentro()){
                        cell[y][i]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(filaY(),i)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            sugAggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok;
                    }

                    if(dentro==true)
                        cell[y][i]->setDentro();
                }
            }
            else{
                if(cell[y-1][i]->isDefault() && cell[y+1][i]->isDefault())
                    ++ok;
                else {
                    bool dentro=false;
                    if(cell[y][i]->isDentro()){
                        cell[y][i]->setInserita(); //
                        dentro=true;
                    }
                    //controllo per la lettera se ha lettere adiacenti
                    //con filaisok() mi salva la nuova parola incrociata
                    if (filaIsOk(filaY(),i)){  //controllo se esiste la parola incrociata
                        //se la cella era gia dentro, non ha formato una nuova parola con le lettere adiacenti quindi il punteggio non va aggiornato
                        if (dentro==false)
                            sugAggiornaPunteggio(punt);    //aggiorno il punteggio
                        ++ok;
                    }

                    if(dentro==true)
                        cell[y][i]->setDentro();
                }
            }
        }
        fila('y'); //rimetto apposto la fila
    }
    //se ok è uguale al numero di lettere della parola va bene
    if(ok==par.length())
        return true;
    else{
        return false;
    }
}

void MainWindow::sugAnnullaTutto(){
    //qui annulli tutto: Se la parola non va bene | Se preme annulla tutto
    if(click()>1){
        jollyGiocato=false;
        int x = filaX();
        int y = filaY();
        if(fila()=='x'){       //se è colonna
            if(x==filaX()){
                //colonna è fissa (x) riga cambia (i)
                for(int i=0; i<DIM_GRIGLIA; ++i){
                    if(cell[i][x]->isInserita()){
                        cell[i][x]->toEmpty();
                    }
                }
            }
        }
        else if(fila()=='y'){   //se è riga
            if(y==filaY()){
                //riga è fissa (y) colonna cambia (i)
                for(int i=0; i<DIM_GRIGLIA; ++i){
                    if(cell[y][i]->isInserita()){
                        cell[y][i]->toEmpty();
                    }
                }
            }
        }
    }
    else if(click()==1){
        int x = filaX();
        int y = filaY();
        //lettera singola
        cell[y][x]->toEmpty();
    }
    azzClick();
    selezToNull();
}

void MainWindow::sugBonusPunteggi(QString &par, QVector<Lettera> &l){
    if (jollyGiocato==false)
        punt+=10;
    //---------------------
    if (par.length()==6)
        punt+=10;
    else if(par.length()==7)
        punt+=30;
    else if(par=="scarabeo" || parola=="scarabei")
        punt+=100;
    else{
        bool vuoto=true;
        for(int k=0; k<l.size(); ++k){
            //se trovo una lettera nelle caselle del giocatore significa che non le ha usate tutte
            if(!(l[k].isFinish())){
                vuoto=false;
                break;
            }
        }
        if(vuoto==true)
            punt+=50;
    }
}

void MainWindow::sugAggiornaPunteggio(int punteggio){
    adiacPunt *= punteggio;
}
