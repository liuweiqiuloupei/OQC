#include "qwdialogDelete.h"
#include "ui_qwdialogDelete.h"

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
QString QWDialogDelete::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray;

    btArray.append(str);//加入原始字符串

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法

    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值

    QString md5 =resultArray.toHex();//转换为16进制字符串

    return  md5;
}


void QWDialogDelete::closeEvent(QCloseEvent *event)
{ //窗口关闭事件，关闭时释放本窗口
    MainWindow *parWind = (MainWindow*)parentWidget(); //获取父窗口指针

    //parWind->setActLocateEnable(true);//使能 actTab_Locate
    parWind->EnbtnDel();//当关闭窗口，btnAdd/使能
   // username = parWind->getLogname();
    parWind->setDlgDelNull(); //将窗口指针设置为NULL
}

void QWDialogDelete::showEvent(QShowEvent *event)
{//窗口显示事件
    MainWindow *parWind = (MainWindow*)parentWidget();//获取
    //parWind->setActLocateEnable(false);
}

QWDialogDelete::QWDialogDelete(QWidget *parent) :
    QDialog(parent), ui(new Ui::QWDialogDelete)
{//构造函数
    ui->setupUi(this);
    QRegExp regx("[a-zA-Z0-9]+$");//只能输入数字和字符串
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit_username);

    ui->lineEdit_username->setValidator( validator );

    bool sqlok = db.connectDb();
    qDebug() << sqlok << endl;
 }

QWDialogDelete::~QWDialogDelete()
{//析构函数
    //QMessageBox::information(this,"提示","单元格定位对话框被删除");
    delete ui;
}

void QWDialogDelete::on_btnOk_clicked()
{//添加用户是否成功

    QSqlQuery query;
//    pwd =ui->
//    QString sqlstatement(QString("select pwd from GPSTest.dbo.Test_Engineers2 where username = '%1'")
//                         .arg(username));//执行搜索
//    query.exec(sqlstatement);
//    if(query.next())
//    {

//       QString sqlpwd  = query.value(0).toString();
//       QString sqlpwdMd5  = encrypt(pwd);
//       if(sqlpwd == sqlpwdMd5)
//            QMessageBox::warning(this, "错误提示", "用户名或密码错误");
//    }

}

void QWDialogDelete::on_btnClose_clicked()
{
    this->reject();
}
