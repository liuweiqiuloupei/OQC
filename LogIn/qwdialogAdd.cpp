//#include "qwdialoglocate.h"
#include "qwdialogAdd.h"
#include "ui_qwdialogAdd.h"

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
QString QWDialogAdd::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray;

    btArray.append(str);//加入原始字符串

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法

    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值

    QString md5 =resultArray.toHex();//转换为16进制字符串

    return  md5;
}

void QWDialogAdd::closeEvent(QCloseEvent *event)
{ //窗口关闭事件，关闭时释放本窗口
    Q_UNUSED(event)
    MainWindow *parWind = (MainWindow*)parentWidget(); //获取父窗口指针
    //parWind->setActLocateEnable(true);//使能 actTab_Locate
    parWind->EnbtnAdd();//当关闭窗口，btnAdd/使能
    parWind->setDlgAddNull(); //将窗口指针设置为NULL
}

void QWDialogAdd::showEvent(QShowEvent *event)
{//窗口显示事件
    Q_UNUSED(event)
    MainWindow *parWind = (MainWindow*)parentWidget();//获取
    //parWind->setActLocateEnable(false);
}

QWDialogAdd::QWDialogAdd(QWidget *parent) :
    QDialog(parent), ui(new Ui::QWDialogAdd)
{//构造函数
    ui->setupUi(this);
    AddDb = new Database("Adddb");
    QRegExp regx("[a-zA-Z0-9]+$");//只能输入数字和字符串
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit_username);
    QValidator *validator1 = new QRegExpValidator(regx, ui->lineEdit_pwd);
    QValidator *validator2 = new QRegExpValidator(regx, ui->lineEdit_calipwd);
    ui->lineEdit_username->setValidator( validator );
    ui->lineEdit_pwd->setValidator( validator1 );
    ui->lineEdit_calipwd->setValidator( validator2 );
    AddDb->connectDb();

    //bool sqlok = db.connectDb();

 }

QWDialogAdd::~QWDialogAdd()
{//析构函数
    //QMessageBox::information(this,"提示","单元格定位对话框被删除");
    delete ui;
}

void QWDialogAdd::on_btnOk_clicked()
{//添加用户是否成功
    int i;
    QSqlQuery query(AddDb->getDBName());
    username = ui->lineEdit_username->text().trimmed();//去掉空格，获取用户名
    QString sqlstatement(QString("select username from GPSTest.dbo.Test_Engineers where TestEngineer_name = '%1'")
                         .arg(username));//执行搜索
    query.exec(sqlstatement);
    if(query.next())
    {
        QMessageBox::warning(this, "错误提示", "用户名已注册");//
        qDebug() << "用户名已注册" << endl;
        ui->lineEdit_username->setStyleSheet("color:red"); //
    }
    else
    {
        if(username.isEmpty())
        {
            QMessageBox::warning(this, "错误提示", "用户名不能为空");//
        }
        else
        {
            i = ui->combB_logGrade->currentIndex();
            i=(i==1)?0:1;//三目运算符
            pwd = ui->lineEdit_pwd->text();
            calipwd = ui->lineEdit_calipwd->text();

            if(pwd.isEmpty() && calipwd.isEmpty())
            {
                QMessageBox::warning(this, "错误提示", "密码不能为空");//
                qDebug() << "密码为空" << endl;
            }
            else
            {
                if(calipwd != pwd )
                {
                    ui->lineEdit_calipwd->setStyleSheet("color:red"); //
                }
                else
                {
                    ui->lineEdit_calipwd->setStyleSheet("color:black");
                    QString pwdMD5 = encrypt(pwd);
                    QString currentTimeStr= QDateTime::currentDateTime().toString("yyyy-MM-dd  HH:mm:ss");
                    query.prepare("insert into GPSTest.dbo.Test_Engineers(TestEngineer_name,TestEngineer_pwdMD5,TestEngineer_Grade,createTime) values (?, ?, ?,?)");

                    QVariantList TestEngineer_name;//批量插入
                    TestEngineer_name<< username  ;
                    query.addBindValue(TestEngineer_name);

                    QVariantList TestEngineer_pwdMD5;
                    TestEngineer_pwdMD5<<  pwdMD5 ;
                    query.addBindValue(TestEngineer_pwdMD5);

                    QVariantList TestEngineer_Grade;
                    TestEngineer_Grade <<i;
                    query.addBindValue(TestEngineer_Grade);

                    QVariantList createTime;
                    createTime <<  currentTimeStr ;
                    query.addBindValue(createTime);

                    if(query.execBatch())
                    {
                        QMessageBox::information(this, "信息提示", "添加用户成功");
                        qDebug() << "添加用户成功" << endl;
                        //this->accept();
                    }
                    else
                    {
                        QMessageBox::information(this, "信息提示", "添加用户失败");
                        qDebug() << "添加用户失败" << endl;

                    }
                }

            }

        }
    }
}

void QWDialogAdd::on_btnClose_clicked()
{
    this->reject();
}
