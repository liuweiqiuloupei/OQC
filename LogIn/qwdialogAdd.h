#ifndef QWDIALOGADD_H
#define QWDIALOGADD_H

#include <QDialog>
#include <database.h>
namespace Ui {
class QWDialogAdd;
}

class QWDialogAdd : public QDialog
{
    Q_OBJECT
private:



public:
    explicit QWDialogAdd(QWidget *parent = 0);
    ~QWDialogAdd();
    void closeEvent(QCloseEvent *event);//关闭事件
    void showEvent(QShowEvent *event);  //显示事件
    QString encrypt(const QString &str);
private slots:
    void on_btnOk_clicked();
    void on_btnClose_clicked();

private:
    Ui::QWDialogAdd *ui;
    QString username;
    QString pwd;
    QString calipwd;
    Database *AddDb;
};

#endif // QWDIALOGLOCATE_H
