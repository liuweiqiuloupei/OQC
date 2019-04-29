#include "mainwindow.h"
#include "qsubthread_1.h"
#include "IT6400.h"
#include "Keysight34461A.h"
#include "Relay32.h"

#include <QDebug>
#include <QVector>
#include <QByteArray>

#pragma comment(lib,"IT6400.lib")
#pragma comment(lib,"Keysight34461A.lib")
#pragma comment(lib,"Relay32.lib")


QMutex  mutex;//两个线程共享一个变量
QWaitCondition  newdataAvailable;//唤醒条件

/**
子线程1
*/

QSubThread_1::QSubThread_1()
{

}

void QSubThread_1::run()
{

    qDebug() <<"run函数：" << endl;

    PowerOn(voltage);//上电

    QByteArray RelayInstrName_Byte = RelayInstrName.toLocal8Bit() ;

    char* InstrName = RelayInstrName_Byte.data();

    flag1=false;//启动线程时令m_stop=false，

    waitCurrOk= false;
    sleepCurrOk = false;

    int32_t RelayNum1 = 1;//继电器1
    int32_t RelayNum2 = 2;//继电器2
    const  LVBoolean on = true;
    const  LVBoolean off = false;

    LVBoolean relay1 = false; //继电器指令1
    LVBoolean* relay1_Pt = &relay1;
    LVBoolean serial1 = false;
    LVBoolean* serial1_Pt = &serial1;

    LVBoolean relay2 = false; //继电器指令2
    LVBoolean* relay2_Pt = &relay2;
    LVBoolean serial2 = false;
    LVBoolean* serial2_Pt = &serial2;


    QThread::msleep(1000*testCalibNum.at((process-1)*3+2));//等待15秒钟再去测试

    mutex.lock();

    CmdSet(InstrName,RelayNum2,on,relay2_Pt,serial2_Pt);//测试电源继电器闭合
    CmdSet(InstrName,RelayNum1,on,relay1_Pt,serial1_Pt);//备用电源继电器断开
    relay1 = false;
    relay2 = false;

    for(unsigned int i=0; i<testCalibNum.at((process-1)*3+1); i++) //测试的次数
    {
        //锁定仪器
        CurrAver = Current(testCalibNum.at((process-1)*3));
        if(CurrAver>= testRange.at((process-1)*2) && CurrAver <= testRange.at((process-1)*2+1))
        {
            testOk = true;
            break;
        }
    }

    CmdSet(InstrName,RelayNum2,off,relay2_Pt,serial2_Pt);//测试电源继电器断开
    //QThread::msleep(1000);
    CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合

    for(int i=0; i<3; i++)
    {
        if( !relay1)
        {
            CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合
        }
        if( relay1 )
        {
            relay1 = false;
            break;
        }
    }

    qInfo() << "第一路仪表电流平均值：" + QString::number(CurrAver,'f') + "成功：" + QString::number(testOk) + "第" + QString::number(process,10) + "项目"<<endl;
    emit thread1_sg_1(CurrAver, testOk, process);//将waitCurrAver的值和测试是否成功与主线程信号与槽绑定
    newdataAvailable.wakeAll();
    mutex.unlock();
    QThread::msleep(1000);
    emit thread1_sg_on(true);//开始启动定时器
    quit();
}

//上电
bool QSubThread_1::PowerOn(double voltage)
{//上电

    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();

    bool OutputEnable = false;
    uintptr_t* VisaNameOut = 0;//串口号

    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid = &DefaSetInit;//默认初始化

    LVBoolean VSetInit = false;
    LVBoolean *VsetValid = &VSetInit;//设置电压

    double VLimit = 5.0;// 不能超过4V
    LVBoolean EnVLimitSet = true;
    LVBoolean VLimitSetInit = false;
    LVBoolean *VLimitSetValid = &VLimitSetInit;//电压上限

    LVBoolean EnableOutput = true;
    LVBoolean OutputInit = false;
    LVBoolean *OutputSetValid = &OutputInit;//设置输出
    OutputEnable = (bool)OutputInit;

    Defaultset(InstrName, VisaNameOut, DefaSetValid);
    VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
    VSet(InstrName, voltage, VsetValid);

    EnOutput(InstrName, EnableOutput, OutputSetValid);
    return OutputInit;
}

//求平均值
double QSubThread_1::average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum)
{
    double currAverage;
    double sum = 0;
    while (begin != end)
    {
        sum = sum + (*begin);
        begin++;
    }
    currAverage = sum / elementsNum;
    return currAverage;
} 

//测平均电流
double QSubThread_1::Current(int testNum)
{

    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();

    uintptr_t* VisaNameOut = 0;//串口号
    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid= &DefaSetInit;//默认初始化
    Defaultset(InstrName, VisaNameOut, DefaSetValid);

    //qDebug() << "进入测试的函数中：" << endl;

    double CInit = 0;
    double *CMeasValue = &CInit;
    LVBoolean CMeasInit = false;
    LVBoolean *CMeasValid= &CMeasInit;//电流测量值


    QVector<double> cur_Arr; //存放测试电流的值的容器
    double curr_Average = 0;  //存放等待模式下电流的平均

    // ConfMeas(InstrName,0.001,VisaNameOut) ;//测试结束之前，将量程调小
    for(int i=0; i<testNum; i++)
    {
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        CMeas(InstrName,CMeasValue,CMeasValid );
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        if(CMeasInit)
        {
            cur_Arr.push_back(CInit);
        }
    }
    ConfMeas(InstrName,3,VisaNameOut); //测试结束之后，将量程调大
    curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());
    return curr_Average;
}

//获取测试的各个参数
void QSubThread_1::getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage)
{

    this->ResouceInstrName = ResouceInstrName;
    //qDebug() << "ResouceInstrname:" << this->InstrName << endl;
    this->RelayInstrName = RelayInstrName;
    qDebug() << "RelayInstrName:" <<  this->RelayInstrName << endl;
    this->testRange = testRange;
    //
    this->testCalibNum = testCalibNum;
    this->voltage = voltage;

}

//在测试之前获取测试哪个步骤，
void QSubThread_1::getProcessNum(int process)
{
    this->process = process;
}

//停止测试
void QSubThread_1::stopThread1()
{
    QMutexLocker locker(&mutex); //必须是锁
    flag1 = true;
}

/**
子线程2
*/
QSubThread_2::QSubThread_2()
{

}
void QSubThread_2::run()
{
    //serialPort1 = new QSerialPort;

    //qDebug()<< "run函数:" <<endl;


    PowerOn(voltage);//
    //  QThread::msleep(15000);//等待15秒钟再去测试
    flag1=false;//启动线程时令m_stop=false，

    waitCurrOk= false;
    sleepCurrOk = false;

    QByteArray RelayInstrName_Byte = RelayInstrName.toLocal8Bit() ;

    char* InstrName = RelayInstrName_Byte.data();

    int32_t RelayNum1 = 3;//继电器1
    int32_t RelayNum2 = 4;//继电器2
    const  LVBoolean on = true;
    const  LVBoolean off = false;

    LVBoolean relay1 = false; //继电器指令1
    LVBoolean* relay1_Pt = &relay1;
    LVBoolean serial1 = false;
    LVBoolean* serial1_Pt = &serial1;

    LVBoolean relay2 = false; //继电器指令2
    LVBoolean* relay2_Pt = &relay2;
    LVBoolean serial2 = false;
    LVBoolean* serial2_Pt = &serial2;


    QThread::msleep(testCalibNum.at((process-1)*3+2));//等待15秒钟再去测试


    mutex.lock();

    CmdSet(InstrName,RelayNum2,on,relay2_Pt,serial2_Pt);//测试电源继电器闭合
    CmdSet(InstrName,RelayNum1,on,relay1_Pt,serial1_Pt);//备用电源继电器断开
    relay1 = false;
    relay2 = false;


    for(unsigned int i=0; i<testCalibNum.at((process-1)*3+1); i++) //测试的次数
    {
        //锁定仪器
        CurrAver = Current(testCalibNum.at((process-1)*3));
        if(CurrAver>= testRange.at((process-1)*2) && CurrAver <= (process-1)*2+1)
        {
            testOk = true;
            break;
        }
    }

    CmdSet(InstrName,RelayNum2,off,relay2_Pt,serial2_Pt);//测试电源继电器断开

    CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合

    for(int i=0; i<3; i++)
    {
        if( !relay1)
        {
            CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合
        }
        if( relay1 )
        {
            relay1 = false;
            break;
        }
    }

    qInfo() << "第二路仪表电流平均值：" + QString::number(CurrAver,'f') + "成功：" + QString::number(testOk) + "第" + QString::number(process,10) + "项目"<<endl;
    emit thread2_sg_2(CurrAver, testOk, process);//将waitCurrAver的值和测试是否成功与主线程信号与槽绑定
    newdataAvailable.wakeAll();
    mutex.unlock();
    QThread::msleep(3000);
    emit thread2_sg_on(true);//开始启动定时器
    quit();
}

bool QSubThread_2::PowerOn(double voltage)
{//上电


    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();
    bool OutputEnable = false;
    uintptr_t* VisaNameOut = 0;//串口号

    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid = &DefaSetInit;//默认初始化

    LVBoolean VSetInit = false;
    LVBoolean *VsetValid = &VSetInit;//设置电压

    double VLimit = 5.0;// 不能超过4V
    LVBoolean EnVLimitSet = true;
    LVBoolean VLimitSetInit = false;
    LVBoolean *VLimitSetValid = &VLimitSetInit;//电压上限

    LVBoolean EnableOutput = true;
    LVBoolean OutputInit = false;
    LVBoolean *OutputSetValid = &OutputInit;//设置输出
    OutputEnable = (bool)OutputInit;

    Defaultset(InstrName, VisaNameOut, DefaSetValid);
    VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
    VSet(InstrName, voltage, VsetValid);

    EnOutput(InstrName, EnableOutput, OutputSetValid);
    return OutputInit;
}


double QSubThread_2::average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum)
{
    double currAverage;
    double sum = 0;
    while (begin != end)
    {
        sum = sum + (*begin);
        begin++;
    }
    currAverage = sum / elementsNum;
    return currAverage;
}


double QSubThread_2::Current(int testNum)
{

    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();
    uintptr_t* VisaNameOut = 0;//串口号
    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid= &DefaSetInit;//默认初始化
    Defaultset(InstrName, VisaNameOut, DefaSetValid);

    //qDebug() << "进入测试的函数中：" << endl;

    double CInit = 0;
    double *CMeasValue = &CInit;
    LVBoolean CMeasInit = false;
    LVBoolean *CMeasValid= &CMeasInit;//电流测量值


    QVector<double> cur_Arr; //存放测试电流的值的容器
    double curr_Average = 0;  //存放等待模式下电流的平均

    ConfMeas(InstrName,0.001,VisaNameOut) ;//测试结束之前，将量程调小
    for(int i=0; i<testNum; i++)
    {
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        CMeas(InstrName,CMeasValue,CMeasValid );
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        if(CMeasInit)
        {
            cur_Arr.push_back(CInit);
        }
    }
    ConfMeas(InstrName,3,VisaNameOut); //测试结束之后，将量程调大
    curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());
    return curr_Average;
}

void QSubThread_2::getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage)
{
    this->voltage = voltage;
    this->ResouceInstrName = ResouceInstrName;
    this->RelayInstrName = RelayInstrName;
    this->testRange = testRange;
    this->testCalibNum = testCalibNum;

}

//在测试之前获取测试哪个步骤，
void QSubThread_2::getProcessNum(int process)
{
    this->process = process;
}

void QSubThread_2::stopThread2()
{
    QMutexLocker locker(&mutex); //必须是锁
    flag1 = true;
}

/**
 子线程3
*/

QSubThread_3::QSubThread_3()
{

}
void QSubThread_3::run()
{
    //serialPort1 = new QSerialPort;

    //qDebug()<< "run函数:" <<endl;

    PowerOn(voltage);//
    //  QThread::msleep(15000);//等待15秒钟再去测试
    flag1=false;//启动线程时令m_stop=false，

    waitCurrOk= false;
    sleepCurrOk = false;

    QByteArray RelayInstrName_Byte = RelayInstrName.toLocal8Bit() ;

    char* InstrName = RelayInstrName_Byte.data();

    int32_t RelayNum1 = 5;//继电器1
    int32_t RelayNum2 = 6;//继电器2
    const  LVBoolean on = true;
    const  LVBoolean off = false;

    LVBoolean relay1 = false; //继电器指令1
    LVBoolean* relay1_Pt = &relay1;
    LVBoolean serial1 = false;
    LVBoolean* serial1_Pt = &serial1;

    LVBoolean relay2 = false; //继电器指令2
    LVBoolean* relay2_Pt = &relay2;
    LVBoolean serial2 = false;
    LVBoolean* serial2_Pt = &serial2;


    QThread::msleep(testCalibNum.at((process-1)*3+2));//等待15秒钟再去测试


    mutex.lock();

    CmdSet(InstrName,RelayNum2,on,relay2_Pt,serial2_Pt);//测试电源继电器闭合
    CmdSet(InstrName,RelayNum1,on,relay1_Pt,serial1_Pt);//备用电源继电器断开
    relay1 = false;
    relay2 = false;

    for(unsigned int i=0; i<testCalibNum.at((process-1)*3+1); i++) //测试的次数
    {
        //锁定仪器
        CurrAver = Current(testCalibNum.at((process-1)*3));
        if(CurrAver>= testRange.at((process-1)*2) && CurrAver <= (process-1)*2+1)
        {
            testOk = true;
            break;
        }
    }

    CmdSet(InstrName,RelayNum2,off,relay2_Pt,serial2_Pt);//测试电源继电器断开
    CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合
    for(int i=0; i<3; i++)
    {
        if( !relay1)
        {
            CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合
        }
        if( relay1 )
        {
            relay1 = false;
            break;
        }
    }

    qInfo() << "第三路仪表电流平均值：" + QString::number(CurrAver,'f') + "成功：" + QString::number(testOk) + "第" + QString::number(process,10) + "项目"<<endl;
    emit thread3_sg_3(CurrAver, testOk, process);//将waitCurrAver的值和测试是否成功与主线程信号与槽绑定
    newdataAvailable.wakeAll();
    mutex.unlock();
    QThread::msleep(3000);
    emit thread3_sg_on(true);//开始启动定时器
    quit();
}

bool QSubThread_3::PowerOn(double voltage)
{//上电

    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();

    bool OutputEnable = false;
    uintptr_t* VisaNameOut = 0;//串口号

    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid = &DefaSetInit;//默认初始化

    LVBoolean VSetInit = false;
    LVBoolean *VsetValid = &VSetInit;//设置电压

    double VLimit = 5.0;// 不能超过4V
    LVBoolean EnVLimitSet = true;
    LVBoolean VLimitSetInit = false;
    LVBoolean *VLimitSetValid = &VLimitSetInit;//电压上限

    LVBoolean EnableOutput = true;
    LVBoolean OutputInit = false;
    LVBoolean *OutputSetValid = &OutputInit;//设置输出
    OutputEnable = (bool)OutputInit;

    Defaultset(InstrName, VisaNameOut, DefaSetValid);
    VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
    VSet(InstrName, voltage, VsetValid);

    EnOutput(InstrName, EnableOutput, OutputSetValid);
    return OutputInit;
}


double QSubThread_3::average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum)
{
    double currAverage;
    double sum = 0;
    while (begin != end)
    {
        sum = sum + (*begin);
        begin++;
    }
    currAverage = sum / elementsNum;
    return currAverage;
}


double QSubThread_3::Current(int testNum)
{
    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();
    uintptr_t* VisaNameOut = 0;//串口号
    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid= &DefaSetInit;//默认初始化
    Defaultset(InstrName, VisaNameOut, DefaSetValid);

    //qDebug() << "进入测试的函数中：" << endl;

    double CInit = 0;
    double *CMeasValue = &CInit;
    LVBoolean CMeasInit = false;
    LVBoolean *CMeasValid= &CMeasInit;//电流测量值


    QVector<double> cur_Arr; //存放测试电流的值的容器
    double curr_Average = 0;  //存放等待模式下电流的平均

    //ConfMeas(InstrName,0.001,VisaNameOut) ;//测试结束之前，将量程调小
    for(int i=0; i<testNum; i++)
    {
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        CMeas(InstrName,CMeasValue,CMeasValid );
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        if(CMeasInit)
        {
            cur_Arr.push_back(CInit);
        }
    }
    ConfMeas(InstrName,3,VisaNameOut); //测试结束之后，将量程调大
    curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());
    return curr_Average;
}

void QSubThread_3::getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage)
{
    this->voltage = voltage;
    this->ResouceInstrName = ResouceInstrName;
    this->RelayInstrName = RelayInstrName;
    this->testRange = testRange;
    this->testCalibNum = testCalibNum;

}

//在测试之前获取测试哪个步骤，
void QSubThread_3::getProcessNum(int process)
{
    this->process = process;
}

void QSubThread_3::stopThread3()
{
    QMutexLocker locker(&mutex); //必须是锁
    flag1 = true;
}

/**
    子线程4
*/

QSubThread_4::QSubThread_4()
{

}
void QSubThread_4::run()
{
    //serialPort1 = new QSerialPort;

    //qDebug()<< "run函数:" <<endl;


    PowerOn(voltage);//
    //  QThread::msleep(15000);//等待15秒钟再去测试
    flag1=false;//启动线程时令m_stop=false，

    QByteArray RelayInstrName_Byte = RelayInstrName.toLocal8Bit() ;

    char* InstrName = RelayInstrName_Byte.data();
    waitCurrOk= false;
    sleepCurrOk = false;


    int32_t RelayNum1 = 7;//继电器1
    int32_t RelayNum2 = 8;//继电器2
    const  LVBoolean on = true;
    const  LVBoolean off = false;

    LVBoolean relay1 = false; //继电器指令1
    LVBoolean* relay1_Pt = &relay1;
    LVBoolean serial1 = false;
    LVBoolean* serial1_Pt = &serial1;

    LVBoolean relay2 = false; //继电器指令2
    LVBoolean* relay2_Pt = &relay2;
    LVBoolean serial2 = false;
    LVBoolean* serial2_Pt = &serial2;


    QThread::msleep(testCalibNum.at((process-1)*3+2));//等待15秒钟再去测试


    mutex.lock();

    CmdSet(InstrName,RelayNum2,on,relay2_Pt,serial2_Pt);//测试电源继电器闭合
    CmdSet(InstrName,RelayNum1,on,relay1_Pt,serial1_Pt);//备用电源继电器断开
    relay1 = false;
    relay2 = false;
    for(unsigned int i=0; i<testCalibNum.at((process-1)*3+1); i++) //测试的次数
    {
        //锁定仪器
        CurrAver = Current(testCalibNum.at((process-1)*3));
        if(CurrAver>= testRange.at((process-1)*2) && CurrAver <= (process-1)*2+1)
        {
            testOk = true;
            break;
        }
    }

    CmdSet(InstrName,RelayNum2,off,relay2_Pt,serial2_Pt);//测试电源继电器断开
    CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合

    for(int i=0; i<3; i++)
    {
        if( !relay1)
        {
            CmdSet(InstrName,RelayNum1,off,relay1_Pt,serial1_Pt);//备用电源继电器闭合
        }
        if( relay1 )
        {
            relay1 = false;
            break;
        }
    }

    qInfo() << "第四路仪表电流平均值：" + QString::number(CurrAver,'f') + "成功：" + QString::number(testOk) + "第" + QString::number(process,10) + "项目"<<endl;

    emit thread4_sg_4(CurrAver, testOk, process);//将waitCurrAver的值和测试是否成功与主线程信号与槽绑定
    newdataAvailable.wakeAll();
    mutex.unlock();
    QThread::msleep(3000);
    emit thread4_sg_on(true);//开始启动定时器
    quit();
}

bool QSubThread_4::PowerOn(double voltage)
{//上电
    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();

    bool OutputEnable = false;
    uintptr_t* VisaNameOut = 0;//串口号

    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid = &DefaSetInit;//默认初始化

    LVBoolean VSetInit = false;
    LVBoolean *VsetValid = &VSetInit;//设置电压

    double VLimit = 5.0;// 不能超过4V
    LVBoolean EnVLimitSet = true;
    LVBoolean VLimitSetInit = false;
    LVBoolean *VLimitSetValid = &VLimitSetInit;//电压上限

    LVBoolean EnableOutput = true;
    LVBoolean OutputInit = false;
    LVBoolean *OutputSetValid = &OutputInit;//设置输出
    OutputEnable = (bool)OutputInit;

    Defaultset(InstrName, VisaNameOut, DefaSetValid);
    VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
    VSet(InstrName, voltage, VsetValid);

    EnOutput(InstrName, EnableOutput, OutputSetValid);
    return OutputInit;
}


double QSubThread_4::average(QVector<double>::const_iterator begin, QVector<double>::const_iterator end, QVector<double>::size_type elementsNum)
{
    double currAverage;
    double sum = 0;
    while (begin != end)
    {
        sum = sum + (*begin);
        begin++;
    }
    currAverage = sum / elementsNum;
    return currAverage;
}


double QSubThread_4::Current(int testNum)
{
    QByteArray ResourceInstrName_Byte = ResouceInstrName.toLocal8Bit() ;
    char* InstrName = ResourceInstrName_Byte.data();
    uintptr_t* VisaNameOut = 0;//串口号
    LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid= &DefaSetInit;//默认初始化
    Defaultset(InstrName, VisaNameOut, DefaSetValid);

    //qDebug() << "进入测试的函数中：" << endl;

    double CInit = 0;
    double *CMeasValue = &CInit;
    LVBoolean CMeasInit = false;
    LVBoolean *CMeasValid= &CMeasInit;//电流测量值


    QVector<double> cur_Arr; //存放测试电流的值的容器
    double curr_Average = 0;  //存放等待模式下电流的平均

    //ConfMeas(InstrName,0.001,VisaNameOut) ;//测试结束之前，将量程调小
    for(int i=0; i<testNum; i++)
    {
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        CMeas(InstrName,CMeasValue,CMeasValid );
        ConfMeas(InstrName,0.001,VisaNameOut) ; //测试结束之前，将量程调小
        // qDebug() << "CInit: " << CInit << endl;
        if(CMeasInit)
        {
            cur_Arr.push_back(CInit);
        }
    }
    ConfMeas(InstrName,3,VisaNameOut); //测试结束之后，将量程调大
    curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());
    return curr_Average;
}

void QSubThread_4::getPortName(QString ResouceInstrName,QString RelayInstrName,QVector<double> testRange, QVector<int> testCalibNum, double voltage)
{
    this->voltage = voltage;
    this->ResouceInstrName = ResouceInstrName;
    this->RelayInstrName = RelayInstrName;
    this->testRange = testRange;
    this->testCalibNum = testCalibNum;

}

//在测试之前获取测试哪个步骤，
void QSubThread_4::getProcessNum(int process)
{
    this->process = process;
}

void QSubThread_4::stopThread4()
{
    QMutexLocker locker(&mutex); //必须是锁
    flag1 = true;
}


