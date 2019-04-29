#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QLineEdit>
#include <QScreen>
#include "Setting/qwdialogconfig.h"


/**********************************************************************
作者：刘维球
时间：2019年4月29日14:39:24
功能：退出程序
参数1：
参数2：
***********************************************************************/



void MainWindow::closeEvent(QCloseEvent *event)
{ //窗口关闭时询问是否退出
    QMessageBox::StandardButton result=QMessageBox::question(this, "确认", "确定要退出本程序吗？",
                                                             QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                                                             QMessageBox::No);
    if (result==QMessageBox::Yes)
    {
        event->accept();

        if (subthread_1->isRunning())
        {
            subthread_1->stopThread1();
            subthread_1->wait();
        }
        if (subthread_2->isRunning())
        {
            subthread_2->stopThread2();
            subthread_2->wait();
        }
        if (subthread_3->isRunning())
        {
            subthread_3->stopThread3();
            subthread_3->wait();
        }
        if (subthread_4->isRunning())
        {
            subthread_4->stopThread4();
            subthread_4->wait();
        }

        db->disconnDb();
        db_1->disconnDb();
        db_2->disconnDb();
        db_3->disconnDb();
        db_4->disconnDb();

    }

    else
    {
        event->ignore();
    }

}






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QScreen * screen  =  QGuiApplication::primaryScreen();//刘维球，获取屏幕的大小，2019年4月28日16:20:09
    QRect rect =  screen->availableGeometry();
    this->setGeometry(50 ,50 ,rect.width()-100, rect.height()-100); //刘维球，设置界面的大小，2019年4月28日08:24:29

    db = new Database("mainUI");//根据名称来决定数据库连接
    db_1 = new Database("First");
    db_2 = new Database("Second");
    db_3 = new Database("Third");
    db_4 = new Database("Fourth");










}

MainWindow::~MainWindow()
{
    delete ui;
}


/*********************************************************************
作者：刘维球
时间：2019年4月28日17:26:04
功能：获取订单配置
参数1：
参数2：
***********************************************************************/


void MainWindow::on_actionsetting_triggered()
{
    QString dlgTitle="输入密码对话框";
    QString txtLabel="请输入密码";
    QString defaultInput="";
    QLineEdit::EchoMode echoMode=QLineEdit::Password;//正常文字输入

    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode, defaultInput, &ok);//刘维球：自定义窗体，2019年4月28日18:02:07

    if (ok && text == "JIMI")
    {
        QWDialogConfig *configDialog = new QWDialogConfig(this);
        Qt::WindowFlags    flags=configDialog->windowFlags();
        configDialog->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

        int ret=configDialog->exec();
        if(ret== QDialog::Accepted ) ////刘维球，配置接受，
        {

        }

        delete configDialog ;
    }
    else {
        QMessageBox::warning(this, "错误提示", "密码不能为空"); ;
    }
}
