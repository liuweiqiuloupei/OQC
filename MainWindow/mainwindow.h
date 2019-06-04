#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DataBase/database.h>
#include <QMainWindow>
#include <QTimer>
#include <Thread/qsubthread_1.h>
#include <QHostAddress>     //主机地址
#include <QHostInfo>
#include <QNetworkInterface>

#include "DataBase/database.h"
#include "OQCSerial/oqcserial.h"    //串口
#include <QPlainTextEdit>
#include <QDateTime>    //日期时间
#include <QTextCharFormat>     //文本格式
//#include "Setting/checkconfigdialog.h"      //查看配置
#include "LogIn/qdlglogin.h"    //登录主界面
#include <QCompleter>       //自动补全

#include "Voice/voice.h"

//extern  bool ComInit[4];   //莫秋兰 RDA串口

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);//关闭窗口时间，可以询问是否退出

    QString getIPAddress(QString localHost);  //梁家榕，获取IP地址，2019年5月7日11:46:04

    void setUserName(QString username);//刘维球，获取登录名，2019年5月7日16:47:55

    void SetUserGrade(const int usergrade);     //梁家榕：设置账号权限
    void SoftWareShow();    //梁家榕：订单号显示
    void SerialFunc();      //梁家榕：串口功能模块
    bool uploadDataBase(QStringList &Result, Database &db); //梁家榕：上传数据库
    bool Return_Contrast(QString &thisStr, int ConfigNum, QStringList &thisResultList);  //梁家榕：串口返回值对比
    bool isEnglishAndDigital(QString &theStr);  //梁家榕：判断字符串是否为字母和数字组成
    int isDLTimes(const QString &theStr); //梁家榕：判断指令延时时间

    void add_ConfigDataTable(QStringList &TempType, QStringList &TempName,
                             QStringList &TempReturn, QStringList &TempPara,
                             QStringList &TempSend); //梁家榕：配置信息加入到数据表格
    void Proname_Load_All();        //梁家榕：全部订单加载
    void Proname_Load_Part(const QString &Str);   //梁家榕：部分订单加载，根据时间
    int setSerial_RDA_Flag();   //莫秋兰 设置串口通信标志位
public slots:
    void RDA_WTimerdeal1();
    void RDA_RTimerdeal1();
    void RDA_WTimerdeal2();
    void RDA_RTimerdeal2();
    void RDA_WTimerdeal3();
    void RDA_RTimerdeal3();
    void RDA_WTimerdeal4();
    void RDA_RTimerdeal4();
private slots:

    void on_actionsetting_triggered();

    void on_btn_Start_1_clicked();  //梁家榕：打开串口1
    void on_btn_Start_2_clicked();  //梁家榕：打开串口2
    void on_btn_Start_3_clicked();  //梁家榕：打开串口3
    void on_btn_Start_4_clicked();  //梁家榕：打开串口4

    void on_btn_Stop_1_clicked();   //梁家榕：关闭串口1
    void on_btn_Stop_2_clicked();   //梁家榕：关闭串口2
    void on_btn_Stop_3_clicked();   //梁家榕：关闭串口3
    void on_btn_Stop_4_clicked();   //梁家榕：关闭串口4

    void dealRead1();   //梁家榕：串口1读取
    void dealRead2();   //梁家榕：串口2读取
    void dealRead3();   //梁家榕：串口3读取
    void dealRead4();   //梁家榕：串口4读取

    void dealMyTimer1();    //梁家榕：串口1发送
    void dealMyTimer2();    //梁家榕：串口2发送
    void dealMyTimer3();    //梁家榕：串口3发送
    void dealMyTimer4();    //梁家榕：串口4发送

    void on_combB_Bps_activated(const QString &arg1);   //梁家榕：修改波特率
    void on_actionSwitch_triggered();   //梁家榕：切换账号
    void on_actionLock_triggered();     //梁家榕：解锁
    void on_actionStartAll_triggered(); //梁家榕：全部开始测试

    void on_btnok_clicked();    //梁家榕：确定配置
    void on_btncancel_clicked();//梁家榕：取消配置
    void on_comboBox_Proname_activated(const QString &arg1);    //梁家榕：选择订单
    void on_radioButton_clicked(bool checked);  //梁家榕：选择订单查询方式：精准、模糊
    void on_pushButton_clicked();   //精准查询
    void on_comboBox_SelectTime_activated(const QString &arg1); //梁家榕：选择订单查询时间
    void on_actionReSerial_triggered(); //梁家榕：重新获取串口
    void dealMyTimeOut();   //梁家榕：超时处理
    void on_actionvoice_triggered();

private:

    Ui::MainWindow *ui;
    QString status = " ";
    QString UserName ;//刘维球：获取用户名，2019年5月7日15:34:57
    int UserGrade ;//刘维球：用户等级，2019年5月7日15:35:26

    QLabel* labOQC_Engineer ;
    QLabel* labtest_Num ;
    QLabel* labpass_Num ;
    QLabel* labfail_Num ;
    QLabel* labpercent_Num ;

    int test_Num = 0; //刘维球：测试总数， 2019年5月7日09:30:35
    int pass_Num = 0; //刘维球：通过总数， 2019年5月7日09:30:44
    int fail_Num = 0; //刘维球：失败总数， 2019年5月7日09:31:26
    double pass_percent ; //刘维球， 通过率， 2019年5月7日09:32:51

    QString localHost ; //刘维球：计算机名，2019年5月7日10:32:02
    QString IPAddress ; //刘维球：计算机名，2019年5月7日10:32:02
    QString OQC_Engineer = ""; //刘维球：OQC工程师，2019年5月7日10:33:29

    QSubThread_1 *subthread_1;//刘维球，创建子线程，2019年4月29日14:53:25
    QSubThread_2 *subthread_2;//刘维球，创建子线程，2019年4月29日14:53:25
    QSubThread_3 *subthread_3;//刘维球，创建子线程，2019年4月29日14:53:25
    QSubThread_4 *subthread_4;//刘维球，创建子线程，2019年4月29日14:53:25

    QSerialPort* serialPort_1;//刘维球，创建串口，2019年4月29日14:55:46
    QSerialPort* serialPort_2;//刘维球，创建串口，2019年4月29日14:55:46
    QSerialPort* serialPort_3;//刘维球，创建串口，2019年4月29日14:55:46
    QSerialPort* serialPort_4;//刘维球，创建串口，2019年4月29日14:55:46

    QByteArray replay_1;//刘维球，第一路串口的回读值；2019年4月29日15:02:29
    QByteArray replay_2;//刘维球，第二路串口的回读值；2019年4月29日15:02:29
    QByteArray replay_3;//刘维球，第三路串口的回读值；2019年4月29日15:02:29
    QByteArray replay_4;//刘维球，第四路串口的回读值；2019年4月29日15:02:29

    QByteArray sendCMD_1 ;//刘维球，一路发送的命令，2019年4月29日15:03:48
    QByteArray sendCMD_2 ;//刘维球，一路发送的命令，2019年4月29日15:03:48
    QByteArray sendCMD_3 ;//刘维球，一路发送的命令，2019年4月29日15:03:48
    QByteArray sendCMD_4 ;//刘维球，一路发送的命令，2019年4月29日15:03:48

    QTimer timers[4]; //刘维球，定时器，每隔200ms（可设）发送一次指令,读一次结果，2019年5月7日09:33:05

    Database *db;//刘维球，数据库,第一路连接，2019年4月29日15:07:14
    Database *db_1;//刘维球，数据库,第一路连接，2019年4月29日15:07:14
    Database *db_2;//刘维球，数据库,第二路连接，2019年4月29日15:07:14
    Database *db_3;//刘维球，数据库,第三路连接，2019年4月29日15:07:14
    Database *db_4;//刘维球，数据库,第四路连接，2019年4月29日15:07:14


    void updateStatus();//刘维球，更新状态栏，2019年5月7日17:15:13


    OQCSerial mySerial;             //梁家榕：串口操作对象

    QSerialPort mySerial1;         //梁家榕：定义串口1对象
    QSerialPort mySerial2;         //梁家榕：定义串口2对象
    QSerialPort mySerial3;         //梁家榕：定义串口3对象
    QSerialPort mySerial4;         //梁家榕：定义串口4对象

    Database updb_1;    //梁家榕：串口1数据库：上传时使用
    Database updb_2;    //梁家榕：串口2数据库：上传时使用
    Database updb_3;    //梁家榕：串口3数据库：上传时使用
    Database updb_4;    //梁家榕：串口4数据库：上传时使用
    Database db_Config; //梁家榕：数据库操作对象：配置时使用

    QTimer myTimer1;        //梁家榕：串口1定时器对象
    QTimer myTimer2;        //梁家榕：串口2定时器对象
    QTimer myTimer3;        //梁家榕：串口1定时器对象
    QTimer myTimer4;        //梁家榕：串口2定时器对象
    QTimer myTimerOut;      //梁家榕：超时处理

    QComboBox *SerialName[4];           //梁家榕：串口名控件数组
    //QComboBox *SerialBaud[4];           //梁家榕：串口波特率控件数组
    //QPushButton *SerialStart[4];        //梁家榕：串口通道数组
    QPlainTextEdit *SerialDisplay[4];   //梁家榕：串口读取显示控件
    QLabel *SerialBaudShow[4];          //梁家榕：测试结果显示控件数组
    QLabel *SerialResult[4];            //梁家榕：测试结果显示控件数组
    QLabel *SoftWareNameShow;           //梁家榕：订单号显示

    int myTimes = 500;  //梁家榕：定时器间隔时间
    int Serial1_ConfigNum = 0;  //梁家榕：串口1配置指令数组下标
    int Serial2_ConfigNum = 0;  //梁家榕：串口2配置指令数组下标
    int Serial3_ConfigNum = 0;  //梁家榕：串口3配置指令数组下标
    int Serial4_ConfigNum = 0;  //梁家榕：串口4配置指令数组下标
    int Config_StartNum = 0;    //梁家榕：配置指令开始发送下标

    QByteArray Readbuf1;        //梁家榕：串口1字节读取数组
    QByteArray Readbuf2;        //梁家榕：串口2字节读取数组
    QByteArray Readbuf3;        //梁家榕：串口3字节读取数组
    QByteArray Readbuf4;        //梁家榕：串口4字节读取数组

    QStringList ConfigType;         //梁家榕：指令类型
    QStringList ConfigName;         //梁家榕：指令名字
    QStringList ConfigSend;         //梁家榕：发送指令
    QStringList ConfigPara;         //梁家榕：指令参数
    QStringList ConfigReturn;       //梁家榕：指令返回值

    QStringList Configlist;     //梁家榕：配置表链接信息
    QStringList db_ResultList;      //梁家榕：结果表链接数据库信息

    QStringList Serial1_Result; //梁家榕：串口1测试结果
    QStringList Serial2_Result; //梁家榕：串口2测试结果
    QStringList Serial3_Result; //梁家榕：串口3测试结果
    QStringList Serial4_Result; //梁家榕：串口4测试结果

    QString WriteDateTime;  //梁家榕：发送的日期时间
    QString SelectDateTime; //梁家榕：查询的日期时间
    QString SoftWareName;   //梁家榕：订单号
    QString MachineName;    //梁家榕：机型
    QString Station;        //梁家榕：所有配置信息
    QString ChooseTime;     //梁家榕：选择的查询时间

    QDateTime LocalDateTime;    //梁家榕：本地时间对象
    QTextCharFormat fmt;        //梁家榕：字体格式对象
    QCompleter *Completer;      //梁家榕：自动补全对象

    int Serial_RDA_Flag;        //莫秋兰 串口通信方式选择标志位 0 -- 普通 1 -- RDA

    QTimer RDA_WTimer1;         //莫秋兰 RDA串口1写数据定时器对象
    QTimer RDA_WTimer2;         //莫秋兰 RDA串口2写数据定时器对象
    QTimer RDA_WTimer3;         //莫秋兰 RDA串口3写数据定时器对象
    QTimer RDA_WTimer4;         //莫秋兰 RDA串口4写数据定时器对象

    QTimer RDA_RTimer1;         //莫秋兰 RDA串口1读数据定时器对象
    QTimer RDA_RTimer2;         //莫秋兰 RDA串口2读数据定时器对象
    QTimer RDA_RTimer3;         //莫秋兰 RDA串口3读数据定时器对象
    QTimer RDA_RTimer4;         //莫秋兰 RDA串口4读数据定时器对象

    QString ReturnMsg1;          //莫秋兰 RDA串口读取回读值暂存区
    QString ReturnMsg2;
    QString ReturnMsg3;
    QString ReturnMsg4;

    voice *m_speech;  //莫秋兰，语音播放设置（显示调节框），2019年5月8日19：28:20
    voice *m_speech_1;  //莫秋兰，语音播放，第一路，2019年5月8日19：28:20
    voice *m_speech_2;  //莫秋兰，语音播放，第二路，2019年5月8日19：28:20
    voice *m_speech_3;  //莫秋兰，语音播放，第三路，2019年5月8日19：28:20
    voice *m_speech_4;  //莫秋兰，语音播放，第四路，2019年5月8日19：28:20

};

#endif // MAINWINDOW_H
