#ifndef QWDIALOGCONFIG_H
#define QWDIALOGCONFIG_H

#include <QDialog>
#include <QShowEvent>
#include "DataBase/database.h"
#include <QShowEvent>
#include <QCompleter>       //自动补全
#include "MainWindow/mainwindow.h"

namespace Ui {
class QWDialogConfig;
}

class QWDialogConfig : public QDialog
{
    Q_OBJECT

public:
    explicit QWDialogConfig(QWidget *parent = 0);
    ~QWDialogConfig();

    void add_ConfigDataTable(); //梁家榕：配置信息加入到数据表格
    void Proname_Load_All();        //梁家榕：全部订单加载
    void Proname_Load_Part(const QString &Str);   //梁家榕：部分订单加载，根据时间
private slots:

    void on_btnok_clicked();
    void on_btncancel_clicked();

    void on_comboBox_Proname_activated(const QString &arg1);    //梁家榕：选择订单
    void on_radioButton_clicked(bool checked);      //梁家榕：选择订单查询方式：精准、模糊
    void on_pushButton_clicked();       //精准查询
    void on_comboBox_SelectTime_activated(const QString &arg1); //梁家榕：选择订单查询时间

private:
    //void showEvent(QShowEvent *event);//当界面显示的时候

private:
    Ui::QWDialogConfig *ui;
    Database *Product_db;//设置机型号


    Database db;                //梁家榕：数据库操作对象
    QCompleter *Completer;      //梁家榕：自动补全对象
    MainWindow *myWin;          //梁家榕：主界面对象

    QDateTime LocalDateTime;    //梁家榕：本地时间对象
    QString SelectDateTime;     //梁家榕：查询的日期时间
    QStringList ConfigTime;     //梁家榕：配置查询时间
    QStringList Configlist;     //梁家榕：配置表链接信息

};

#endif // QWDIALOGSIZE_H
