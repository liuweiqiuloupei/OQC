#ifndef QDLGLOGIN_H
#define QDLGLOGIN_H

#include <QDialog>
#include <QSqlDatabase>
#include <QString>
#include  "DataBase/database.h"
#include "INI/ini.h"

namespace Ui {
class dlgLogin;
}

class QDlgLogin : public QDialog
{
    Q_OBJECT
private:
    int m_tryCount=0; //试错次数
    QString encrypt(const QString& str);//字符串加密
    Database *logDB;
protected:

public:
    explicit QDlgLogin(QWidget *parent = 0);

    ~QDlgLogin();
    QString getUserName();//返回测试工程师的姓名
    int getUserGrade();//刘维球，获取当前登录的用户名，2019年5月7日15:33:17
private slots:
    void on_btnOK_clicked(); //OK按键
    void on_btnCancel_clicked();    
private:
    Ui::dlgLogin *ui;
    QString pwd;
    QString username;
    int UserGrade;//刘维球，用户级别，2019年5月7日15:33:42
    


};

#endif // DLGLOGIN_H
