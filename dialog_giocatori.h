#ifndef DIALOG_GIOCATORI_H
#define DIALOG_GIOCATORI_H

#include <QDialog>

namespace Ui {
class Dialog_Giocatori;
}

class Dialog_Giocatori : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Giocatori(QWidget *parent = 0);
    ~Dialog_Giocatori();

private slots:
    void on_pushButton_clicked();

signals:
    void numeroGioc(int numGioc);

private:
    Ui::Dialog_Giocatori *ui;
};

#endif // DIALOG_GIOCATORI_H
