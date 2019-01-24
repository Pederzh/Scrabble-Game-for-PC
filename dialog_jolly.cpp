#include "dialog_jolly.h"
#include "ui_dialog_jolly.h"
#include "QMessageBox"

Dialog_jolly::Dialog_jolly(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_jolly)
{
    ui->setupUi(this);
}

Dialog_jolly::~Dialog_jolly()
{
    delete ui;
}

void Dialog_jolly::on_pushButton_clicked()
{
    if (ui->lineEdit->text().length()==1)
        if(ui->lineEdit->text().at(0).toLatin1()>='a' && ui->lineEdit->text().at(0).toLatin1()<='z'){
            emit letteraJolly(ui->lineEdit->text().at(0).toUpper().toLatin1());
            Dialog_jolly::close();
        }
        else if (ui->lineEdit->text().at(0).toLatin1()>='A' && ui->lineEdit->text().at(0).toLatin1()<='Z'){
            emit letteraJolly(ui->lineEdit->text().at(0).toLatin1());
            Dialog_jolly::close();
        }
        else{
            QMessageBox messageBox;
            messageBox.critical(0,"Errore","Inserisci una lettera!");
            messageBox.setFixedSize(500,200);
        }
    else{
        QMessageBox messageBox;
        messageBox.critical(0,"Errore","Inserisci una sola lettera!");
        messageBox.setFixedSize(500,200);
    }
}
