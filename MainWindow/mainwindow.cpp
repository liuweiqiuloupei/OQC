#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QInputDialog>
#include <QLineEdit>
#include <QScreen>
#include "Setting/qwdialogconfig.h"
#include <QLabel>


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

//        if (subthread_1->isRunning())
//        {
//            subthread_1->stopThread1();
//            subthread_1->wait();
//        }
//        if (subthread_2->isRunning())
//        {
//            subthread_2->stopThread2();
//            subthread_2->wait();
//        }
//        if (subthread_3->isRunning())
//        {
//            subthread_3->stopThread3();
//            subthread_3->wait();
//        }
//        if (subthread_4->isRunning())
//        {
//            subthread_4->stopThread4();
//            subthread_4->wait();
//        }

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


//刘维球：获取屏幕大小，以适应不同的分辨率，2019年5月7日11:44:47
    QScreen * screen  =  QGuiApplication::primaryScreen();//刘维球，获取屏幕的大小，2019年4月28日16:20:09
    QRect rect =  screen->availableGeometry();
    this->setGeometry(50 ,50 ,rect.width()-100, rect.height()-100); //刘维球，设置界面的大小，2019年4月28日08:24:29

//刘维球：状态栏，2019年5月7日11:44:44
    localHost = QHostInfo::localHostName ();
    IPAddress = getIPAddress(localHost);

    QStatusBar *sBar = statusBar ();
    sBar->addWidget (new QLabel("主机名: "+localHost+"     ", this));//刘维球，主机名，2019年5月7日14:14:53
    sBar->addWidget (new QLabel("IP: "+getIPAddress(localHost)+"  ", this));//刘维球，IP地址，2019年5月7日14:14:53

    OQC_Engineer = "测试人员: " + OQC_Engineer +"    " ;
    labOQC_Engineer =  new QLabel(OQC_Engineer,this); //刘维球，OQC测试工程师，2019年5月7日14:14:53
    sBar->addWidget (labOQC_Engineer);

    labtest_Num = new QLabel ("测试总数："+test_Num+"     ", this) ;//刘维球，测试总数，2019年5月7日14:14:53
    sBar->addWidget (labtest_Num);

    labpass_Num = new QLabel ("成功总数："+pass_Num+"     ", this) ;//刘维球，成功总数，2019年5月7日14:14:53
    labpass_Num->setStyleSheet("QLabel{color:green;}");   //刘维球，成功的背景就是绿色
    sBar->addWidget (labpass_Num);

    labfail_Num = new QLabel ("失败总数："+fail_Num+"     ", this) ;//刘维球，失败总数，2019年5月7日14:14:53
    labfail_Num->setStyleSheet("QLabel{color:red;}");   //刘维球，失败的背景就是绿色
    sBar->addWidget (labfail_Num);

    labpercent_Num = new QLabel ("合格率："+pass_percent+"     ", this) ;//刘维球，合格率总数，2019年5月7日14:14:53
    sBar->addWidget (labpercent_Num);


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

    test_Num = "1" ;
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

/**********************************************************************
作者：刘维球
时间：2019年5月7日17:20:54
功能：刷新状态栏
参数1：
参数2：
***********************************************************************/
void MainWindow::updateStatus()
{
    if(test_Num == 0)
    {
        labtest_Num->setText("测试总数："+test_Num+"     ");
        labpass_Num->setText("成功总数："+pass_Num+"     ");
        labfail_Num->setText("失败总数："+fail_Num+"     ");
        labpercent_Num->setText("合格率："+pass_percent+"     ");
    }
}

/**********************************************************************
作者：梁家榕，
时间：2019年5月7日11:51:58
功能：获取IP地址
参数1：localHost ，主机名
参数2：
***********************************************************************/

QString MainWindow::getIPAddress(QString localHost)
{
    QString ipAddr;
    QHostInfo info = QHostInfo::fromName (localHost);
    info.addresses ();          //QHostInfo的address函数获取本机Ip地址
    foreach(QHostAddress address, info.addresses ())                //如果存在多条ip地址（ipv4和ipv6）
    {
        if(address.protocol () == QAbstractSocket::IPv4Protocol)    //取第一条ipv4地址
        {
            qDebug () << address.toString ();
            ipAddr = address.toString ();
            break;
        }
    }
    return ipAddr;
}
/**********************************************************************
作者：刘维球
时间：2019年5月7日17:16:05
功能：设置登录名
参数1：
参数2：
***********************************************************************/

void MainWindow::setUserName(QString username)
{
    OQC_Engineer = username ;
    labOQC_Engineer->setText("测试人员: " + OQC_Engineer + "    "); //显示模型索引的行和列号
}
