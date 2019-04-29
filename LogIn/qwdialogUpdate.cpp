#include "qwdialogUpdate.h"
#include "ui_qwdialogUpdate.h"

#include    <QCloseEvent>
#include    "mainwindow.h"
#include    <QMessageBox>
#include    <QCryptographicHash>
#include    <QSqlQuery>
#include    <QRegExp>
#include    <QValidator>
#include    <QFont>
#include    <QComboBox>
#include    <QDateTime>
#include    <QSqlQuery>
QString QWDialogUpdate::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray;

    btArray.append(str);//加入原始字符串

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法

    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值

    QString md5 =resultArray.toHex();//转换为16进制字符串
    return  md5;
}

QWDialogUpdate::QWDialogUpdate(QWidget *parent) :
    QDialog(parent), ui(new Ui::QWDialogUpdate)
{//构造函数
    ui->setupUi(this);

    // db = new Database("AddDb");
    QRegExp regx("[a-zA-Z0-9]+$");//只能输入数字和字符串
    UpdDb = new Database("UpdDb");
    QValidator *validator1 = new QRegExpValidator(regx, ui->lineEdit_pwd);
    QValidator *validator2 = new QRegExpValidator(regx, ui->lineEdit_calipwd);
    ui->lineEdit_pwd->setValidator( validator1 );
    ui->lineEdit_calipwd->setValidator( validator2 );

    UpdDb->connectDb();
    //db.connectDb();
}

QWDialogUpdate::~QWDialogUpdate()
{//析构函数
    //QMessageBox::information(this,"提示","单元格定位对话框被删除");
    delete ui;
}

void QWDialogUpdate::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    MainWindow *parWind = (MainWindow*)parentWidget(); //获取父窗口指针
    //parWind->setActLocateEnable(true);//使能 actTab_Locate
    parWind->EnbtnUpd();//当关闭窗口，btnAdd/使能
}

void QWDialogUpdate::on_btnOk_clicked()
{//添加用户是否成功

    username = ui->lineEdit_pwd_2->text();
    //qDebug() <<"username: " << username << endl;
    pwd = ui->lineEdit_pwd->text();//输入的密码
    calipwd = ui->lineEdit_calipwd->text();
    if(!pwd.isEmpty())
    {
        QString selectstatement(QString("select TestEngineer_pwdMD5, TestEngineer_name from GPSTest.dbo.Test_Engineers where TestEngineer_name = '%1'").arg(username)) ;
        QSqlQuery query(UpdDb->getDBName());
        query.exec(selectstatement);
        if(query.next())
        {
            QString sqlpwd  = query.value(0).toString();//数据库中查询到的密码
            QString pwdMd5  = encrypt(pwd);
            if(sqlpwd == pwdMd5)
            {
                QMessageBox::warning(this, "错误提示", "密码已重复");//
                qDebug() << "密码重复" << endl;
            }
            else
            {
                if(pwd !=calipwd)
                {
                    QMessageBox::warning(this, "错误提示", "密码错误，请核对");
                    qDebug() << "密码错误" << endl;
                }
                else
                {
                    QString updatestatement(QString("update GPSTest.dbo.Test_Engineers set TestEngineer_pwdMD5 = '%1' where TestEngineer_name = '%2'").arg(pwdMd5).arg(username)) ;

                    QMessageBox::StandardButton result=QMessageBox::question(this, "确认", "确定修改密码？", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

                    if (result==QMessageBox::Yes)
                    {
                        //qDebug() << pwdMd5 << endl;

                        if(!query.exec(updatestatement))
                        {
                            QMessageBox::warning(this, "错误提示", "修改失败");
                            qDebug() << "密码修改失败" << endl;
                        }
                        else{

                            QMessageBox::information(this, "信息提示", "修改成功");
                            qInfo() << "密码修改成功" << endl;
                        }
                    }

                }

            }

        }
        else{

            QString user_name = query.value(1).toString();
            if(user_name.isEmpty())
            {
                QMessageBox::warning(this, "错误提示", "无该测试人员");//
            }
        }

    }
    else
    {
        QMessageBox::warning(this, "错误提示", "请输入密码");//
    }

}


void QWDialogUpdate::getLogName(QString logName)
{
    ui->lineEdit_pwd_2->setText(logName);//
}


void QWDialogUpdate::getGrade(const int user_Grade)
{
    this->user_Grade = user_Grade ;
    if(user_Grade == 1)
    {
        ui->lineEdit_pwd_2->setReadOnly(true);
    }
    else
    {
        ui->lineEdit_pwd_2->setReadOnly(false);
    }

}


void QWDialogUpdate::on_btnClose_clicked()
{
    this->reject();
}
