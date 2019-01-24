#ifndef DIALOG_JOLLY_H
#define DIALOG_JOLLY_H

#include <QDialog>

namespace Ui {
class Dialog_jolly;
}

class Dialog_jolly : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_jolly(QWidget *parent = 0);
    ~Dialog_jolly();

private slots:
    void on_pushButton_clicked();

signals:
    void letteraJolly(char carattere);

private:
    Ui::Dialog_jolly *ui;
};

#endif // DIALOG_JOLLY_H
