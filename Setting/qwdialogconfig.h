#ifndef QWDIALOGCONFIG_H
#define QWDIALOGCONFIG_H

#include <QDialog>
#include <QShowEvent>
#include "DataBase/database.h"
#include <QShowEvent>

namespace Ui {
class QWDialogConfig;
}

class QWDialogConfig : public QDialog
{
    Q_OBJECT

public:
    explicit QWDialogConfig(QWidget *parent = 0);
    ~QWDialogConfig();


private slots:

    void on_btnok_clicked();

    void on_btncancel_clicked();

private:
    //void showEvent(QShowEvent *event);//当界面显示的时候

private:
    Ui::QWDialogConfig *ui;
    Database *Product_db;//设置机型号

signals:



};

#endif // QWDIALOGSIZE_H
