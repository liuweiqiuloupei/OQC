#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DataBase/database.h>
#include <QMainWindow>
#include <QTimer>
#include <Thread/qsubthread_1.h>
#include <QHostAddress>     //主机地址
#include <QHostInfo>
#include <QNetworkInterface>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);//关闭窗口时间，可以询问是否退出

    QString getIPAddress(QString localHost);  //梁家榕，获取IP地址，2019年5月7日11:46:04

    void setUserName(QString username);//刘维球，获取登录名，2019年5月7日16:47:55


    ~MainWindow();

private slots:

    void on_actionsetting_triggered();

    
private:

    Ui::MainWindow *ui;

    QString UserName ;//刘维球：获取用户名，2019年5月7日15:34:57
    int UserGrade ;//刘维球：用户等级，2019年5月7日15:35:26

    QLabel *labOQC_Engineer ;
    QLabel* labtest_Num ;
    QLabel* labpass_Num ;
    QLabel* labfail_Num ;
    QLabel* labpercent_Num ;

    int test_Num = "0"; //刘维球：测试总数， 2019年5月7日09:30:35
    int pass_Num = "0" ; //刘维球：通过总数， 2019年5月7日09:30:44
    int fail_Num =  "0"; //刘维球：失败总数， 2019年5月7日09:31:26
    double pass_percent = "0.00%" ; //刘维球， 通过率， 2019年5月7日09:32:51

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
    
};

#endif // MAINWINDOW_H
