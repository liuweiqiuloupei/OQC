#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DataBase/database.h>
#include <QMainWindow>
#include <QTimer>
#include <Thread/qsubthread_1.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);//关闭窗口时间，可以询问是否退出

    ~MainWindow();

private slots:

    void on_actionsetting_triggered();

    
private:

    Ui::MainWindow *ui;

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

    QTimer timers[4]; //刘维球，定时器，每隔200ms（可设）发送一次指令,读一次结果

    Database *db;//刘维球，数据库,第一路连接，2019年4月29日15:07:14
    Database *db_1;//刘维球，数据库,第一路连接，2019年4月29日15:07:14
    Database *db_2;//刘维球，数据库,第二路连接，2019年4月29日15:07:14
    Database *db_3;//刘维球，数据库,第三路连接，2019年4月29日15:07:14
    Database *db_4;//刘维球，数据库,第四路连接，2019年4月29日15:07:14


    
};

#endif // MAINWINDOW_H
