#ifndef QWDIALOGUPDATE_H
#define QWDIALOGUPDATE_H

#include <QDialog>
#include <database.h>
namespace Ui {
class QWDialogUpdate;
}

class QWDialogUpdate : public QDialog
{
    Q_OBJECT
private:

public:
    explicit QWDialogUpdate(QWidget *parent = 0);
    ~QWDialogUpdate();
    void closeEvent(QCloseEvent *event);//关闭事件

    QString encrypt(const QString &str);
private slots:
    void on_btnOk_clicked();
    void on_btnClose_clicked();
    void getLogName(const QString str);//
    void getGrade( const int user_Grade);
    //void sendTestInfo(QString ProductName);
    //sendCmdSet()
private:
    int user_Grade; //管理权限
    Ui::QWDialogUpdate *ui;
    QString username;
    QString pwd;
    QString calipwd;
    Database *UpdDb;
};

#endif // QWDIALOGLOCATE_H
