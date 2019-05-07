#include "qdlglogin.h"
#include "ui_qdlglogin.h"

#include    <QMouseEvent>
#include    <QSettings>
#include    <QMessageBox>
#include    <QByteArray>
#include    <QCryptographicHash>
#include    <QSqlQuery>
#include    <QDebug>
#include    <QSqlRecord>
QString QDlgLogin::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray;

    btArray.append(str);//加入原始字符串

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法

    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值

    QString md5 =resultArray.toHex();//转换为16进制字符串

    return  md5;
}

QDlgLogin::QDlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgLogin)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);//无边框，但是在任务显示对话框标题

    logDB = new Database("logUI");

    logDB->connectLogInfoDb();
    if(logDB->connectLogInfoDb()) //lwq:连接数据库是否成功
    {
        logDB->createLogTable("dbo.OQC_Engineers");
    }
    else
    {
        QMessageBox::warning(this,"连接数据库","连接失败");
    }

}

QDlgLogin::~QDlgLogin()
{

    delete ui;
}

QString QDlgLogin::getUserName()
{
    return this->username ;
}

int QDlgLogin::getUserGrade()
{
    return this->UserGrade ;
}

void QDlgLogin::on_btnOK_clicked()
{//"确定"按钮相应

    username=ui->editUser->text().trimmed();
    pwd = ui->editPSWD->text();
    //qDebug() << encrypt(pwd) << endl ; //刘维球，

    QString selectstatement(QString("select UserPWDMD5 from GPSTest.dbo.MES_OQC_Engineers where UserName = '%1'").arg(username)) ;
    QSqlQuery query(this->logDB->getDBName());
    query.exec(selectstatement);
    if(query.next())
    {
       QString sqlpwd  = query.value(0).toString();
       QString sqlpwdMd5  = encrypt(pwd);
       qDebug() << sqlpwdMd5 << endl ;
       if(sqlpwd == sqlpwdMd5)
       {
          logDB->disconnDb();
          this->accept();
       }
    }
    else
    {
       QMessageBox::warning(this, "错误提示", "用户名或密码错误");
    }
}


void QDlgLogin::on_btnCancel_clicked()
{
    this->reject();
}

