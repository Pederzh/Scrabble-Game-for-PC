#include "dialog_giocatori.h"
#include "ui_dialog_giocatori.h"
#include "qmessagebox.h"

Dialog_Giocatori::Dialog_Giocatori(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Giocatori)
{
    ui->setupUi(this);
}

Dialog_Giocatori::~Dialog_Giocatori()
{
    delete ui;
}

void Dialog_Giocatori::on_pushButton_clicked()
{
    //controllo se il dato inserito è valido e se il numero di giocatori è <=4
    //passa il numero di giocatori a..
    if (ui->spinBox->value()<=4){
        emit numeroGioc(ui->spinBox->value());
            Dialog_Giocatori::close();
}
    else{
        QMessageBox messageBox;
        messageBox.critical(0,"Errore","Numero massimo di giocatori è 4!");
        messageBox.setFixedSize(500,200);
            Dialog_Giocatori::close();
    }
}
