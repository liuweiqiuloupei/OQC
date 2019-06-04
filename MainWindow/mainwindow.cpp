#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QInputDialog>
#include <QLineEdit>
#include <QScreen>
#include "Setting/qwdialogconfig.h"         //配置
#include <QLabel>

/*extern QStringList ConfigType;         //梁家榕：指令类型
extern QStringList ConfigName;         //梁家榕：指令名字
extern QStringList ConfigSend;         //梁家榕：发送指令
extern QStringList ConfigPara;         //梁家榕：指令参数
extern QStringList ConfigReturn;       //梁家榕：指令返回值

extern QString SoftWareName;    //梁家榕：订单号
extern QString MachineName;     //梁家榕：机型
extern QString Station;         //梁家榕：所有配置信息*/

/**********************************************************************
作者：刘维球
时间：2019年4月29日14:39:24
功能：退出程序
参数1：
参数2：
***********************************************************************/

void MainWindow::closeEvent(QCloseEvent *event)
{ //窗口关闭时询问是否退出
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("确认"));
    msgBox.setText(tr("确定要退出本程序吗？"));
    QPushButton *Button1 = msgBox.addButton(tr("确定"),QMessageBox::AcceptRole);
    QPushButton *Button2 = msgBox.addButton(tr("取消"),QMessageBox::RejectRole);
    msgBox.exec();
    if (msgBox.clickedButton() == Button1)
    {
        event->accept();
        db->disconnDb();
        db_1->disconnDb();
        db_2->disconnDb();
        db_3->disconnDb();
        db_4->disconnDb();
    }
    else if (msgBox.clickedButton() == Button2)
    {
        event->ignore();
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),m_speech(nullptr)
{
    ui->setupUi(this);

    //刘维球：获取屏幕大小，以适应不同的分辨率，2019年5月7日11:44:47
    QScreen * screen  =  QGuiApplication::primaryScreen();//刘维球，获取屏幕的大小，2019年4月28日16:20:09
    QRect rect =  screen->availableGeometry();
    this->setGeometry(40 ,40 ,rect.width()-150, rect.height()-100); //刘维球，设置界面的大小，2019年4月28日08:24:29

    ui->tabWidget->setGeometry(40 ,40,rect.width()-300, rect.height()-300);////刘维球：设置界面的大小
    //刘维球：从左往右添加状态栏，2019年5月7日11:44:44
    QStatusBar *sBar = statusBar ();
    labtest_Num = new QLabel ("测试总数：" + status+ "          ", this) ;//刘维球，测试总数，2019年5月7日14:14:53
    sBar->addWidget (labtest_Num);

    labpass_Num = new QLabel ("成功总数："+ status+"          ", this) ;//刘维球，成功总数，2019年5月7日14:14:53
    labpass_Num->setStyleSheet("QLabel{color:green;}");   //刘维球，成功的背景就是绿色
    sBar->addWidget (labpass_Num);

    labfail_Num = new QLabel ("失败总数："+ status+"          ", this) ;//刘维球，失败总数，2019年5月7日14:14:53
    labfail_Num->setStyleSheet("QLabel{color:red;}");   //刘维球，失败的背景就是红色
    sBar->addWidget (labfail_Num);

    labpercent_Num = new QLabel ("合格率："+ status+"          ", this) ;//刘维球，合格率总数，2019年5月7日14:14:53
    sBar->addWidget (labpercent_Num);

    //梁家榕：从右往左添加状态栏，2019年5月18日08:21:00
    SoftWareNameShow = new QLabel ("订单号：" + status + "     ", this);    //梁家榕：订单号，2019年5月16日17:04:00
    sBar->addPermanentWidget (SoftWareNameShow);

    localHost = QHostInfo::localHostName ();            //获取主机名
    IPAddress = getIPAddress(localHost);                //获取IP地址
    sBar->addPermanentWidget (new QLabel("IP: "+getIPAddress(localHost)+"     ", this));//梁家榕：IP地址，2019年5月18日08:22:00
    sBar->addPermanentWidget (new QLabel("主机名: "+localHost+"     ", this));//梁家榕：主机名，2019年5月18日08:22:00

    OQC_Engineer = "测试人员: " + OQC_Engineer +"     ";
    labOQC_Engineer =  new QLabel(OQC_Engineer,this); //刘维球，OQC测试工程师，2019年5月7日14:14:53
    sBar->addPermanentWidget (labOQC_Engineer);

    db = new Database("mainUI");//根据名称来决定数据库连接
    db_1 = new Database("First");
    db_2 = new Database("Second");
    db_3 = new Database("Third");
    db_4 = new Database("Fourth");

    //梁家榕：串口功能模块，2019年5月14日15:47:00
    SerialFunc();

    test_Num = 0;   pass_Num = 0;   fail_Num = 0;       //初始化测试、成功、失败总数
    this->setCentralWidget (ui->tabWidget);
    this->setWindowState (Qt::WindowMaximized);

    //语音播报初始化
    m_speech = new voice; //显示语音播放调节框
    m_speech_1 = new voice;
    m_speech_2 = new voice;
    m_speech_3 = new voice;
    m_speech_4 = new voice;


}

MainWindow::~MainWindow()
{
    delete m_speech;
    delete m_speech_1;
    delete m_speech_2;
    delete m_speech_3;
    delete m_speech_4;
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
        //关闭所有串口

        ui->tabWidget->setCurrentIndex(1);////刘维球，设置当前的页面为设置页面
        on_btn_Stop_1_clicked();
        on_btn_Stop_2_clicked();
        on_btn_Stop_3_clicked();
        on_btn_Stop_4_clicked();


        INISETTINGS  iniSettings; //刘维球，初始化设置，2019年4月28日11:07:50
        QString appDir = QApplication::applicationDirPath(); ////刘维球：当前程序路径，2019年4月28日09:21:02
        const QString  configFile = appDir + "/config.ini" ; ////刘维球，config.ini路径
        QFile file(configFile);
        if(file.exists())
        {
            Configlist = iniSettings.getConfigTable(configFile);     //获取链接信息  即ini文件中的信息
            if(db_Config.ConnectDataBase (Configlist))
                qDebug() << "90数据库连接成功";
            else
                QMessageBox::warning(this, tr("错误提示"), tr("数据库连接失败"), "确 定");

            //订单筛选
            Completer = new QCompleter(ui->comboBox_Proname->model(), this);  //使用者，使用者父对象
            ui->comboBox_Proname->setCompleter(Completer);      //使用自动补全功能

            //on_comboBox_Proname_activated(SoftWareName);    //重新查询一次订单配置
            //ui->comboBox_Proname->setEditText (SoftWareName);//编辑区显示上一次配置的订单

        }
        else {
            QMessageBox::warning(this, "错误提示", "查看当前路径下的工程文件是否存在！");
        }

        ui->tab_2->setEnabled (true);
        ui->btnok->setEnabled (true);
        ui->tabWidget->setCurrentIndex (1);     //选择当前页面为配置页面
    }
    else
    {
        QMessageBox::warning(this, tr("错误提示"), tr("密码错误!"), tr("确 定"));
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

    if(test_Num != 0)
    {
        //pass_percent =  pass_Num /test_Num ;
        pass_percent = double(pass_Num)/double(test_Num);
        QString f1 ;
        labtest_Num->setText("测试总数："+ QString("%1").arg(test_Num,10)+"     ");
        labpass_Num->setText("成功总数："+ QString("%1").arg(pass_Num,10)+"     ");
        labfail_Num->setText("失败总数："+ QString("%1").arg(fail_Num,10)+"     ");
        labpercent_Num->setText("合格率："+QString("%L1").arg(pass_percent,0,'f',2)+"     ");//刘维球，合格率，2019年5月7日17:43:02
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

/**********************************************************************
作者：梁家榕
时间：2019年5月21日11:34:00
功能：设置账号权限
参数1：
参数2：
***********************************************************************/
void MainWindow::SetUserGrade(const int usergrade)
{
    UserGrade = usergrade;

    if(1 == UserGrade)  //管理员权限，都可以使用
    {
        ui->actionLock->setIcon (QIcon("://images/unlock.png"));   //图标改为解锁图标
        ui->actionsetting->setEnabled (true);  //配置工具
        ui->actionLock->setEnabled (true);     //锁定工具
        ui->combB_Bps->setEnabled (true);     //波特率控件
        ui->cBx_Td->setEnabled (true);      //泰斗产品控件
        ui->cBx_Blue->setEnabled (true);    //蓝牙连接控件
        ui->cBx_Again->setEnabled (true);   //重测控件
        for(int i=0; i<4; i++)
        {
            SerialName[i]->setEnabled (true);  //串口号控件
            SerialName[i]->setToolTip ("串口号");       //提示
        }
        ui->tab_2->setEnabled (false);

        ui->actionsetting->setToolTip ("设置");
        ui->combB_Bps->setToolTip ("波特率");
        ui->cBx_Td->setToolTip ("泰斗产品");
        ui->cBx_Blue->setToolTip ("蓝牙连接");
        ui->cBx_Again->setToolTip ("重测");
        ui->actionLock->setToolTip ("已解锁");

        ui->tab_2->setToolTip ("无法更改");
    }
    else if(0 == UserGrade)     //测试人员权限
    {
        ui->actionLock->setIcon (QIcon("://images/lock.png"));   //图标改为锁定状态
        ui->actionsetting->setEnabled (false);
        ui->combB_Bps->setEnabled (false);
        ui->actionLock->setEnabled (false);
        ui->cBx_Td->setEnabled (false);
        ui->cBx_Blue->setEnabled (false);
        ui->cBx_Again->setEnabled (false);
        for(int i=0; i<4; i++)
        {
            SerialName[i]->setEnabled (false);
            SerialName[i]->setToolTip ("无法更改");
        }
        ui->tab_2->setEnabled (false);

        ui->actionsetting->setToolTip ("无法更改");
        ui->combB_Bps->setToolTip ("无法更改");
        ui->cBx_Td->setToolTip ("无法更改");
        ui->cBx_Blue->setToolTip ("无法更改");
        ui->cBx_Again->setToolTip ("无法更改");
        ui->actionLock->setToolTip ("已锁定");

        ui->tab_2->setToolTip ("无法更改");
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日16:40:00
功能：显示订单号
参数1：
参数2：
***********************************************************************/
void MainWindow::SoftWareShow()
{
    QStatusBar *sBar = statusBar ();
    sBar->addWidget (new QLabel("主机名: "+SoftWareName+"     ", this));//刘维球，主机名，2019年5月7日14:14:53
}

/**********************************************************************
作者：梁家榕
时间：2019年5月21日15:28:00
功能：全部开始/或停止
参数1：
参数2：
***********************************************************************/
void MainWindow::on_actionStartAll_triggered()
{
    if(SerialName[0]->count () == 0
            && SerialName[1]->count () == 0
            && SerialName[2]->count () == 0
            && SerialName[3]->count () == 0)    //没有一个可用串口
    {
        QMessageBox::warning(this, tr("错误提示"), tr("没有可用串口！"), tr("确 定"));
        return ;
    }

    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if("全部开始" == ui->actionStartAll->toolTip ())
        {
            if(ui->btn_Start_1->isEnabled ())   //如果开始按钮启用：串口已关闭，可以打开串口
            {
                on_btn_Start_1_clicked();
            }
            if(ui->btn_Start_2->isEnabled ())
            {
                on_btn_Start_2_clicked();
            }
            if(ui->btn_Start_3->isEnabled ())
            {
                on_btn_Start_3_clicked();
            }
            if(ui->btn_Start_4->isEnabled ())
            {
                on_btn_Start_4_clicked();
            }

            ui->actionStartAll->setToolTip ("全部停止");
            ui->actionStartAll->setIcon (QIcon("://images/stop.png"));   //图标改为开始图标
        }
        else if("全部停止" == ui->actionStartAll->toolTip ())
        {
            if(ui->btn_Stop_1->isEnabled ())    //如果停止按钮启用：串口已开启，可以关闭串口
            {
                on_btn_Stop_1_clicked();
            }
            if(ui->btn_Stop_2->isEnabled ())
            {
                on_btn_Stop_2_clicked();
            }
            if(ui->btn_Stop_3->isEnabled ())
            {
                on_btn_Stop_3_clicked();
            }
            if(ui->btn_Stop_4->isEnabled ())
            {
                on_btn_Stop_4_clicked();
            }
            ui->actionStartAll->setToolTip ("全部开始");
            ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为停止图标
        }
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月14日15:42:00
功能：串口功能模块
参数1：
参数2：
***********************************************************************/
void MainWindow::SerialFunc()
{
//梁家榕：初始化控件数组，2019年5月14日15:50:00
    //初始化串口名控件数组
    SerialName[0] = ui->combB_COM_1;    SerialName[1] = ui->combB_COM_2;
    SerialName[2] = ui->combB_COM_3;    SerialName[3] = ui->combB_COM_4;

    //初始化串口波特率显示数组
    SerialBaudShow[0] = ui->label_Bps_1;    SerialBaudShow[1] = ui->label_Bps_2;
    SerialBaudShow[2] = ui->label_Bps_3;    SerialBaudShow[3] = ui->label_Bps_4;

    //初始化串口读取显示控件数组
    SerialDisplay[0] = ui->pTEdit_display_1;    SerialDisplay[1] = ui->pTEdit_display_2;
    SerialDisplay[2] = ui->pTEdit_display_3;    SerialDisplay[3] = ui->pTEdit_display_4;

    //初始化测试结果显示控件数组
    SerialResult[0] = ui->label_Result_1;   SerialResult[1] = ui->label_Result_2;
    SerialResult[2] = ui->label_Result_3;   SerialResult[3] = ui->label_Result_4;

    //加载动态库
    if(true == mySerial.RDADllInit())
    {
        qDebug()<<"加载动态库成功！";
        //m_speech->speak("加载动态库成功");
    }
    else {
        qDebug()<<"加载动态库失败！";
    }

    //初始化RDA串口 莫秋兰
    for(int i = 0; i < 4; i++)
    {
        mySerial.ComInit[i] = false;
    }


//梁家榕：初始化默认波特率、串口号，2019年5月8日10:11:00
    for(int i=0; i<4; i++)
    {
        int Num = mySerial.SerachSerialPortName (SerialName[i]);  //寻找可用串口 2019/5/14 16:57:00
        if(i < Num)
        {
            SerialName[i]->setCurrentIndex (i);     //显示默认串口
        }
        else
        {
            SerialName[i]->clear ();    //串口不够，不分配
        }
    }

    ui->combB_Bps->setCurrentIndex (8); //显示默认波特率
    for(int i=0; i<4; i++)
    {
        SerialBaudShow[i]->setText ("波特率   " + ui->combB_Bps->currentText ());     //显示默认波特率
        SerialResult[i]->setText ("等待");
        SerialResult[i]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体
    }

    INISETTINGS  iniSettings; //刘维球，初始化设置，2019年4月28日11:07:50
    QString appDir = QApplication::applicationDirPath(); ////刘维球：当前程序路径，2019年4月28日09:21:02
    const QString  configFile = appDir + "/config.ini" ; ////刘维球，config.ini路径
    QFile file(configFile);
    if( file.exists())
    {
        db_ResultList = iniSettings.getResultTable (configFile);  //获取结果表链接信息

        //莫秋兰：RDA串口通信关联
        connect(&RDA_WTimer1,&QTimer::timeout,this,&MainWindow::RDA_WTimerdeal1);
        connect(&RDA_WTimer2,&QTimer::timeout,this,&MainWindow::RDA_WTimerdeal2);
        connect(&RDA_WTimer3,&QTimer::timeout,this,&MainWindow::RDA_WTimerdeal3);
        connect(&RDA_WTimer4,&QTimer::timeout,this,&MainWindow::RDA_WTimerdeal4);

        connect(&RDA_RTimer1,&QTimer::timeout,this,&MainWindow::RDA_RTimerdeal1);
        connect(&RDA_RTimer2,&QTimer::timeout,this,&MainWindow::RDA_RTimerdeal2);
        connect(&RDA_RTimer3,&QTimer::timeout,this,&MainWindow::RDA_RTimerdeal3);
        connect(&RDA_RTimer4,&QTimer::timeout,this,&MainWindow::RDA_RTimerdeal4);

        //梁家榕：关联串口发送、串口读取，2019年5月8日10:13:00
        connect (&myTimer1, &QTimer::timeout, this, &MainWindow::dealMyTimer1);     //处理串口1的定时器发送数据
        connect (&myTimer2, &QTimer::timeout, this, &MainWindow::dealMyTimer2);     //处理串口2的定时器发送数据
        connect (&myTimer3, &QTimer::timeout, this, &MainWindow::dealMyTimer3);     //处理串口3的定时器发送数据
        connect (&myTimer4, &QTimer::timeout, this, &MainWindow::dealMyTimer4);     //处理串口4的定时器发送数据

        connect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);    //处理串口1数据读取
        connect (&mySerial2, &QSerialPort::readyRead, this, &MainWindow::dealRead2);    //处理串口2数据读取
        connect (&mySerial3, &QSerialPort::readyRead, this, &MainWindow::dealRead3);    //处理串口3数据读取
        connect (&mySerial4, &QSerialPort::readyRead, this, &MainWindow::dealRead4);    //处理串口4数据读取

        connect (&myTimerOut, &QTimer::timeout, this, &MainWindow::dealMyTimeOut);  //处理超时
        myTimerOut.start (2000);
    }
    else
    {
        QMessageBox::warning(this, "错误提示", "查看当前路径下的工程文件是否存在！");
    }

    ui->tabWidget->setCurrentIndex (0);     //选择当前页面为检测页面
    //设置搜索订单按钮特效
    ui->pushButton->setStyleSheet ("QPushButton{border-image: url(:/images/search.PNG);}"
                                   "QPushButton:hover{border-image: url(:/images/searching.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/searching.png);}");
    QStringList tableHeader;
    tableHeader << "指令类型" << "设置项目" << "返回值" << "可变参数" << "指令";
    ui->tableWidget->setHorizontalHeaderLabels (tableHeader);
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日10:10:00
功能：打开串口槽函数
参数1：
参数2：
***********************************************************************/
void MainWindow::on_btn_Start_1_clicked()       //打开串口1
{
    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
        return ;
    }
    if(ui->btnok->isEnabled ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请确定配置！！！"), tr("确 定"));
        return ;
    }

    if(0 != SerialName[0]->count ())   //串口号充足
    {
        //进行通信方式标志位判断
        if(0 == setSerial_RDA_Flag())   //普通串口通信    莫秋兰
        {
            if(false == mySerial.OpenSerial(&mySerial1, SerialName[0], ui->combB_Bps))   //打开串口
            {
                SerialDisplay[0]->appendPlainText ("串口打开失败");
                return ;
            }
            Serial1_ConfigNum = -1;    //测试指令从头开始
            SerialResult[0]->setText ("等待");
            SerialResult[0]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            ui->btn_Start_1->setEnabled (false);    //关闭开始按钮设置使能
            ui->btn_Stop_1->setEnabled (true);      //打开关闭按钮设置使能

            myTimer1.start (myTimes);          //开启定时器
        }
        else    //RDA串口通信   莫秋兰
        {
            qDebug()<<"RDA串口通信";
            if(false == mySerial.RDAComInit(0,SerialName[0]->currentText()))
            {
                SerialDisplay[0]->appendPlainText("串口打开失败");
                return;
            }
            else {
                Serial1_ConfigNum = -1;    //测试指令从头开始
                SerialResult[0]->setText ("等待");
                SerialResult[0]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

                ui->btn_Start_1->setEnabled (false);    //关闭开始按钮设置使能
                ui->btn_Stop_1->setEnabled (true);      //打开关闭按钮设置使能
                //打开RDA写的定时器
                RDA_WTimer1.start(500);


            }

        }
        //梁家榕：连接90数据库，2019年5月14日9:54:00
        if(updb_1.ConnectDataBase (db_ResultList))
            qDebug() << "GPSTest111111111111111数据库连接成功";
        else
            QMessageBox::warning(this, tr("错误提示"), tr("数据库连接失败"), "确 定");


        if(ui->btn_Stop_1->isEnabled () && ui->btn_Stop_2->isEnabled ()   //串口打开，关闭按钮启用
                && ui->btn_Stop_3->isEnabled () && ui->btn_Stop_4->isEnabled ())  //全部串口打开，全部关闭按钮启用
        {
            ui->actionStartAll->setToolTip ("全部关闭");
            ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
        }

        SerialDisplay[0]->clear ();     //清空数据接收编辑区
    }
}

void MainWindow::on_btn_Start_2_clicked()       //打开串口2
{
    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
        return ;
    }
    if(ui->btnok->isEnabled ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请确定配置！！！"), tr("确 定"));
        return ;
    }
    if(0 != SerialName[1]->count ())   //串口号充足
    {
        if(0 == setSerial_RDA_Flag())   // 莫秋兰 普通串口
        {
            if(false == mySerial.OpenSerial(&mySerial2, SerialName[1], ui->combB_Bps))   //打开串口
            {
                SerialDisplay[1]->appendPlainText ("串口打开失败");
                return ;
            }
            Serial2_ConfigNum = -1;    //指令从头开始
            SerialResult[1]->setText ("等待");
            SerialResult[1]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            ui->btn_Start_2->setEnabled (false);    //关闭开始按钮设置使能
            ui->btn_Stop_2->setEnabled (true);      //打开关闭按钮设置使能

            myTimer2.start (myTimes);          //开启定时器
        }
        else        //莫秋兰
        {
            qDebug()<<"RDA串口通信";
            if(false == mySerial.RDAComInit(1,SerialName[1]->currentText()))
            {
                SerialDisplay[1]->appendPlainText("串口打开失败");
                return;
            }
            else {
                Serial2_ConfigNum = -1;    //测试指令从头开始
                SerialResult[1]->setText ("等待");
                SerialResult[1]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

                ui->btn_Start_2->setEnabled (false);    //关闭开始按钮设置使能
                ui->btn_Stop_2->setEnabled (true);      //打开关闭按钮设置使能
                //打开RDA写的定时器
                RDA_WTimer2.start(500);


            }
        }

        //梁家榕：连接90数据库，2019年5月14日9:54:00
        if(updb_2.ConnectDataBase (db_ResultList))
            qDebug() << "GPSTest数据库连接成功";
        else
            QMessageBox::warning(this, tr("错误提示"), tr("数据库连接失败"), "确 定");


        if(ui->btn_Stop_1->isEnabled () && ui->btn_Stop_2->isEnabled ()   //串口打开，关闭按钮启用
                && ui->btn_Stop_3->isEnabled () && ui->btn_Stop_4->isEnabled ())  //全部串口打开，全部关闭按钮启用
        {
            ui->actionStartAll->setToolTip ("全部关闭");
            ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
        }

        SerialDisplay[1]->clear ();     //清空数据接收编辑区

    }
}

void MainWindow::on_btn_Start_3_clicked()       //打开串口3
{
    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
        return ;
    }
    else if(ui->btnok->isEnabled ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请确定配置！！！"), tr("确 定"));
        return ;
    }
    if(0 != SerialName[2]->count ())   //串口号充足
    {
        if(0 == setSerial_RDA_Flag())   //莫秋兰
        {
            if(false == mySerial.OpenSerial(&mySerial3, SerialName[2], ui->combB_Bps))   //打开串口
            {
                SerialDisplay[2]->appendPlainText ("串口打开失败");
                return ;
            }
            Serial3_ConfigNum = -1;    //指令从头开始

            SerialResult[2]->setText ("等待");
            SerialResult[2]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            ui->btn_Start_3->setEnabled (false);    //关闭开始按钮设置使能
            ui->btn_Stop_3->setEnabled (true);      //打开关闭按钮设置使能

            myTimer3.start (myTimes);          //开启定时器
        }
        else {      //莫秋兰
            qDebug()<<"RDA串口通信";
            if(false == mySerial.RDAComInit(2,SerialName[2]->currentText()))
            {
                SerialDisplay[2]->appendPlainText("串口打开失败");
                return;
            }
            else {

                Serial3_ConfigNum = -1;    //测试指令从头开始
                SerialResult[2]->setText ("等待");
                SerialResult[2]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

                ui->btn_Start_3->setEnabled (false);    //关闭开始按钮设置使能
                ui->btn_Stop_3->setEnabled (true);      //打开关闭按钮设置使能
                //打开RDA写的定时器
                RDA_WTimer3.start(500);


            }
        }


        //梁家榕：连接90数据库，2019年5月14日9:54:00
        if(updb_3.ConnectDataBase (db_ResultList))
            qDebug() << "GPSTest数据库连接成功";
        else
            QMessageBox::warning(this, tr("错误提示"), tr("数据库连接失败"), "确 定");


        if(ui->btn_Stop_1->isEnabled () && ui->btn_Stop_2->isEnabled ()   //串口打开，关闭按钮启用
                && ui->btn_Stop_3->isEnabled () && ui->btn_Stop_4->isEnabled ())  //全部串口打开，全部关闭按钮启用
        {
            ui->actionStartAll->setToolTip ("全部关闭");
            ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
        }

        SerialDisplay[2]->clear ();     //清空数据接收编辑区
    }
}

void MainWindow::on_btn_Start_4_clicked()       //打开串口4
{
    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
        return ;
    }
    if(ui->btnok->isEnabled ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请确定配置！！！"), tr("确 定"));
        return ;
    }
    if(0 != SerialName[3]->count ())   //串口号充足
    {
        if(0 == setSerial_RDA_Flag())   //莫秋兰
        {
            if(false == mySerial.OpenSerial(&mySerial4, SerialName[3], ui->combB_Bps))   //打开串口
            {
                SerialDisplay[3]->appendPlainText ("串口打开失败");
                return ;
            }
            Serial4_ConfigNum = -1;    //指令从头开始

            SerialResult[3]->setText ("等待");
            SerialResult[3]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            ui->btn_Start_4->setEnabled (false);    //关闭开始按钮设置使能
            ui->btn_Stop_4->setEnabled (true);      //打开关闭按钮设置使能

            myTimer4.start (myTimes);          //开启定时器
        }
        else {      //莫秋兰

            qDebug()<<"RDA串口通信";
            if(false == mySerial.RDAComInit(3,SerialName[3]->currentText()))
            {
                SerialDisplay[3]->appendPlainText("串口打开失败");
                return;
            }
            else {

                Serial4_ConfigNum = -1;    //测试指令从头开始
                SerialResult[3]->setText ("等待");
                SerialResult[3]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

                ui->btn_Start_4->setEnabled (false);    //关闭开始按钮设置使能
                ui->btn_Stop_4->setEnabled (true);      //打开关闭按钮设置使能
                //打开RDA写的定时器
                RDA_WTimer4.start(500);


            }
        }


        //梁家榕：连接90数据库，2019年5月14日9:54:00
        if(updb_4.ConnectDataBase (db_ResultList))
            qDebug() << "GPSTest数据库连接成功";
        else
            QMessageBox::warning(this, tr("错误提示"), tr("数据库连接失败"), "确 定");


        if(ui->btn_Stop_1->isEnabled () && ui->btn_Stop_2->isEnabled ()   //串口打开，关闭按钮启用
                && ui->btn_Stop_3->isEnabled () && ui->btn_Stop_4->isEnabled ())  //全部串口打开，全部关闭按钮启用
        {
            ui->actionStartAll->setToolTip ("全部关闭");
            ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
        }

        SerialDisplay[3]->clear ();     //清空数据接收编辑区
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日10:10:00
功能：关闭串口槽函数
参数1：
参数2：
***********************************************************************/
void MainWindow::on_btn_Stop_1_clicked()        //关闭串口1
{
    //莫秋兰 判断串口通信方式,关闭串口 普通串口方式
    if(0 == setSerial_RDA_Flag())
    {
        mySerial.CloseSerial (&mySerial1);   //关闭串口

        ui->btn_Start_1->setEnabled (true);     //打开开始按钮设置使能
        ui->btn_Stop_1->setEnabled (false);     //关闭关闭按钮设置使能

        myTimer1.stop ();   //关闭定时器1
    }
    else    //RDA方式
    {
        if(mySerial.CloseRDACom(0))
        {
            qDebug()<<"串口关闭！";
            //关闭定时器
            RDA_RTimer1.stop();
            RDA_WTimer1.stop();
            ui->btn_Start_1->setEnabled (true);     //打开开始按钮设置使能
            ui->btn_Stop_1->setEnabled (false);     //关闭关闭按钮设置使能
            mySerial.ComInit[0] = false;
            Serial1_ConfigNum = 0;
            Config_StartNum = 0;

        }
    }


    if("等待" != SerialResult[0]->text () && "成功" != SerialResult[0]->text ())    //中途中断:测试失败
    {
        test_Num ++; fail_Num++;
        updateStatus ();    //刷新状态栏
    }

    SerialResult[0]->setText ("等待");
    SerialResult[0]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

    if(ui->btn_Start_1->isEnabled () && ui->btn_Start_2->isEnabled ()   //串口关闭，开始按钮启用
            && ui->btn_Start_3->isEnabled () && ui->btn_Start_4->isEnabled ())  //全部串口关闭，全部开始按钮启用
    {
        ui->actionStartAll->setToolTip ("全部开始");
        ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
    }
}

void MainWindow::on_btn_Stop_2_clicked()        //关闭串口2
{
    if(0 == setSerial_RDA_Flag())   //莫秋兰 普通串口
    {
        mySerial.CloseSerial (&mySerial2);   //关闭串口

        ui->btn_Start_2->setEnabled (true);     //打开开始按钮设置使能
        ui->btn_Stop_2->setEnabled (false);     //关闭关闭按钮设置使能

        myTimer2.stop ();   //关闭定时器2
    }
    else {  //      RDA

        if(mySerial.CloseRDACom(1))
        {
            qDebug()<<"串口关闭！";
            //关闭定时器
            RDA_RTimer2.stop();
            RDA_WTimer2.stop();
            ui->btn_Start_2->setEnabled (true);     //打开开始按钮设置使能
            ui->btn_Stop_2->setEnabled (false);     //关闭关闭按钮设置使能
            mySerial.ComInit[1] = false;
            Serial2_ConfigNum = 0;
            Config_StartNum = 0;

        }
    }


    if("等待" != SerialResult[1]->text () && "成功" != SerialResult[1]->text ())    //中途中断:测试失败
    {
        test_Num ++; fail_Num++;
        updateStatus ();    //刷新状态栏
    }

    SerialResult[1]->setText ("等待");
    SerialResult[1]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

    if(ui->btn_Start_1->isEnabled () && ui->btn_Start_2->isEnabled ()   //串口关闭，开始按钮启用
            && ui->btn_Start_3->isEnabled () && ui->btn_Start_4->isEnabled ())  //全部串口关闭，全部开始按钮启用
    {
        ui->actionStartAll->setToolTip ("全部开始");
        ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
    }
}

void MainWindow::on_btn_Stop_3_clicked()        //关闭串口3
{
    //莫秋兰
    if(0 == setSerial_RDA_Flag())
    {
        mySerial.CloseSerial (&mySerial3);   //关闭串口

        ui->btn_Start_3->setEnabled (true);     //打开开始按钮设置使能
        ui->btn_Stop_3->setEnabled (false);     //关闭关闭按钮设置使能

        myTimer3.stop ();   //关闭定时器3
    }
    else {

        if(mySerial.CloseRDACom(2))
        {
            qDebug()<<"串口关闭！";
            //关闭定时器
            RDA_RTimer3.stop();
            RDA_WTimer3.stop();
            ui->btn_Start_3->setEnabled (true);     //打开开始按钮设置使能
            ui->btn_Stop_3->setEnabled (false);     //关闭关闭按钮设置使能
            mySerial.ComInit[2] = false;
            Serial3_ConfigNum = 0;
            Config_StartNum = 0;

        }
    }


    if("等待" != SerialResult[2]->text () && "成功" != SerialResult[2]->text ())    //中途中断:测试失败
    {
        test_Num ++; fail_Num++;
        updateStatus ();    //刷新状态栏
    }

    SerialResult[2]->setText ("等待");
    SerialResult[2]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

    if(ui->btn_Start_1->isEnabled () && ui->btn_Start_2->isEnabled ()   //串口关闭，开始按钮启用
            && ui->btn_Start_3->isEnabled () && ui->btn_Start_4->isEnabled ())  //全部串口关闭，全部开始按钮启用
    {
        ui->actionStartAll->setToolTip ("全部开始");
        ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
    }
}

void MainWindow::on_btn_Stop_4_clicked()        //关闭串口4
{
    //莫秋兰
    if(0 == setSerial_RDA_Flag())
    {
        mySerial.CloseSerial (&mySerial4);   //关闭串口

        ui->btn_Start_4->setEnabled (true);     //打开开始按钮设置使能
        ui->btn_Stop_4->setEnabled (false);     //关闭关闭按钮设置使能

        myTimer4.stop ();   //关闭定时器4
    }
    else {
        if(mySerial.CloseRDACom(3))
        {
            qDebug()<<"串口关闭！";
            //关闭定时器
            RDA_RTimer4.stop();
            RDA_WTimer4.stop();
            ui->btn_Start_4->setEnabled (true);     //打开开始按钮设置使能
            ui->btn_Stop_4->setEnabled (false);     //关闭关闭按钮设置使能
            mySerial.ComInit[3] = false;
            Serial4_ConfigNum = 0;
            Config_StartNum = 0;

        }
    }


    if("等待" != SerialResult[3]->text () && "成功" != SerialResult[3]->text ())    //中途中断:测试失败
    {
        test_Num ++; fail_Num++;
        updateStatus ();    //刷新状态栏
    }

    SerialResult[3]->setText ("等待");
    SerialResult[3]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

    if(ui->btn_Start_1->isEnabled () && ui->btn_Start_2->isEnabled ()   //串口关闭，开始按钮启用
            && ui->btn_Start_3->isEnabled () && ui->btn_Start_4->isEnabled ())  //全部串口关闭，全部开始按钮启用
    {
        ui->actionStartAll->setToolTip ("全部开始");
        ui->actionStartAll->setIcon (QIcon("://images/start.png"));   //图标改为开始图标
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日10:11:00
功能：处理串口数据读取
参数1：
参数2：
***********************************************************************/
void MainWindow::dealRead1()        //串口1读取
{

    QString thisStr;        //接收到的数据
    QString needStrMin;     //需要接收到的数据最小值
    QString needStrMax;     //需要接收到的数据最大值
    //QString ReadDateTime;   //日期时间
    static int thisNum = 1; //已重复次数
    int needNum = 1;        //失败重复读取次数

    //莫秋兰
//    int test_flag;  //测试成功标志位
//    int upto_flag;  //上传数据库成功标志位

#if 0
        qDebug() << "444444444444444";
#endif

    if(Serial1_ConfigNum == -1)
    {
        mySerial.ReadData(&mySerial1, Readbuf1);     //读取数据
        if(!Readbuf1.isEmpty ())
        {
            thisStr = tr(Readbuf1);     //本次返回值
            if(-1 != thisStr.indexOf ("TEST_OK"))
            {
                Serial1_ConfigNum = Config_StartNum;
                Readbuf1.clear ();
                SerialDisplay[0]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (thisStr.mid (0, thisStr.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[0]->clear (); //清空编辑区
                //disconnect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取
                return ;
            }
        }
    }

    for(int i=0; i< ConfigReturn.size (); i++)
    {
        if("重试次数" == ConfigReturn.at (i))
        {
            needNum = ConfigPara.at (i).toInt ();
            break;
        }
    }

    if(Config_StartNum == Serial1_ConfigNum)
    {
        SerialResult[0]->setText ("测试");
        SerialResult[0]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    //梁家榕：获取系统时间，2019年5月18日15:12:00
    //LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    //ReadDateTime = LocalDateTime.toString ("yyyy-MM-dd  hh:mm:ss");     //格式化日期时间

    //梁家榕：读取返回值并判断，2019年5月8日14:15:00
    mySerial.ReadData(&mySerial1, Readbuf1);     //读取数据
    if(!Readbuf1.isEmpty ())
    {
        thisStr = tr(Readbuf1);     //本次返回值
#if 0
        qDebug() << thisStr;
#endif
        if(1 == Return_Contrast(thisStr, Serial1_ConfigNum, Serial1_Result))        //成功
        {
            SerialDisplay[0]->appendPlainText(QString("    返回:%1  正常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2))); //返回值  去掉\r\n
            //Serial1_Result.append (thisStr);    //加入结果列表

            //没有待测项
            if(Serial1_ConfigNum == ConfigType.size ()-1)
            {
                Serial1_ConfigNum = -1;      //重头发测试指令       检测新机子功能未做   判断机子是否已测功能未做
                SerialResult[0]->setText ("成功");
                SerialResult[0]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                SerialDisplay[0]->appendPlainText("********  产品测试成功  ********\r\n");

                //莫秋兰
                //test_flag = 1;

                //语音播报成功
                m_speech_1->speak("通道一成功");

                test_Num ++; pass_Num++;
                updateStatus ();    //刷新状态栏

                if(uploadDataBase(Serial1_Result, updb_1))     //拼接字符串，上传数据库
                {
                    SerialDisplay[0]->appendPlainText("********  数据上传成功  ********\r\n");

                    //莫秋兰
                    //upto_flag = 1;
                }
                else
                {
                    fmt.setForeground (Qt::red);    //设置前景色(字体色)
                    SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    SerialDisplay[0]->appendPlainText("********  数据上传失败  ********\r\n");

                    //莫秋兰
                    //upto_flag = 0;

                    fmt.setForeground (Qt::black);    //设置前景色(字体色)
                    SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式
                }
                Serial1_Result.clear ();    //清空结果
                //SerialDisplay[0]->clear (); //清空编辑区
            }
            else
            {
                Serial1_ConfigNum++;
            }
            thisNum = 1;
        }
        else        //失败
        {
            //梁家榕：设置字体颜色并显示，2019年5月18日16:51:00
            fmt.setForeground (Qt::red);    //设置前景色(字体色)
            SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式
            SerialDisplay[0]->appendPlainText(QString("    返回:%1  异常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2)));          //返回值
            fmt.setForeground (Qt::black);    //设置前景色(字体色)
            SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式

            //重复次数已够
            if(thisNum == needNum)
            {
                thisNum = 1;
                Serial1_ConfigNum = -1;
                SerialResult[0]->setText ("失败");
                SerialResult[0]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[0]->appendPlainText("********  产品测试失败  ********\r\n");

                //莫秋兰
                //test_flag = 0;

                //莫秋兰 语音播报
                m_speech_1->speak("通道一失败");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial1_Result.clear ();    //清空结果
                //SerialDisplay[0]->clear (); //清空编辑区
            }
            else
            {
                thisNum++;
            }
        }

//        //莫秋兰  语音播报结果
//        if(test_flag == 1 && upto_flag == 1)
//        {
//            //全部成功
//            m_speech_1->speak("成功");
//        }
//        else if(test_flag == 1 && upto_flag == 0)
//        {
//            //测试成功，上传失败
//            m_speech_1->speak("测试成功，上传失败");
//        }



    }
#if 0
        qDebug() << "111111" << Readbuf1;
#endif

    //disconnect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取
    Readbuf1.clear ();
    myTimer1.start (myTimes);             //打开定时器
}

void MainWindow::dealRead2()        //串口2读取
{
    QString thisStr;        //接收到的数据
    QString needStrMin;     //需要接收到的数据最小值
    QString needStrMax;     //需要接收到的数据最大值
    //QString ReadDateTime;   //日期时间
    static int thisNum = 1; //已重复次数
    int needNum = 1;        //失败重复读取次数

    if(Serial2_ConfigNum == -1)
    {
        mySerial.ReadData(&mySerial2, Readbuf2);     //读取数据
        if(!Readbuf2.isEmpty ())
        {
            thisStr = tr(Readbuf2);     //本次返回值
            if(-1 != thisStr.indexOf ("TEST_OK"))
            {
                Serial2_ConfigNum = Config_StartNum;
                Readbuf2.clear ();
                SerialDisplay[1]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (thisStr.mid (0, thisStr.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[1]->clear (); //清空编辑区
                return ;
            }
        }
    }

    for(int i=0; i< ConfigReturn.size (); i++)
    {
        if("重试次数" == ConfigReturn.at (i))
        {
            needNum = ConfigPara.at (i).toInt ();
            break;
        }
    }

    if(Config_StartNum == Serial2_ConfigNum)
    {
        SerialResult[1]->setText ("测试");
        SerialResult[1]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    //梁家榕：获取系统时间，2019年5月18日15:12:00
    //LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    //ReadDateTime = LocalDateTime.toString ("yyyy-MM-dd  hh:mm:ss");     //格式化日期时间

    //梁家榕：读取返回值并判断，2019年5月8日14:15:00
    mySerial.ReadData(&mySerial2, Readbuf2);     //读取数据
    if(false == Readbuf2.isEmpty ())
    {
        thisStr = tr(Readbuf2);     //本次返回值

        if(1 == Return_Contrast(thisStr, Serial2_ConfigNum, Serial2_Result))
        {
            SerialDisplay[1]->appendPlainText(QString("    返回:%1  正常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2)));          //返回值
            //Serial2_Result.append (thisStr);    //加入结果列表

            //没有待测项
            if(Serial2_ConfigNum == ConfigType.size ()-1)
            {
                Serial2_ConfigNum = -1;      //重头发测试指令       检测新机子功能未做   判断机子是否已测功能未做
                SerialResult[1]->setText ("成功");
                SerialResult[1]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                SerialDisplay[1]->appendPlainText("********  产品测试成功  ********\r\n");

                //莫秋兰 语音播报
                m_speech_2->speak("通道二成功");

                test_Num ++; pass_Num++;
                updateStatus ();    //刷新状态栏

                if(uploadDataBase(Serial2_Result, updb_2))     //拼接字符串，上传数据库
                {
                    SerialDisplay[1]->appendPlainText("********  数据上传成功  ********\r\n");
                }
                else
                {
                    fmt.setForeground (Qt::red);    //设置前景色(字体色)
                    SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    SerialDisplay[1]->appendPlainText("********  数据上传失败  ********\r\n");
                    fmt.setForeground (Qt::black);    //设置前景色(字体色)
                    SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式
                }
                Serial2_Result.clear ();    //清空结果
                //SerialDisplay[1]->clear (); //清空编辑区
            }
            else    //还有待测项
            {
                Serial2_ConfigNum++;
            }
            thisNum = 1;
        }
        else
        {
            //梁家榕：设置字体颜色并显示，2019年5月18日16:51:00
            fmt.setForeground (Qt::red);    //设置前景色(字体色)
            SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式
            SerialDisplay[1]->appendPlainText(QString("    返回:%1  异常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2)));          //返回值
            fmt.setForeground (Qt::black);    //设置前景色(字体色)
            SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式

            //重复次数已够
            if(thisNum == needNum)
            {
                thisNum = 1;
                Serial2_ConfigNum = -1;
                SerialResult[1]->setText ("失败");
                SerialResult[1]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[1]->appendPlainText("********  产品测试失败  ********\r\n");

                //莫秋兰 语音播报
                m_speech_2->speak("通道二失败");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial2_Result.clear ();    //清空结果
                //SerialDisplay[1]->clear (); //清空编辑区
            }
            else
            {
                thisNum++;
            }
        }
    }

    Readbuf2.clear ();
    myTimer2.start (myTimes);           //打开定时器
}

void MainWindow::dealRead3()        //串口3读取
{
    QString thisStr;        //接收到的数据
    QString needStrMin;     //需要接收到的数据最小值
    QString needStrMax;     //需要接收到的数据最大值
    //QString ReadDateTime;   //日期时间
    static int thisNum = 1; //已重复次数
    int needNum = 1;        //失败重复读取次数

    if(Serial3_ConfigNum == -1)
    {
        mySerial.ReadData(&mySerial3, Readbuf3);     //读取数据
        if(!Readbuf3.isEmpty ())
        {
            thisStr = tr(Readbuf3);     //本次返回值
            if(-1 != thisStr.indexOf ("TEST_OK"))
            {
                Serial3_ConfigNum = Config_StartNum;
                Readbuf3.clear ();
                SerialDisplay[2]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (thisStr.mid (0, thisStr.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[2]->clear (); //清空编辑区
                return ;
            }
        }
    }

    for(int i=0; i< ConfigReturn.size (); i++)
    {
        if("重试次数" == ConfigReturn.at (i))
        {
            needNum = ConfigPara.at (i).toInt ();
            break;
        }
    }

    if(Config_StartNum == Serial3_ConfigNum)
    {
        SerialResult[2]->setText ("测试");
        SerialResult[2]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    mySerial.ReadData(&mySerial3, Readbuf3);     //读取数据

    if(false == Readbuf3.isEmpty ())
    {
        thisStr = tr(Readbuf3);     //本次返回值

        if(1 == Return_Contrast(thisStr, Serial3_ConfigNum, Serial3_Result))
        {
            SerialDisplay[2]->appendPlainText(QString("    返回:%1  正常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2)));          //返回值
            //Serial3_Result.append (thisStr);    //加入结果列表

            //没有待测项
            if(Serial3_ConfigNum == ConfigType.size ()-1)
            {
                Serial3_ConfigNum = -1;      //重头发测试指令     检测新机子功能未做   判断机子是否已测功能未做
                SerialResult[2]->setText ("成功");
                SerialResult[2]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                SerialDisplay[2]->appendPlainText("********  产品测试成功  ********\r\n");

                //莫秋兰 语音播报
                m_speech_3->speak("通道三成功");

                test_Num ++; pass_Num++;
                updateStatus ();    //刷新状态栏

                if(uploadDataBase(Serial3_Result, updb_3))     //拼接字符串，上传数据库
                {
                    SerialDisplay[2]->appendPlainText("********  数据上传成功  ********\r\n");
                }
                else
                {
                    fmt.setForeground (Qt::red);    //设置前景色(字体色)
                    SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    SerialDisplay[2]->appendPlainText("********  数据上传失败  ********\r\n");
                    fmt.setForeground (Qt::black);    //设置前景色(字体色)
                    SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式
                }
                Serial3_Result.clear ();    //清空结果
                //SerialDisplay[2]->clear (); //清空编辑区
            }
            else    //还有待测项
            {
                Serial3_ConfigNum++;
            }
            thisNum = 1;
        }
        else
        {
            //梁家榕：设置字体颜色并显示，2019年5月18日16:51:00
            fmt.setForeground (Qt::red);    //设置前景色(字体色)
            SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式
            SerialDisplay[2]->appendPlainText(QString("    返回:%1  异常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2)));          //返回值
            fmt.setForeground (Qt::black);    //设置前景色(字体色)
            SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式

            //重复次数已够
            if(thisNum == needNum)
            {
                thisNum = 1;
                Serial3_ConfigNum = -1;
                SerialResult[2]->setText ("失败");
                SerialResult[2]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[2]->appendPlainText("********  产品测试失败  ********\r\n");

                //莫秋兰 语音播报
                m_speech_3->speak("通道三失败");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial3_Result.clear ();    //清空结果
                //SerialDisplay[2]->clear (); //清空编辑区
            }
            else
            {
                thisNum++;
            }
        }
    }

    Readbuf3.clear ();
    myTimer3.start (myTimes);           //打开定时器
}

void MainWindow::dealRead4()        //串口4读取
{
    QString thisStr;        //接收到的数据
    QString needStrMin;     //需要接收到的数据最小值
    QString needStrMax;     //需要接收到的数据最大值
    //QString ReadDateTime;   //日期时间
    static int thisNum = 1;     //已重复次数
    int needNum = 1;        //失败重复读取次数

    if(Serial4_ConfigNum == -1)
    {
        mySerial.ReadData(&mySerial4, Readbuf4);     //读取数据
        if(!Readbuf4.isEmpty ())
        {
            thisStr = tr(Readbuf4);     //本次返回值
            if(-1 != thisStr.indexOf ("TEST_OK"))
            {
                Serial4_ConfigNum = Config_StartNum;
                Readbuf4.clear ();
                SerialDisplay[3]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (thisStr.mid (0, thisStr.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[3]->clear (); //清空编辑区
                return ;
            }
        }
    }

    for(int i=0; i< ConfigReturn.size (); i++)
    {
        if("重试次数" == ConfigReturn.at (i))
        {
            needNum = ConfigPara.at (i).toInt ();
            break;
        }
    }

    if(Config_StartNum == Serial4_ConfigNum)
    {
        SerialResult[3]->setText ("测试");
        SerialResult[3]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    //梁家榕：获取系统时间，2019年5月18日15:12:00
    //LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    //ReadDateTime = LocalDateTime.toString ("yyyy-MM-dd  hh:mm:ss");     //格式化日期时间

    //梁家榕：读取返回值并判断，2019年5月8日14:15:00
    mySerial.ReadData(&mySerial4, Readbuf4);     //读取数据
    if(false == Readbuf4.isEmpty ())
    {
        thisStr = tr(Readbuf4);     //本次返回值

        if(1 == Return_Contrast(thisStr, Serial4_ConfigNum, Serial4_Result))
        {
            SerialDisplay[3]->appendPlainText(QString("    返回:%1  正常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2)));          //返回值
            //Serial4_Result.append (thisStr);    //加入结果列表

            //没有待测项
            if(Serial4_ConfigNum == ConfigType.size ()-1)    //没有待测项
            {
                Serial4_ConfigNum = -1;      //重头发测试指令       检测新机子功能未做   判断机子是否已测功能未做
                SerialResult[3]->setText ("成功");
                SerialResult[3]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                SerialDisplay[3]->appendPlainText("********  产品测试成功  ********\r\n");

                //莫秋兰 语音播报
                m_speech_4->speak("通道四成功");

                test_Num ++; pass_Num++;
                updateStatus ();    //刷新状态栏

                if(uploadDataBase(Serial4_Result, updb_4))     //拼接字符串，上传数据库
                {
                    SerialDisplay[3]->appendPlainText("********  数据上传成功  ********\r\n");
                }
                else
                {
                    fmt.setForeground (Qt::red);    //设置前景色(字体色)
                    SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    SerialDisplay[3]->appendPlainText("********  数据上传失败  ********\r\n");
                    fmt.setForeground (Qt::black);    //设置前景色(字体色)
                    SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式
                }
                Serial4_Result.clear ();    //清空结果
                //SerialDisplay[3]->clear (); //清空编辑区
            }
            else    //还有待测项
            {
                Serial4_ConfigNum++;
            }
            thisNum = 1;
        }
        else
        {
            //梁家榕：设置字体颜色并显示，2019年5月18日16:51:00
            fmt.setForeground (Qt::red);    //设置前景色(字体色)
            SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式
            SerialDisplay[3]->appendPlainText(QString("    返回:%1  异常")
                                              .arg (thisStr.mid (0, thisStr.length ()-2)));            //返回值
            fmt.setForeground (Qt::black);    //设置前景色(字体色)
            SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式

            //重复次数已够
            if(thisNum == needNum)
            {
                thisNum = 1;
                Serial4_ConfigNum = -1;
                SerialResult[3]->setText ("失败");
                SerialResult[3]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[3]->appendPlainText("********  产品测试失败  ********\r\n");

                //莫秋兰 语音播报
                m_speech_4->speak("通道四失败");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial4_Result.clear ();    //清空结果
                //SerialDisplay[3]->clear (); //清空编辑区
            }
            else
            {
                thisNum++;
            }
        }
    }

    Readbuf4.clear ();
    myTimer4.start (myTimes);           //打开定时器
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日16:04:00
功能：处理定时器数据发送
参数1：
参数2：
***********************************************************************/
void MainWindow::dealMyTimer1()       //定时器1发送
{
    //梁家榕：获取系统时间，2019年5月18日15:12:00
    LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    WriteDateTime = LocalDateTime.toString ("yyyy-MM-dd hh:mm:ss");     //格式化日期时间

    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial1_ConfigNum == -1)
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();
            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial1, writeStr))
                {
                    break;
                }
            }
            SerialResult[0]->setText ("等待");
            SerialResult[0]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体
            //connect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取
            SerialDisplay[0]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
            if(SerialDisplay[0]->blockCount () > 100)
            {
                SerialDisplay[0]->clear ();
            }
        }
        else
        {
            if(ConfigSend.at (Serial1_ConfigNum) == "")        //遇到""，跳过
            {
                Serial1_ConfigNum++;
            }

            QString str = QString("%1%2").arg(ConfigSend.at (Serial1_ConfigNum)).arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();
            //mySerial1->write (writeStr);
            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial1, writeStr))
                {
                    myTimer1.stop ();   //关闭定时器1
                    break;
                }
            }
            //connect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取

            SerialDisplay[0]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial1_ConfigNum))
                                              .arg (ConfigReturn.at (Serial1_ConfigNum))
                                              .arg (ConfigPara.at (Serial1_ConfigNum)));
        }
    }
}

void MainWindow::dealMyTimer2()       //定时器2发送
{
    //梁家榕：获取系统时间，2019年5月18日15:12:00
    LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    WriteDateTime = LocalDateTime.toString ("yyyy-MM-dd hh:mm:ss");     //格式化日期时间

    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial2_ConfigNum == -1)
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();
            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial2, writeStr))
                {
                    break;
                }
            }
            SerialResult[1]->setText ("等待");
            SerialResult[1]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体
            SerialDisplay[1]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
            if(SerialDisplay[1]->blockCount () > 100)
            {
                SerialDisplay[1]->clear ();
            }
        }
        else
        {
            if(ConfigSend.at (Serial2_ConfigNum) == "")        //遇到""，跳过
            {
                Serial2_ConfigNum++;
            }

            QString str = QString("%1%2").arg(ConfigSend.at (Serial2_ConfigNum)).arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();
            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial2, writeStr))
                {
                    myTimer2.stop ();   //关闭定时器
                    break;
                }
            }
            SerialDisplay[1]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial2_ConfigNum))
                                              .arg (ConfigReturn.at (Serial2_ConfigNum))
                                              .arg (ConfigPara.at (Serial2_ConfigNum)));
        }
    }
}

void MainWindow::dealMyTimer3()       //定时器3发送
{
    //梁家榕：获取系统时间，2019年5月18日15:12:00
    LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    WriteDateTime = LocalDateTime.toString ("yyyy-MM-dd hh:mm:ss");     //格式化日期时间

    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial3_ConfigNum == -1)
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();
            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial3, writeStr))
                {
                    break;
                }
            }
            SerialResult[2]->setText ("等待");
            SerialResult[2]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体
            SerialDisplay[2]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
            if(SerialDisplay[2]->blockCount () > 100)
            {
                SerialDisplay[2]->clear ();
            }
        }
        else
        {
            if(ConfigSend.at (Serial3_ConfigNum) == "")        //遇到""，跳过
            {
                Serial3_ConfigNum++;
            }

            QString str = QString("%1%2").arg(ConfigSend.at (Serial3_ConfigNum)).arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();

            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial3, writeStr))
                {
                    myTimer3.stop ();   //关闭定时器
                    break;
                }
            }
            SerialDisplay[2]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial3_ConfigNum))
                                              .arg (ConfigReturn.at (Serial3_ConfigNum))
                                              .arg (ConfigPara.at (Serial3_ConfigNum)));
        }
    }
}

void MainWindow::dealMyTimer4()       //定时器4发送
{
    //梁家榕：获取系统时间，2019年5月18日15:12:00
    LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    WriteDateTime = LocalDateTime.toString ("yyyy-MM-dd hh:mm:ss");     //格式化日期时间

    if(0 >= ConfigType.size ())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial4_ConfigNum == -1)
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();
            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial4, writeStr))
                {
                    break;
                }
            }
            SerialResult[3]->setText ("等待");
            SerialResult[3]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体
            SerialDisplay[3]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
            if(SerialDisplay[3]->blockCount () > 100)
            {
                SerialDisplay[3]->clear ();
            }
        }
        else
        {
            if(ConfigSend.at (Serial4_ConfigNum) == "")        //遇到""，跳过
            {
                Serial4_ConfigNum++;
            }

            QString str = QString("%1%2").arg(ConfigSend.at (Serial4_ConfigNum)).arg ("\r\n");
            QByteArray writeStr = str.toLatin1 ();
            while(1)   //发送配置指令
            {
                if(-1 != mySerial.WriteData(&mySerial4, writeStr))
                {
                    myTimer4.stop ();   //关闭定时器
                    break;
                }
            }
            SerialDisplay[3]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial4_ConfigNum))
                                              .arg (ConfigReturn.at (Serial4_ConfigNum))
                                              .arg (ConfigPara.at (Serial4_ConfigNum)));
        }
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月18日14:08:00
功能：修改波特率
参数1：arg1,波特率
参数2：
***********************************************************************/
void MainWindow::on_combB_Bps_activated(const QString &arg1)
{
    //关闭所有串口
    on_btn_Stop_1_clicked();
    on_btn_Stop_2_clicked();
    on_btn_Stop_3_clicked();
    on_btn_Stop_4_clicked();

    for(int i=0; i<4; i++)
    {
        SerialBaudShow[i]->setText ("波特率   " + arg1);     //显示默认波特率
        SerialResult[i]->setText ("等待");
        SerialResult[i]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月21日14:12:00
功能：切换账号
参数1：
参数2：
***********************************************************************/
void MainWindow::on_actionSwitch_triggered()
{
    QDlgLogin   *dlgLogin=new QDlgLogin; //创建对话框
    if (dlgLogin->exec()==QDialog::Accepted)//意思是如果，login登录成功，就跳转到主界面上去。
    {
        QString username = dlgLogin->getUserName();     //获取账户名
        int usergrade = dlgLogin->getUserGrade ();      //获取账户权限

        setUserName(username);      //设置登录名
        SetUserGrade (usergrade);   //设置权限
    }

    for(int i=0; i<4; i++)  //清空编辑区
    {
        SerialDisplay[i]->clear ();
    }

    //关闭所有串口
    on_btn_Stop_1_clicked();
    on_btn_Stop_2_clicked();
    on_btn_Stop_3_clicked();
    on_btn_Stop_4_clicked();
}

/**********************************************************************
作者：梁家榕
时间：2019年5月21日14:25:00
功能：锁定选项
参数1：
参数2：
***********************************************************************/
void MainWindow::on_actionLock_triggered()
{
    QString dlgTitle="输入密码对话框";
    QString txtLabel="请输入密码";
    QString defaultInput="";
    QLineEdit::EchoMode echoMode=QLineEdit::Password;//正常文字输入

    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode, defaultInput, &ok);//刘维球：自定义窗体，2019年4月28日18:02:07

    if (ok && text == "JIMI")
    {
        //锁定或解锁
        if("已锁定" == ui->actionLock->toolTip ())
        {
            ui->actionLock->setIcon (QIcon("://images/unlock.png"));   //图标改为开锁图标
            ui->actionsetting->setEnabled (true);
            ui->combB_Bps->setEnabled (true);
            ui->cBx_Td->setEnabled (true);
            ui->cBx_Blue->setEnabled (true);
            ui->cBx_Again->setEnabled (true);
            for(int i=0; i<4; i++)
            {
                SerialName[i]->setEnabled (true);
                SerialName[i]->setToolTip ("串口号");
            }
            ui->tab_2->setEnabled (true);

            ui->actionsetting->setToolTip ("设置");
            ui->combB_Bps->setToolTip ("波特率");
            ui->cBx_Td->setToolTip ("泰斗产品");
            ui->cBx_Blue->setToolTip ("蓝牙连接");
            ui->cBx_Again->setToolTip ("重测");
            ui->actionLock->setToolTip ("已解锁");
            ui->tab_2->setToolTip ("配置界面");
        }
        else if("已解锁" == ui->actionLock->toolTip ())
        {
            ui->actionLock->setIcon (QIcon("://images/lock.png"));   //图标改为锁定状态
            ui->actionsetting->setEnabled (false);
            ui->combB_Bps->setEnabled (false);
            ui->cBx_Td->setEnabled (false);
            ui->cBx_Blue->setEnabled (false);
            ui->cBx_Again->setEnabled (false);
            for(int i=0; i<4; i++)
            {
                SerialName[i]->setEnabled (false);
                SerialName[i]->setToolTip ("无法更改");
            }
            ui->tab_2->setEnabled (false);

            ui->actionsetting->setToolTip ("无法更改");
            ui->combB_Bps->setToolTip ("无法更改");
            ui->cBx_Td->setToolTip ("无法更改");
            ui->cBx_Blue->setToolTip ("无法更改");
            ui->cBx_Again->setToolTip ("无法更改");
            ui->actionLock->setToolTip ("已锁定");
            ui->tab_2->setToolTip ("无法更改");
        }
    }
    else
    {
        QMessageBox::warning(this, tr("错误提示"), tr("密码错误!"), tr("确 定"));
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月22日17:25:00
功能：测试结果上传数据库
参数1：
参数2：
***********************************************************************/
bool MainWindow::uploadDataBase(QStringList &Result, Database &db)
{
    //返回值拼接
    QString RStr;
    for(int i=0; i<Result.size (); i++)
    {
        if(i == Result.size ()-1)
        {
            RStr += Result.at (i);
        }
        else
        {
            RStr += Result.at (i) + "@@";
        }
#if 0
        qDebug() << Result.at (i);
#endif
    }
#if 1
    qDebug() << RStr << endl;
#endif
    QString HostIp = QString("%1%2%3").arg (localHost).arg ("@").arg (IPAddress);

    int Rows = db.Select_RowsNum("GPSOQC_Result");
#if 0
    qDebug() << Rows;
#endif

#if 1
    //上传数据
    QSqlQuery query;
    QString preStr = QString("insert into %1(Id, SN, SoftModel, Version, Result, TestId, TestGrade, Computer, TestSetting)"
                             "values(:id, :sn, :softModel, :version, :result, :testId, :testGrade, :computer, :testSetting)")
                            .arg (db_ResultList.at (db_ResultList.size ()-1));

    query.prepare (preStr);
    query.bindValue (":id", Rows+1);        //id
    query.bindValue (":sn", "SN");
    query.bindValue (":softModel", MachineName);    //MachineName，机型
    query.bindValue (":version", SoftWareName);     //SoftWare,订单号
    query.bindValue (":result", 1);                 //结果:1成功
    query.bindValue (":testId", OQC_Engineer);      //测试人员
    query.bindValue (":testGrade", UserGrade);      //测试人员权限
    query.bindValue (":computer", HostIp);          //主机名@ip
    query.bindValue (":testSetting", Station);      //所有配置信息
#endif
    return query.exec ();
}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日11:11:00
功能：返回值对比
参数1：thisStr, 返回值
参数2：ConfigNum, 串口对应指令
参数3：thisResultList, 结果列表
***********************************************************************/
bool MainWindow::Return_Contrast(QString &thisStr, int ConfigNum, QStringList &thisResultList)
{
    QStringList para_list;     //参数上下限列表lll
    QString RealPara;   //真正参数
    QStringList ParaTempList;   //辅助分隔参数

    int flag = 0;       //测试通过标志位

    if("" != thisStr)       //数据非空  判断返回值
    {
        if("" == ConfigPara.at (ConfigNum)      //返回值无参数
                && (ConfigReturn.at (ConfigNum).mid (ConfigReturn.at (ConfigNum).length ()-1, 1) != ":")) //返回值无参数，且返回值后不带:  即单纯只有返回值
        {
            RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum)),
                                    ConfigReturn.at (ConfigNum).length ());    //找到返回值字符串
#if 0
            qDebug() << RealPara;
#endif
            if(-1 != thisStr.indexOf (ConfigReturn.at (ConfigNum))) //发现两个返回值的相同部分
            {
                flag = 1;
            }
            else
            {
                flag = 0;
            }
        }
        else        //返回值有参数
        {
            //梁家榕：返回值参数判断，2019年5月16日11:23:00
            if("AT+RSSI?" == ConfigSend.at (ConfigNum))    //RSSI信号强度
            {
                RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum))+ConfigReturn.at (ConfigNum).length (),
                                        thisStr.length ()-ConfigReturn.at (ConfigNum).length ());    //找到返回值字符串，截取后半部分的真正参数
                RealPara = RealPara.mid (0, RealPara.indexOf ("\r\n")); //去掉换行
                para_list = ConfigPara.at (ConfigNum).split (",");  //分隔

                if(-1 != thisStr.indexOf (ConfigReturn.at (ConfigNum))  //发现两个返回值的相同部分
                        && RealPara.toInt () >= para_list.at (0).toInt ()   //上下限比较
                        && RealPara.toInt () <= para_list.at (1).toInt ())
                {
                    flag = 1;
                    RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum)), ConfigReturn.at (ConfigNum).length ())
                            +RealPara;  //返回值+参数
                }
                else
                {
                    flag = 0;
                }
            }

            else if("AT^GT_CM=ID,1" == ConfigSend.at (ConfigNum))    //芯片ID检查
            {
                if("项目名称不可变" == ConfigPara.at (ConfigNum) || "" == ConfigPara.at(ConfigNum))
                {
                    RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum))+ConfigReturn.at (ConfigNum).length (),
                                            thisStr.length ()-ConfigReturn.at (ConfigNum).length ());    //找到返回值字符串，截取后半部分的真正参数
                    ParaTempList = RealPara.split (" ");
                    RealPara.clear ();  //丢掉空格重新赋值
                    for(int i=0; i<ParaTempList.size (); i++)
                    {
                        RealPara += ParaTempList.at (i);
                    }
                    RealPara = RealPara.mid (0, RealPara.indexOf ("\r\n"));
#if 1
                    qDebug() << "///////////////////////////////////////" << RealPara.length();
#endif
                    if(-1 != thisStr.indexOf (ConfigReturn.at (ConfigNum))  //发现两个返回值的相同部分
                            && 32 == RealPara.length ()
                            && true == isEnglishAndDigital (RealPara))
                    {
                        flag = 1;
                        RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum)), ConfigReturn.at (ConfigNum).length ())
                                +RealPara;  //返回值+参数
                    }
                    else
                    {
                        flag = 0;
                    }
                }
            }

            else if("AT^GT_CM=VERSION" == ConfigSend.at (ConfigNum))   //软件版本
            {
                if( "项目名称不可变" == ConfigPara.at (ConfigNum))
                {
                    RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum))+ConfigReturn.at (ConfigNum).length (),
                                            thisStr.length ()-ConfigReturn.at (ConfigNum).length ());    //找到返回值字符串，截取后半部分的真正参数
                    //返回：[VERSION]GT02F_32_61DM2R1_D23_R0_V15
                    RealPara = RealPara.mid (0, RealPara.indexOf ("\r\n"));
#if 0
                    qDebug() << RealPara;
#endif
                    if(-1 != thisStr.indexOf (ConfigReturn.at (ConfigNum))  //发现两个返回值的相同部分
                            && -1 != RealPara.indexOf ("GT02F"))  //发现与订单号的前半部分相同
                    {
                        flag = 1;
                        RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum)), ConfigReturn.at (ConfigNum).length ())
                                +RealPara;  //返回值+参数
                    }
                    else
                    {
                        flag = 0;
                    }
                }
            }

            else if("AT^GT_CM=LINK" == ConfigSend.at (ConfigNum))    //查/写LINK
            {
                RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum))+ConfigReturn.at (ConfigNum).length (),
                                        thisStr.length ()-ConfigReturn.at (ConfigNum).length ());    //找到返回值字符串，截取后半部分的真正参数
                RealPara = RealPara.mid (0, 1);     //截取一位
#if 0
                qDebug() << RealPara;
#endif
                if(-1 != thisStr.indexOf (ConfigReturn.at (ConfigNum))  //发现两个返回值的相同部分
                        && ConfigPara.at (ConfigNum).toInt () == RealPara.toInt ()) //参数相同
                {
                    flag = 1;
                    RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum)), ConfigReturn.at (ConfigNum).length ())
                            +RealPara;  //返回值+参数
#if 0
                    qDebug() << RealPara;
#endif
                }
                else
                {
                    flag = 0;
                }
            }

            else if("AT^GT_CM=IMEI" == ConfigSend.at (ConfigNum))    //IMEI
            {
                RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum))+ConfigReturn.at (ConfigNum).length (),
                                        thisStr.length ()-ConfigReturn.at (ConfigNum).length ());    //找到返回值字符串，截取后半部分的真正参数
                RealPara = RealPara.mid (0, RealPara.indexOf ("\r\n"));
#if 0
                    qDebug() << RealPara;
#endif
                if(-1 != thisStr.indexOf (ConfigReturn.at (ConfigNum))  //发现两个返回值的相同部分
                        && 15 == RealPara.length ()) //15位
                {
                    flag = 1;
                    RealPara = thisStr.mid (thisStr.indexOf (ConfigReturn.at (ConfigNum)), ConfigReturn.at (ConfigNum).length ())
                            +RealPara;  //返回值+参数
#if 0
                    qDebug() << RealPara;
#endif
                }
                else
                {
                    flag = 0;
                }
            }

            //莫秋兰 RDA指令返回值判断
            if("Dongle蓝牙RSSI信号强度" == ConfigName.at(ConfigNum))
            {
                RealPara = thisStr.mid(thisStr.indexOf(ConfigReturn.at(ConfigNum))+ConfigReturn.at(ConfigNum).length(),
                                       thisStr.length()-ConfigReturn.at(ConfigNum).length());

                para_list = ConfigPara.at(ConfigNum).split(",");

                if(-1 != thisStr.indexOf(ConfigReturn.at(ConfigNum))
                        && RealPara.toInt() >= para_list.at(0).toInt()
                        && RealPara.toInt() <= para_list.at(1).toInt())
                {
                    flag = 1;
                }
                else {
                   flag = 0;
                }
            }
        }

    }

    if(1 == flag && ConfigNum != -1)
    {
        thisResultList.append (RealPara);    //加入结果列表
    }

    return flag;
}

/**********************************************************************
作者：梁家榕
时间：2019年5月24日10:45:00
功能：判断字符串是否为字母和数字组成
参数1：theStr, 目标字符串
参数2：
***********************************************************************/
bool MainWindow::isEnglishAndDigital(QString &theStr)
{
    bool flag = 0;
    QByteArray ba = theStr.toLatin1 ();
    const char *s = ba.data ();
    while(*s)
    {
        if((*s >= '0' && *s <= '9')
                || (*s >= 'a' && *s <= 'z')
                || (*s >= 'A' && *s <= 'Z'))
        {
            flag = true;
        }
        else
        {
            flag = false;
            break;
        }
        s++;
    }
    return flag;
}

/**********************************************************************
作者：梁家榕
时间：2019年5月24日16:29:00
功能：判断指令延时时间
参数1：theStr, 目标字符串
参数2：
***********************************************************************/
int MainWindow::isDLTimes(const QString &theStr)
{
    if(-1 != theStr.indexOf ("DL"))
    {
        return theStr.mid(theStr.indexOf ("DL")+2, theStr.length ()-theStr.indexOf ("DL")-2).toInt ();
    }
    else
    {
        return 0;
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月28日8:29:00
功能：确定配置
参数1：
参数2：
***********************************************************************/
void MainWindow::on_btnok_clicked()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);

    msgBox.setWindowTitle(tr("提示！"));
    msgBox.setText(tr("请确认订单！"));
    QPushButton *Button1 = msgBox.addButton(tr("确定"),QMessageBox::AcceptRole);
    QPushButton *Button2 = msgBox.addButton(tr("取消"),QMessageBox::RejectRole);

    msgBox.exec();
    if (msgBox.clickedButton() == Button1)
    {
        ChooseTime = ui->comboBox_SelectTime->currentText ();
        SoftWareName = ui->comboBox_Proname->currentText ();
        SoftWareNameShow->setText("订单号："+ QString("%1").arg(SoftWareName,10)+"     ");      //更新状态栏订单号
        MachineName = db_Config.Select_OneInfo (QString("select * from %1 where SoftWare = '%2'").arg (Configlist.at (Configlist.size ()-1)).arg (SoftWareName),
                                         "MachineName"); //机型确认
        Station = db_Config.Select_OneInfo (QString("select * from %1 where SoftWare = '%2'").arg (Configlist.at (Configlist.size ()-1)).arg (SoftWareName),
                                      "Station"); //所有配置信息确认
#if 0
        qDebug() << SoftWareName << MachineName << Station << ChooseTime;
#endif

        //清空配置列表
        ConfigType.clear ();
        ConfigName.clear ();
        ConfigReturn.clear ();
        ConfigPara.clear ();
        ConfigSend.clear ();
        //添加到配置列表
        for(int i=0; i<ui->tableWidget->rowCount (); i++)   //行
        {
            for(int j=0; j<ui->tableWidget->columnCount (); j++)    //列
            {
                if(ui->tableWidget->item (i,j) == NULL || ui->tableWidget->item (i,j)->text () == "")  //为空
                {
                    ui->tableWidget->setItem (i, j, new QTableWidgetItem(""));
                }
                if(ui->tableWidget->item (i,j) != NULL)    //不为空
                {
                    if(j==0)
                    {
                        ConfigType.append (ui->tableWidget->item (i,j)->text ());
                    }
                    if(j==1)
                    {
                        ConfigName.append (ui->tableWidget->item (i,j)->text ());
                    }
                    if(j==2)
                    {
                        ConfigReturn.append (ui->tableWidget->item (i,j)->text ());
                    }
                    if(j==3)
                    {
                        ConfigPara.append (ui->tableWidget->item (i,j)->text ());
                    }
                    if(j==4)
                    {
                        ConfigSend.append (ui->tableWidget->item (i,j)->text ());
                    }
                }
            }
        }
#if 0
        for(int i=0; i<ConfigType.size (); i++)   //行
        {
            qDebug() << ConfigType.at(i) <<  ConfigName.at(i) << ConfigReturn.at(i) << ConfigPara.at(i) << ConfigSend.at(i);
        }
#endif

        for(int i=0; i<ConfigType.size (); i++)     //查询第一个测试指令下标
        {
            if("共有指令" != ConfigType.at (i))
            {
                Config_StartNum = i;
                break;
            }
        }

        ui->btnok->setEnabled (false);
        ui->tab_2->setEnabled (false);
        ui->tabWidget->setCurrentIndex (0);
    }

#if 0
    else if (msgBox.clickedButton() == Button2)
    {
        qDebug() << "取消" << endl ;
        ConfigType.clear ();
        ConfigName.clear ();
        ConfigReturn.clear ();
        ConfigPara.clear ();
        ConfigSend.clear ();
        SoftWareName = QString("");  //订单号撤销
        MachineName = QString("");   //机型
        Station = QString("");       //所有配置信息
    }
#endif

}

/**********************************************************************
作者：梁家榕
时间：2019年5月28日8:29:00
功能：取消配置
参数1：
参数2：
***********************************************************************/
void MainWindow::on_btncancel_clicked()
{
    ui->comboBox_Proname->setCurrentText (SoftWareName);    //重新显示当前订单
    ui->comboBox_SelectTime->setCurrentText (ChooseTime);   //重新显示当前时间
    add_ConfigDataTable(ConfigType, ConfigName, ConfigReturn, ConfigPara, ConfigSend);  //重新加载到配置表
    ui->btnok->setEnabled (false);
    ui->tab_2->setEnabled (false);
    ui->tabWidget->setCurrentIndex (0);
}

/**********************************************************************
作者：梁家榕
时间：2019年5月12日17:08:00
功能：配置信息加入到数据表格
参数1：
参数2：
***********************************************************************/
void MainWindow::add_ConfigDataTable(QStringList &TempType, QStringList &TempName,
                                     QStringList &TempReturn, QStringList &TempPara,
                                     QStringList &TempSend)
{
    int rows = 0;        //总行数
    rows = ui->tableWidget->rowCount ();
    //qDebug() << "rows1:" << ui->tableWidget->rowCount ();
    if(TempType.size () > rows)       //缺少行
    {
        for(int isrow=rows; isrow<TempType.size (); isrow++)
        {
            ui->tableWidget->insertRow (isrow);
        }
    }

    rows = ui->tableWidget->rowCount ();  //重新获取总行数
    if(TempType.size () < rows)       //有多余行
    {
        for(int rvrow=rows; rvrow>TempType.size ()-1; rvrow--)     //从后往前删 eg：5条指令size=5,最后一条row=4
        {
            ui->tableWidget->removeRow (rvrow); //删除多余行
        }
    }
    //qDebug() << "rows2:" << ui->tableWidget->rowCount ();

    ui->tableWidget->clear ();
    QStringList tableHeader;
    tableHeader << "指令类型" << "设置项目" << "返回值" << "可变参数" << "指令";
    ui->tableWidget->setHorizontalHeaderLabels (tableHeader);
    for(int i=0; i<TempType.size (); i++)     //添加到配置表
    {
        if(i < TempType.size ())
        {
            ui->tableWidget->setItem (i, 0, new QTableWidgetItem(TempType.at (i)));
        }
        if(i < TempName.size ())
        {
            ui->tableWidget->setItem (i, 1, new QTableWidgetItem(TempName.at (i)));
        }
        if(i < TempReturn.size ())
        {
            ui->tableWidget->setItem (i, 2, new QTableWidgetItem(TempReturn.at (i)));
        }
        if(i < TempPara.size ())
        {
            ui->tableWidget->setItem (i, 3, new QTableWidgetItem(TempPara.at (i)));
        }
        if(i < TempSend.size ())
        {
            ui->tableWidget->setItem (i, 4, new QTableWidgetItem(TempSend.at (i)));
        }
    }

    for(int i=0; i<ui->tableWidget->rowCount (); i++)     //行
    {
        if("共有指令" == TempType.at (i) && ui->tableWidget->item (i,0)->text () != NULL)
        {
            for(int j=0; j<ui->tableWidget->columnCount (); j++)    //设置颜色      //列
            {
                if(ui->tableWidget->item (i,j) == NULL || ui->tableWidget->item (i,j)->text () == "")  //为空
                {
                    ui->tableWidget->setItem (i, j, new QTableWidgetItem(""));
                    ui->tableWidget->item (i, j)->setForeground (Qt::blue);
                }
                if(ui->tableWidget->item (i,j)->text () != NULL)
                {
                    ui->tableWidget->item (i, j)->setForeground (Qt::blue);
                }
            }
        }
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日11:33:00
功能：全部订单加载
参数1：
参数2：
***********************************************************************/
void MainWindow::Proname_Load_All()     //加载全部订单
{
    QSqlQuery query;
    QString readStr;

    QString str = QString("select SoftWare from %1").arg (Configlist.at (Configlist.size ()-1));

    if(query.exec (str))
    {
        while(query.next ())
        {
            readStr = query.value (query.record ().indexOf ("SoftWare")).toString ();
            ui->comboBox_Proname->addItem (readStr);
        }
    }
    else
    {
        qDebug() << query.lastError ();
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日11:33:00
功能：部分订单加载
参数1：
参数2：
***********************************************************************/
void MainWindow::Proname_Load_Part(const QString &Str)    //部分订单加载
{
    QSqlQuery query;
    QString readStr;
    QDateTime SqlTime;      //服务器的时间
    qint64 Days;            //时间差
    QString selStr;         //目标时间

    //梁家榕：获取系统时间，2019年5月18日15:12:00
    LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    SelectDateTime = LocalDateTime.toString ("yyyy-MM-dd");     //格式化日期时间

    QString str = QString("select RecordTime, SoftWare from %1").arg (Configlist.at (Configlist.size ()-1));      //查询创建时间
    if(query.exec (str))
    {
        while(query.next ())
        {
            readStr = query.value (query.record ().indexOf ("RecordTime")).toString ();   //查询到的字段
            SqlTime = QDateTime(QDate(readStr.mid (0, 4).toInt (),      //年     2019-03-27  0~4
                                      readStr.mid (5, 2).toInt (),       //月     2019-03-27  5~6
                                      readStr.mid (8, 2).toInt ()));     //日     2019-03-27  8~9
            Days = SqlTime.daysTo (LocalDateTime);      //计算时间差

            if("1周内" == Str && 7 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("半个月内" == Str && 15 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("1个月内" == Str && 30 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("2个月内" == Str && 30*2 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("3个月内" == Str && 30*3 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("半年内" == Str && 30*6 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("1年内" == Str && 30*12 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("全部" == Str)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
        }
    }
    else
    {
        qDebug() << query.lastError ();
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日11:33:00
功能：选择订单
参数1：arg1,订单号
参数2：
***********************************************************************/
void MainWindow::on_comboBox_Proname_activated(const QString &arg1)
{
    QStringList TempType;
    QStringList TempName;
    QStringList TempReturn;
    QStringList TempPara;
    QStringList TempSend;
    QString SoftWareTemp;



    QString Str = QString("select * from %1 where SoftWare = '%2'").arg (Configlist.at (Configlist.size ()-1)).arg (arg1);

    db_Config.Select_ConfigInfo(TempType, TempName, TempReturn,
                                           TempPara, TempSend, Str);       //查询配置信息

    SoftWareTemp = db_Config.Select_OneInfo (QString("select * from %1 where SoftWare = '%2'").arg (Configlist.at (Configlist.size ()-1)).arg (arg1),
                                             "SoftWare"); //梁家榕：订单号确认，2019年5月16日16:24:00
    if(SoftWareTemp == "")
    {
        QMessageBox::warning(this, tr("错误提示"), tr("查询失败，请输入正确订单号！"), tr("确 定"));
    }
    else if(TempName.size () <=0 )     //查不到配置时
    {
        QMessageBox::warning(this, tr("错误提示"), tr("订单中没有指令！"), tr("确 定"));
    }
    add_ConfigDataTable(TempType, TempName, TempReturn, TempPara, TempSend);      //添加到配置表
}

/**********************************************************************
作者：梁家榕
时间：2019年5月20日10:40:00
功能：选择订单查询方式
参数1：checked，选择状态
参数2：
***********************************************************************/
void MainWindow::on_radioButton_clicked(bool checked)
{
    if(true == checked)        //选择精确查询
    {
        ui->radioButton->setText ("精确查询");
        ui->tableWidget->clear ();      //清空配置表
    }
    else
    {
        ui->radioButton->setText ("模糊查询");
        ui->tableWidget->clear ();      //清空配置表
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月20日10:43:00
功能：精准查询订单配置
参数1：
参数2：
***********************************************************************/
void MainWindow::on_pushButton_clicked()
{
    on_comboBox_Proname_activated(ui->comboBox_Proname->currentText ());        //查询配置
}

/**********************************************************************
作者：梁家榕
时间：2019年5月20日14:20:00
功能：选择订单查询时间
参数1：
参数2：
***********************************************************************/
void MainWindow::on_comboBox_SelectTime_activated(const QString &arg1)
{
    ui->comboBox_Proname->clear (); //清空选项-订单号
    Proname_Load_Part (arg1);
}

/**********************************************************************
作者：梁家榕
时间：2019年5月29日11:14:00
功能：重新获取串口
参数1：
参数2：
***********************************************************************/
void MainWindow::on_actionReSerial_triggered()
{
    on_btn_Stop_1_clicked();
    on_btn_Stop_2_clicked();
    on_btn_Stop_3_clicked();
    on_btn_Stop_4_clicked();

    //梁家榕：初始化默认波特率、串口号，2019年5月8日10:11:00
    for(int i=0; i<4; i++)
    {
        SerialName[i]->clear ();    //先删除原先的分配
        int Num = mySerial.SerachSerialPortName (SerialName[i]);  //寻找可用串口 2019/5/14 16:57:00
        if(i < Num)
        {
            SerialName[i]->setCurrentIndex (i);     //显示默认串口
        }
        else
        {
            SerialName[i]->clear ();    //串口不够，不分配
        }
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月29日14:26:00
功能：超时处理
参数1：
参数2：
***********************************************************************/
void MainWindow::dealMyTimeOut()
{
    //梁家榕：获取系统时间，2019年5月18日15:12:00
    LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    WriteDateTime = LocalDateTime.toString ("yyyy-MM-dd hh:mm:ss");     //格式化日期时间

    static QString Str1 = "";
    static QString Str2 = "";
    static QString Str3 = "";
    static QString Str4 = "";

    static int Num1 = 1;
    static int Num2 = 1;
    static int Num3 = 1;
    static int Num4 = 1;
    int needNum = 1;

    for(int i=0; i< ConfigReturn.size (); i++)
    {
        if("重试次数" == ConfigReturn.at (i))
        {
            needNum = ConfigPara.at (i).toInt ();
            break;
        }
    }

    //串口1
    if(Str1 == SerialDisplay[0]->toPlainText () && mySerial1.isOpen ()) //没有改变
    {
        QByteArray writeStr;
        QString str;
        if(Num1 == needNum) //已达最大值
        {
            if(Serial1_ConfigNum != -1)     //如果是TEST指令   不用判断成功和失败
            {
                Serial1_ConfigNum = -1;
                SerialResult[0]->setText ("失败");
                SerialResult[0]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[0]->appendPlainText("********  产品测试失败  ********\r\n");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial1_Result.clear ();    //清空结果
            }
            Num1 = 1;
            str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            writeStr = str.toLatin1 ();
        }
        else
        {
            if(Serial1_ConfigNum == -1)
            {
                str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            }
            else
            {
                str = QString("%1%2").arg(ConfigSend.at (Serial1_ConfigNum)).arg ("\r\n");
            }
            writeStr = str.toLatin1 ();
        }
        while(1)   //发送配置指令
        {
            if(-1 != mySerial.WriteData(&mySerial1, writeStr))
            {               
                break;
            }
        }
        Num1 += 1;
        if(Serial1_ConfigNum == -1) //TEST指令
        {
            SerialDisplay[0]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
        }
        else
        {
            SerialDisplay[0]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial1_ConfigNum))
                                              .arg (ConfigReturn.at (Serial1_ConfigNum))
                                              .arg (ConfigPara.at (Serial1_ConfigNum)));
        }
        if(SerialDisplay[0]->blockCount () > 100)
        {
            SerialDisplay[0]->clear ();
        }
        Str1 = SerialDisplay[0]->toPlainText ();
    }
    else if(Str1 != SerialDisplay[0]->toPlainText () && mySerial1.isOpen ())
    {
        Num1 = 1;
        Str1 = SerialDisplay[0]->toPlainText ();
    }

    //串口2
    if(Str2 == SerialDisplay[1]->toPlainText () && mySerial2.isOpen ())
    {
        QByteArray writeStr;
        QString str;
        if(Num2 == needNum) //已打最大值
        {
            if(Serial2_ConfigNum != -1)     //如果是TEST指令   不用判断成功和失败
            {
                Serial2_ConfigNum = -1;
                SerialResult[1]->setText ("失败");
                SerialResult[1]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[1]->appendPlainText("********  产品测试失败  ********\r\n");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial2_Result.clear ();    //清空结果
            }
            Num2 = 1;
            str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            writeStr = str.toLatin1 ();
        }
        else
        {
            if(Serial2_ConfigNum == -1)
            {
                str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            }
            else
            {
                str = QString("%1%2").arg(ConfigSend.at (Serial2_ConfigNum)).arg ("\r\n");
            }
            writeStr = str.toLatin1 ();
        }
        while(1)   //发送配置指令
        {
            if(-1 != mySerial.WriteData(&mySerial2, writeStr))
            {

                break;
            }
        }
        Num2 += 1;
        if(Serial2_ConfigNum == -1)
        {
            SerialDisplay[1]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
        }
        else
        {
            SerialDisplay[1]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial2_ConfigNum))
                                              .arg (ConfigReturn.at (Serial2_ConfigNum))
                                              .arg (ConfigPara.at (Serial2_ConfigNum)));
        }
        if(SerialDisplay[1]->blockCount () > 100)
        {
            SerialDisplay[1]->clear ();
        }
        Str2 = SerialDisplay[1]->toPlainText ();
    }
    else if(Str2 != SerialDisplay[1]->toPlainText () && mySerial2.isOpen ())
    {
        Num2 = 1;
        Str2 = SerialDisplay[1]->toPlainText ();
    }

    //串口3
    if(Str3 == SerialDisplay[2]->toPlainText () && mySerial3.isOpen ())
    {
        QByteArray writeStr;
        QString str;
        if(Num3 == needNum) //已打最大值
        {
            if(Serial3_ConfigNum != -1)     //如果是TEST指令   不用判断成功和失败
            {
                Serial3_ConfigNum = -1;
                SerialResult[2]->setText ("失败");
                SerialResult[2]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[2]->appendPlainText("********  产品测试失败  ********\r\n");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial3_Result.clear ();    //清空结果
            }
            Num3 = 1;
            str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            writeStr = str.toLatin1 ();
        }
        else
        {
            if(Serial3_ConfigNum == -1)
            {
                str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            }
            else
            {
                str = QString("%1%2").arg(ConfigSend.at (Serial3_ConfigNum)).arg ("\r\n");
            }
            writeStr = str.toLatin1 ();
        }
        while(1)   //发送配置指令
        {
            if(-1 != mySerial.WriteData(&mySerial3, writeStr))
            {

                break;
            }
        }
        Num3 += 1;
        if(Serial3_ConfigNum == -1)
        {
            SerialDisplay[2]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
        }
        else
        {
            SerialDisplay[2]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial3_ConfigNum))
                                              .arg (ConfigReturn.at (Serial3_ConfigNum))
                                              .arg (ConfigPara.at (Serial3_ConfigNum)));
        }
        if(SerialDisplay[2]->blockCount () > 100)
        {
            SerialDisplay[2]->clear ();
        }
        Str3 = SerialDisplay[2]->toPlainText ();
    }
    else if(Str3 != SerialDisplay[2]->toPlainText () && mySerial3.isOpen ())
    {
        Num3 = 1;
        Str3 = SerialDisplay[2]->toPlainText ();
    }

    //串口4
    if(Str4 == SerialDisplay[3]->toPlainText () && mySerial4.isOpen ())
    {
        QByteArray writeStr;
        QString str;
        if(Num4 == needNum) //已打最大值
        {
            if(Serial4_ConfigNum != -1)     //如果是TEST指令   不用判断成功和失败
            {
                Serial4_ConfigNum = -1;
                SerialResult[3]->setText ("失败");
                SerialResult[3]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                SerialDisplay[3]->appendPlainText("********  产品测试失败  ********\r\n");

                test_Num ++; fail_Num++;
                updateStatus ();    //刷新状态栏
                Serial4_Result.clear ();    //清空结果
            }
            Num4 = 1;
            str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            writeStr = str.toLatin1 ();
        }
        else
        {
            if(Serial4_ConfigNum == -1)
            {
                str = QString("%1%2").arg("AT^GT_CM=TEST").arg ("\r\n");
            }
            else
            {
                str = QString("%1%2").arg(ConfigSend.at (Serial4_ConfigNum)).arg ("\r\n");
            }
            writeStr = str.toLatin1 ();
        }
        while(1)   //发送配置指令
        {
            if(-1 != mySerial.WriteData(&mySerial4, writeStr))
            {

                break;
            }
        }
        Num4 += 1;
        if(Serial4_ConfigNum == -1)
        {
            SerialDisplay[3]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
        }
        else
        {
            SerialDisplay[3]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg (ConfigSend.at (Serial4_ConfigNum))
                                              .arg (ConfigReturn.at (Serial4_ConfigNum))
                                              .arg (ConfigPara.at (Serial4_ConfigNum)));
        }
        if(SerialDisplay[3]->blockCount () > 100)
        {
            SerialDisplay[3]->clear ();
        }
        Str4 = SerialDisplay[3]->toPlainText ();
    }
    else if(Str4 != SerialDisplay[3]->toPlainText () && mySerial4.isOpen ())
    {
        Num4 = 1;
        Str4 = SerialDisplay[3]->toPlainText ();
    }
}



/**********************************************************************
作者：梁家榕
时间：2019年6月01日8:26:00
功能：设定通信方式标志位 0 -- 普通串口  1 -- RDA串口
参数1：
参数2：
***********************************************************************/
int MainWindow::setSerial_RDA_Flag()
{
    if(ui->cBx_Again_2->isChecked())
    {
        Serial_RDA_Flag = 1;    //RDA模式
    }
    else
    {
        Serial_RDA_Flag = 0;    //普通串口模式
    }

    qDebug()<<"Serial_RDA_Flag = "<<Serial_RDA_Flag;
    return Serial_RDA_Flag;
}


//莫秋兰  莫秋兰
/**********************************************************************
作者：莫秋兰
时间：2019年6月1日8:40:00
功能：RDA串口1写定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_WTimerdeal1()
{
    //获取系统当前时间
    LocalDateTime = QDateTime::currentDateTime();
    WriteDateTime = LocalDateTime.toString("yy-MM-dd hh:mm:ss");
    qDebug()<<"系统当前时间为："<<WriteDateTime;

    if(0 >= ConfigType.size())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial1_ConfigNum == -1) //发送TEST指令
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg("\r\n");
            while(1)
            {
                //每次发送之前都要清空返回值存储区
                RDAComReceive[0].clear();   /////////
                ReturnMsg1.clear();

                if(true == mySerial.RDAComWriteData(0,str))
                {
                    break;
                }
            }
            SerialResult[0]->setText ("等待");
            SerialResult[0]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            SerialDisplay[0]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
           // 开启数据读取定时器
           RDA_RTimer1.start(400);

            if(SerialDisplay[0]->blockCount () > 100)
            {
                SerialDisplay[0]->clear ();
            }
            //
        }
        else
        {
            qDebug()<<"Serial_ConfigNum = "<<Serial1_ConfigNum;
            if(ConfigSend.at(Serial1_ConfigNum) == "")
                Serial1_ConfigNum++;

            //每次写之前都先清空数组
            RDAComReceive[0].clear();
            ReturnMsg1.clear();

            if(mySerial.RDAComWriteData(0,ConfigSend.at(Serial1_ConfigNum)))
            {
                //SerialFlag = 1;

               qDebug()<<"发送的指令为："<<ConfigSend.at(Serial1_ConfigNum);

               SerialDisplay[0]->appendPlainText(QString("%1   发送%2    参考：%3%4")
                                                  .arg(WriteDateTime)                       //时间
                                                  .arg(ConfigSend.at(Serial1_ConfigNum))     //发送的指令
                                                  .arg(ConfigReturn.at(Serial1_ConfigNum))   //指令返回值
                                                  .arg(ConfigPara.at(Serial1_ConfigNum)));   //指令返回值参数


               /*开启数据读取定时器
               RDA_RTimer1.start(400);*/

    //           Serial_ConfigNum++;

            }else {

                qDebug()<<"指令发送失败！";
            }

        }
        //关闭定时器
       // timer.stop();
    }

}


/**********************************************************************
作者：莫秋兰
时间：2019年6月1日9:10:00
功能：RDA串口1读定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_RTimerdeal1()
{
    static int thisNum = 1; //初始化已重复次数为1
    int needNum = 1;    //失败重复读取次数


    //获取可失败的最大次数
    for(int i = 0; i < ConfigReturn.size(); i++)
    {
        if("重试次数" == ConfigReturn.at(i))
        {
            needNum = ConfigPara.at(i).toInt();
            break;
        }


    }

    if(Config_StartNum == Serial1_ConfigNum)
    {
        SerialResult[0]->setText ("测试");
        SerialResult[0]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    if(RDAComReceive[0] != "")
    {

        ReturnMsg1 = RDAComReceive[0];
        RDAComReceive[0].clear();
        qDebug()<<"返回数组中的值："<<ReturnMsg1;

        if(Serial1_ConfigNum == -1)
        {
            if(-1 != ReturnMsg1.indexOf("TEST_OK"))
            {
                Serial1_ConfigNum = Config_StartNum;

                SerialDisplay[0]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (ReturnMsg1.mid (0, ReturnMsg1.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[0]->clear (); //清空编辑区
                //disconnect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取
                return ;
            }
        }
        else
        {
            //进行返回值的判断
            if(1 == Return_Contrast(ReturnMsg1,Serial1_ConfigNum,Serial1_Result))
            {
                SerialDisplay[0]->appendPlainText(QString("    返回：%1   正常")
                                                   .arg(ReturnMsg1.mid(0,ReturnMsg1.length()-2)));

                //没有待测项
                if(Serial1_ConfigNum == ConfigType.size() - 1)
                {

                    Serial1_ConfigNum = -1; //指令重头发送
                    SerialResult[0]->setText ("成功");
                    SerialResult[0]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                    SerialDisplay[0]->appendPlainText("********  产品测试成功  ********\r\n");
                    test_Num ++; pass_Num++;
                    updateStatus ();    //刷新状态栏

                    if(uploadDataBase(Serial1_Result, updb_1))     //拼接字符串，上传数据库
                    {
                        SerialDisplay[0]->appendPlainText("********  数据上传成功  ********\r\n");
                    }
                    else
                    {
                        fmt.setForeground (Qt::red);    //设置前景色(字体色)
                        SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式
                        SerialDisplay[0]->appendPlainText("********  数据上传失败  ********\r\n");
                        fmt.setForeground (Qt::black);    //设置前景色(字体色)
                        SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    }
                    Serial1_Result.clear ();    //清空结果
                    //SerialDisplay[0]->clear (); //清空编辑区
                }
                else {

                    Serial1_ConfigNum++;

                }
                thisNum = 1;    //将失败次数变为1

            }
            else {

                fmt.setForeground (Qt::red);    //设置前景色(字体色)
                SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式
                SerialDisplay[0]->appendPlainText(QString("    返回:%1  异常")
                                                  .arg (ReturnMsg1.mid (0, ReturnMsg1.length ()-2)));          //返回值
                fmt.setForeground (Qt::black);    //设置前景色(字体色)
                SerialDisplay[0]->mergeCurrentCharFormat (fmt);    //使用当前格式

                //重复次数已够
                if(thisNum == needNum)
                {
                    thisNum = 1;
                    Serial1_ConfigNum = -1;

                    SerialResult[0]->setText ("失败");
                    SerialResult[0]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                    SerialDisplay[0]->appendPlainText("********  产品测试失败  ********\r\n");

                    test_Num ++; fail_Num++;
                    updateStatus ();    //刷新状态栏
                    Serial1_Result.clear();
                }
                else {
                    thisNum++;
                }

            }
        }

    }
}



/**********************************************************************
作者：莫秋兰
时间：2019年6月1日9:40:00
功能：RDA串口2写定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_WTimerdeal2()
{
    //获取系统当前时间
    LocalDateTime = QDateTime::currentDateTime();
    WriteDateTime = LocalDateTime.toString("yy-MM-dd hh:mm:ss");
    qDebug()<<"系统当前时间为："<<WriteDateTime;

    if(0 >= ConfigType.size())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial2_ConfigNum == -1) //发送TEST指令
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg("\r\n");
            while(1)
            {
                //每次发送之前都要清空返回值存储区
                RDAComReceive[1].clear();   /////////
                ReturnMsg2.clear();

                if(true == mySerial.RDAComWriteData(1,str))
                {
                    break;
                }
            }
            SerialResult[1]->setText ("等待");
            SerialResult[1]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            SerialDisplay[1]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
           // 开启数据读取定时器
           RDA_RTimer2.start(400);

            if(SerialDisplay[1]->blockCount () > 100)
            {
                SerialDisplay[1]->clear ();
            }
            //
        }
        else
        {
            qDebug()<<"Serial_ConfigNum = "<<Serial2_ConfigNum;
            if(ConfigSend.at(Serial2_ConfigNum) == "")
                Serial2_ConfigNum++;

            //每次写之前都先清空数组
            RDAComReceive[1].clear();
            ReturnMsg2.clear();

            if(mySerial.RDAComWriteData(1,ConfigSend.at(Serial2_ConfigNum)))
            {
                //SerialFlag = 1;

               qDebug()<<"发送的指令为："<<ConfigSend.at(Serial2_ConfigNum);

               SerialDisplay[1]->appendPlainText(QString("%1   发送%2    参考：%3%4")
                                                  .arg(WriteDateTime)                       //时间
                                                  .arg(ConfigSend.at(Serial2_ConfigNum))     //发送的指令
                                                  .arg(ConfigReturn.at(Serial2_ConfigNum))   //指令返回值
                                                  .arg(ConfigPara.at(Serial2_ConfigNum)));   //指令返回值参数


               /*开启数据读取定时器
               RDA_RTimer1.start(400);*/

    //           Serial_ConfigNum++;

            }else {

                qDebug()<<"指令发送失败！";
            }

        }
        //关闭定时器
       // timer.stop();
    }

}

/**********************************************************************
作者：莫秋兰
时间：2019年6月1日10:10:00
功能：RDA串口2读定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_RTimerdeal2()
{
    static int thisNum = 1; //初始化已重复次数为1
    int needNum = 1;    //失败重复读取次数


    //获取可失败的最大次数
    for(int i = 0; i < ConfigReturn.size(); i++)
    {
        if("重试次数" == ConfigReturn.at(i))
        {
            needNum = ConfigPara.at(i).toInt();
            break;
        }


    }

    if(Config_StartNum == Serial2_ConfigNum)
    {
        SerialResult[1]->setText ("测试");
        SerialResult[1]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    if(RDAComReceive[1] != "")
    {

        ReturnMsg2 = RDAComReceive[1];
        RDAComReceive[1].clear();
        qDebug()<<"返回数组中的值："<<ReturnMsg2;

        if(Serial2_ConfigNum == -1)
        {
            if(-1 != ReturnMsg2.indexOf("TEST_OK"))
            {
                Serial2_ConfigNum = Config_StartNum;

                SerialDisplay[1]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (ReturnMsg2.mid (0, ReturnMsg2.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[1]->clear (); //清空编辑区
                //disconnect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取
                return ;
            }
        }
        else
        {
            //进行返回值的判断
            if(1 == Return_Contrast(ReturnMsg2,Serial2_ConfigNum,Serial2_Result))
            {
                SerialDisplay[1]->appendPlainText(QString("    返回：%1   正常")
                                                   .arg(ReturnMsg2.mid(0,ReturnMsg2.length()-2)));

                //没有待测项
                if(Serial2_ConfigNum == ConfigType.size() - 1)
                {

                    Serial2_ConfigNum = -1; //指令重头发送
                    SerialResult[1]->setText ("成功");
                    SerialResult[1]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                    SerialDisplay[1]->appendPlainText("********  产品测试成功  ********\r\n");
                    test_Num ++; pass_Num++;
                    updateStatus ();    //刷新状态栏

                    if(uploadDataBase(Serial2_Result, updb_2))     //拼接字符串，上传数据库
                    {
                        SerialDisplay[1]->appendPlainText("********  数据上传成功  ********\r\n");
                    }
                    else
                    {
                        fmt.setForeground (Qt::red);    //设置前景色(字体色)
                        SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式
                        SerialDisplay[1]->appendPlainText("********  数据上传失败  ********\r\n");
                        fmt.setForeground (Qt::black);    //设置前景色(字体色)
                        SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    }
                    Serial2_Result.clear ();    //清空结果
                    //SerialDisplay[0]->clear (); //清空编辑区
                }
                else {

                    Serial2_ConfigNum++;

                }
                thisNum = 1;    //将失败次数变为1

            }
            else {

                fmt.setForeground (Qt::red);    //设置前景色(字体色)
                SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式
                SerialDisplay[1]->appendPlainText(QString("    返回:%1  异常")
                                                  .arg (ReturnMsg2.mid (0, ReturnMsg2.length ()-2)));          //返回值
                fmt.setForeground (Qt::black);    //设置前景色(字体色)
                SerialDisplay[1]->mergeCurrentCharFormat (fmt);    //使用当前格式

                //重复次数已够
                if(thisNum == needNum)
                {
                    thisNum = 1;
                    Serial2_ConfigNum = -1;

                    SerialResult[1]->setText ("失败");
                    SerialResult[1]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                    SerialDisplay[1]->appendPlainText("********  产品测试失败  ********\r\n");

                    test_Num ++; fail_Num++;
                    updateStatus ();    //刷新状态栏
                    Serial2_Result.clear();
                }
                else {
                    thisNum++;
                }

            }
        }

    }
}


/**********************************************************************
作者：莫秋兰
时间：2019年6月1日10:40:00
功能：RDA串口3写定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_WTimerdeal3()
{
    //获取系统当前时间
    LocalDateTime = QDateTime::currentDateTime();
    WriteDateTime = LocalDateTime.toString("yy-MM-dd hh:mm:ss");
    qDebug()<<"系统当前时间为："<<WriteDateTime;

    if(0 >= ConfigType.size())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial3_ConfigNum == -1) //发送TEST指令
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg("\r\n");
            while(1)
            {
                //每次发送之前都要清空返回值存储区
                RDAComReceive[2].clear();   /////////
                ReturnMsg3.clear();

                if(true == mySerial.RDAComWriteData(2,str))
                {
                    break;
                }
            }
            SerialResult[2]->setText ("等待");
            SerialResult[2]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            SerialDisplay[2]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
           // 开启数据读取定时器
           RDA_RTimer3.start(400);

            if(SerialDisplay[2]->blockCount () > 100)
            {
                SerialDisplay[2]->clear ();
            }
            //
        }
        else
        {
            qDebug()<<"Serial_ConfigNum = "<<Serial3_ConfigNum;
            if(ConfigSend.at(Serial3_ConfigNum) == "")
                Serial3_ConfigNum++;

            //每次写之前都先清空数组
            RDAComReceive[2].clear();
            ReturnMsg3.clear();

            if(mySerial.RDAComWriteData(2,ConfigSend.at(Serial3_ConfigNum)))
            {
                //SerialFlag = 1;

               qDebug()<<"发送的指令为："<<ConfigSend.at(Serial3_ConfigNum);

               SerialDisplay[2]->appendPlainText(QString("%1   发送%2    参考：%3%4")
                                                  .arg(WriteDateTime)                       //时间
                                                  .arg(ConfigSend.at(Serial3_ConfigNum))     //发送的指令
                                                  .arg(ConfigReturn.at(Serial3_ConfigNum))   //指令返回值
                                                  .arg(ConfigPara.at(Serial3_ConfigNum)));   //指令返回值参数


               /*开启数据读取定时器
               RDA_RTimer1.start(400);*/

    //           Serial_ConfigNum++;

            }else {

                qDebug()<<"指令发送失败！";
            }

        }
        //关闭定时器
       // timer.stop();
    }

}

/**********************************************************************
作者：莫秋兰
时间：2019年6月1日11:10:00
功能：RDA串口3读定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_RTimerdeal3()
{
    static int thisNum = 1; //初始化已重复次数为1
    int needNum = 1;    //失败重复读取次数


    //获取可失败的最大次数
    for(int i = 0; i < ConfigReturn.size(); i++)
    {
        if("重试次数" == ConfigReturn.at(i))
        {
            needNum = ConfigPara.at(i).toInt();
            break;
        }


    }

    if(Config_StartNum == Serial3_ConfigNum)
    {
        SerialResult[2]->setText ("测试");
        SerialResult[2]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    if(RDAComReceive[2] != "")
    {

        ReturnMsg3 = RDAComReceive[2];
        RDAComReceive[2].clear();
        qDebug()<<"返回数组中的值："<<ReturnMsg3;

        if(Serial3_ConfigNum == -1)
        {
            if(-1 != ReturnMsg3.indexOf("TEST_OK"))
            {
                Serial3_ConfigNum = Config_StartNum;

                SerialDisplay[2]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (ReturnMsg3.mid (0, ReturnMsg3.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[2]->clear (); //清空编辑区
                //disconnect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取
                return ;
            }
        }
        else
        {
            //进行返回值的判断
            if(1 == Return_Contrast(ReturnMsg3,Serial3_ConfigNum,Serial3_Result))
            {
                SerialDisplay[2]->appendPlainText(QString("    返回：%1   正常")
                                                   .arg(ReturnMsg3.mid(0,ReturnMsg3.length()-2)));

                //没有待测项
                if(Serial3_ConfigNum == ConfigType.size() - 1)
                {

                    Serial3_ConfigNum = -1; //指令重头发送
                    SerialResult[2]->setText ("成功");
                    SerialResult[2]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                    SerialDisplay[2]->appendPlainText("********  产品测试成功  ********\r\n");
                    test_Num ++; pass_Num++;
                    updateStatus ();    //刷新状态栏

                    if(uploadDataBase(Serial3_Result, updb_3))     //拼接字符串，上传数据库
                    {
                        SerialDisplay[2]->appendPlainText("********  数据上传成功  ********\r\n");
                    }
                    else
                    {
                        fmt.setForeground (Qt::red);    //设置前景色(字体色)
                        SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式
                        SerialDisplay[2]->appendPlainText("********  数据上传失败  ********\r\n");
                        fmt.setForeground (Qt::black);    //设置前景色(字体色)
                        SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    }
                    Serial3_Result.clear ();    //清空结果
                    //SerialDisplay[0]->clear (); //清空编辑区
                }
                else {

                    Serial3_ConfigNum++;

                }
                thisNum = 1;    //将失败次数变为1

            }
            else {

                fmt.setForeground (Qt::red);    //设置前景色(字体色)
                SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式
                SerialDisplay[2]->appendPlainText(QString("    返回:%1  异常")
                                                  .arg (ReturnMsg3.mid (0, ReturnMsg3.length ()-2)));          //返回值
                fmt.setForeground (Qt::black);    //设置前景色(字体色)
                SerialDisplay[2]->mergeCurrentCharFormat (fmt);    //使用当前格式

                //重复次数已够
                if(thisNum == needNum)
                {
                    thisNum = 1;
                    Serial3_ConfigNum = -1;

                    SerialResult[2]->setText ("失败");
                    SerialResult[2]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                    SerialDisplay[2]->appendPlainText("********  产品测试失败  ********\r\n");

                    test_Num ++; fail_Num++;
                    updateStatus ();    //刷新状态栏
                    Serial3_Result.clear();
                }
                else {
                    thisNum++;
                }

            }
        }

    }
}


/**********************************************************************
作者：莫秋兰
时间：2019年6月1日11:40:00
功能：RDA串口4写定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_WTimerdeal4()
{
    //获取系统当前时间
    LocalDateTime = QDateTime::currentDateTime();
    WriteDateTime = LocalDateTime.toString("yy-MM-dd hh:mm:ss");
    qDebug()<<"系统当前时间为："<<WriteDateTime;

    if(0 >= ConfigType.size())
    {
        QMessageBox::warning(this, tr("错误提示"), tr("请配置指令！！！"), tr("确 定"));
    }
    else
    {
        if(Serial4_ConfigNum == -1) //发送TEST指令
        {
            QString str = QString("%1%2").arg("AT^GT_CM=TEST").arg("\r\n");
            while(1)
            {
                //每次发送之前都要清空返回值存储区
                RDAComReceive[3].clear();   /////////
                ReturnMsg4.clear();

                if(true == mySerial.RDAComWriteData(3,str))
                {
                    break;
                }
            }
            SerialResult[3]->setText ("等待");
            SerialResult[3]->setStyleSheet ("color: rgb(0, 0, 0);");        //黑色字体

            SerialDisplay[3]->appendPlainText(QString("%1  发送%2  参考:%3%4")
                                              .arg (WriteDateTime)
                                              .arg ("AT^GT_CM=TEST")
                                              .arg ("TEST_OK")
                                              .arg (""));
           // 开启数据读取定时器
           RDA_RTimer4.start(400);

            if(SerialDisplay[3]->blockCount () > 100)
            {
                SerialDisplay[3]->clear ();
            }
            //
        }
        else
        {
            qDebug()<<"Serial_ConfigNum = "<<Serial4_ConfigNum;
            if(ConfigSend.at(Serial4_ConfigNum) == "")
                Serial4_ConfigNum++;

            //每次写之前都先清空数组
            RDAComReceive[3].clear();
            ReturnMsg4.clear();

            if(mySerial.RDAComWriteData(3,ConfigSend.at(Serial4_ConfigNum)))
            {
                //SerialFlag = 1;

               qDebug()<<"发送的指令为："<<ConfigSend.at(Serial4_ConfigNum);

               SerialDisplay[3]->appendPlainText(QString("%1   发送%2    参考：%3%4")
                                                  .arg(WriteDateTime)                       //时间
                                                  .arg(ConfigSend.at(Serial4_ConfigNum))     //发送的指令
                                                  .arg(ConfigReturn.at(Serial4_ConfigNum))   //指令返回值
                                                  .arg(ConfigPara.at(Serial4_ConfigNum)));   //指令返回值参数


               /*开启数据读取定时器
               RDA_RTimer1.start(400);*/

    //           Serial_ConfigNum++;

            }else {

                qDebug()<<"指令发送失败！";
            }

        }
        //关闭定时器
       // timer.stop();
    }

}

/**********************************************************************
作者：莫秋兰
时间：2019年6月1日14:00:00
功能：RDA串口4读定时器处理函数
参数1：
参数2：
***********************************************************************/
void MainWindow::RDA_RTimerdeal4()
{
    static int thisNum = 1; //初始化已重复次数为1
    int needNum = 1;    //失败重复读取次数


    //获取可失败的最大次数
    for(int i = 0; i < ConfigReturn.size(); i++)
    {
        if("重试次数" == ConfigReturn.at(i))
        {
            needNum = ConfigPara.at(i).toInt();
            break;
        }


    }

    if(Config_StartNum == Serial4_ConfigNum)
    {
        SerialResult[3]->setText ("测试");
        SerialResult[3]->setStyleSheet ("color: rgb(0, 0, 0);");       //黑色字体
    }

    if(RDAComReceive[3] != "")
    {

        ReturnMsg4 = RDAComReceive[3];
        RDAComReceive[3].clear();
        qDebug()<<"返回数组中的值："<<ReturnMsg4;

        if(Serial4_ConfigNum == -1)
        {
            if(-1 != ReturnMsg4.indexOf("TEST_OK"))
            {
                Serial4_ConfigNum = Config_StartNum;

                SerialDisplay[3]->appendPlainText(QString("    返回:%1  已连接")
                                                  .arg (ReturnMsg4.mid (0, ReturnMsg4.length ()-2))); //返回值  去掉\r\n
                SerialDisplay[3]->clear (); //清空编辑区
                //disconnect (&mySerial1, &QSerialPort::readyRead, this, &MainWindow::dealRead1);     //处理串口1数据读取
                return ;
            }
        }
        else
        {
            //进行返回值的判断
            if(1 == Return_Contrast(ReturnMsg4,Serial4_ConfigNum,Serial4_Result))
            {
                SerialDisplay[3]->appendPlainText(QString("    返回：%1   正常")
                                                   .arg(ReturnMsg4.mid(0,ReturnMsg4.length()-2)));

                //没有待测项
                if(Serial4_ConfigNum == ConfigType.size() - 1)
                {

                    Serial4_ConfigNum = -1; //指令重头发送
                    SerialResult[3]->setText ("成功");
                    SerialResult[3]->setStyleSheet ("color: rgb(0, 255, 0);");       //绿字
                    SerialDisplay[3]->appendPlainText("********  产品测试成功  ********\r\n");
                    test_Num ++; pass_Num++;
                    updateStatus ();    //刷新状态栏

                    if(uploadDataBase(Serial4_Result, updb_4))     //拼接字符串，上传数据库
                    {
                        SerialDisplay[3]->appendPlainText("********  数据上传成功  ********\r\n");
                    }
                    else
                    {
                        fmt.setForeground (Qt::red);    //设置前景色(字体色)
                        SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式
                        SerialDisplay[3]->appendPlainText("********  数据上传失败  ********\r\n");
                        fmt.setForeground (Qt::black);    //设置前景色(字体色)
                        SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式
                    }
                    Serial4_Result.clear ();    //清空结果
                    //SerialDisplay[0]->clear (); //清空编辑区
                }
                else {

                    Serial4_ConfigNum++;

                }
                thisNum = 1;    //将失败次数变为1

            }
            else {

                fmt.setForeground (Qt::red);    //设置前景色(字体色)
                SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式
                SerialDisplay[3]->appendPlainText(QString("    返回:%1  异常")
                                                  .arg (ReturnMsg4.mid (0, ReturnMsg4.length ()-2)));          //返回值
                fmt.setForeground (Qt::black);    //设置前景色(字体色)
                SerialDisplay[3]->mergeCurrentCharFormat (fmt);    //使用当前格式

                //重复次数已够
                if(thisNum == needNum)
                {
                    thisNum = 1;
                    Serial4_ConfigNum = -1;

                    SerialResult[3]->setText ("失败");
                    SerialResult[3]->setStyleSheet ("color: rgb(255, 0, 0);");       //红字
                    SerialDisplay[3]->appendPlainText("********  产品测试失败  ********\r\n");

                    test_Num ++; fail_Num++;
                    updateStatus ();    //刷新状态栏
                    Serial4_Result.clear();
                }
                else {
                    thisNum++;
                }

            }
        }

    }
}


//音量控制
void MainWindow::on_actionvoice_triggered()
{
    m_speech->show();
}
