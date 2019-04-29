#ifndef QSUBTHREAD_1_H
#define QSUBTHREAD_1_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <QString>
#include <QVector>
#include <qiterator.h>
//#include <QSerialPort>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QByteArray>
#include <QIODevice>
#include <QVector>

//bool flags ;//刘维球：当前共享变量

/*
*
* 第一个线程
*/
class QSubThread_1 : public QThread
{
    Q_OBJECT
public:
    QSubThread_1();
    void stopThread1() ;
    void startThread1() ;
    double average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum);
    double Current(int testNum);
    void run() Q_DECL_OVERRIDE;
    bool  PowerOn(double voltage);//设置测试电源的电压的恒定值

private:
    QString ResouceInstrName; // 程控电源串口地址
    QString RelayInstrName ;  // 继电器串口地址

    double CurrAver = 0 ; //等待电流的值
    bool testOk; //休眠电流的值

    bool waitCurrOk = false;//等待电流是否ok
    bool sleepCurrOk = false;//休眠电流是否ok
    bool chargeCurrOk = false;//充电电流是否ok

    bool flag1 = false; //开始线程

    QVector<double> testRange;
    QVector<int> testCalibNum;

    QString judgeCondtion;//判断条件，全部通过还是平均值通过
    double voltage;
    int process;//提示测试的是哪一个电流, 1表示待机，2表示休眠

private slots:
    void getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage );//传一个引用过来
    void getProcessNum(int process);
signals:
    void thread1_sg_on(bool TimeStart);//开始时钟
    void thread1_sg_1(double value, bool waitCurrOk,int Process);//等待电流测试结束发送一个信号
};



/*
*
* 第二个线程
*/
class QSubThread_2 : public QThread
{
    Q_OBJECT
public:
    QSubThread_2();
    void stopThread2() ;
    void startThread2() ;
    double average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum);
    double Current(int testNum);
    void run() Q_DECL_OVERRIDE;
    bool  PowerOn(double voltage);//设置测试电源的电压的恒定值

private:
    QString ResouceInstrName; // 程控电源串口地址
    QString RelayInstrName ;  // 继电器串口地址

    double CurrAver = 0 ; //等待电流的值
    bool testOk; //休眠电流的值

    bool waitCurrOk = false;//等待电流是否ok
    bool sleepCurrOk = false;//休眠电流是否ok
    bool chargeCurrOk = false;//充电电流是否ok

    bool flag1 = false; //开始线程

    QVector<double> testRange;
    QVector<int> testCalibNum;


    QString judgeCondtion;//判断条件，全部通过还是平均值通过
    double voltage;
    int process;//提示测试的是哪一个电流, 1表示待机，2表示休眠

private slots:
    void getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage );//传一个引用过来
    void getProcessNum(int process);
signals:
    void thread2_sg_on(bool TimeStart);//开始时钟
    void thread2_sg_2(double value, bool waitCurrOk,int Process);//等待电流测试结束发送一个信号
};



/*
*
* 第三个线程
*/

class QSubThread_3 : public QThread
{
    Q_OBJECT
public:
    QSubThread_3();
    void stopThread3() ;
    void startThread3() ;
    double average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum);
    double Current(int testNum);
    void run() Q_DECL_OVERRIDE;
    bool  PowerOn(double voltage);//设置测试电源的电压的恒定值

private:
    QString ResouceInstrName; // 程控电源串口地址
    QString RelayInstrName ;  // 继电器串口地址

    double CurrAver = 0 ; //等待电流的值
    bool testOk; //休眠电流的值

    bool waitCurrOk = false;//等待电流是否ok
    bool sleepCurrOk = false;//休眠电流是否ok
    bool chargeCurrOk = false;//充电电流是否ok

    bool flag1 = false; //开始线程

    QVector<double> testRange;
    QVector<int> testCalibNum;


    QString judgeCondtion;//判断条件，全部通过还是平均值通过
    double voltage;
    int process;//提示测试的是哪一个电流, 1表示待机，2表示休眠

private slots:
    void getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage );//传一个引用过来
    void getProcessNum(int process);
signals:
    void thread3_sg_on(bool TimeStart);//开始时钟
    void thread3_sg_3(double value, bool waitCurrOk,int Process);//等待电流测试结束发送一个信号
};


/*
 * 第四个线程
 *
**/

class QSubThread_4 : public QThread
{
    Q_OBJECT
public:
    QSubThread_4();
    void stopThread4() ;
    void startThread4() ;
    double average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum);
    double Current(int testNum);
    void run() Q_DECL_OVERRIDE;
    bool  PowerOn(double voltage);//设置测试电源的电压的恒定值

private:
    QString ResouceInstrName; // 程控电源串口地址
    QString RelayInstrName ;  // 继电器串口地址

    double CurrAver = 0 ; //等待电流的值
    bool testOk; //休眠电流的值

    bool waitCurrOk = false;//等待电流是否ok
    bool sleepCurrOk = false;//休眠电流是否ok
    bool chargeCurrOk = false;//充电电流是否ok

    bool flag1 = false; //开始线程

    QVector<double> testRange;
    QVector<int> testCalibNum;


    QString judgeCondtion;//判断条件，全部通过还是平均值通过
    double voltage;
    int process;//提示测试的是哪一个电流, 1表示待机，2表示休眠

private slots:
    void getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage );//传一个引用过来
    void getProcessNum(int process);
signals:
    void thread4_sg_on(bool TimeStart);//开始时钟
    void thread4_sg_4(double value, bool waitCurrOk,int Process);//等待电流测试结束发送一个信号
};



#endif // QSUBTHREAD_1_H
