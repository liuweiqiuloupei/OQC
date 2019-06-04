#ifndef OQCSERIAL_H
#define OQCSERIAL_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QComboBox>    //下拉列表
#include <QPushButton>

extern "C"  //莫秋兰
{
    #include <Windows.h>
}

#include <QLibrary> //莫秋兰 动态库加载 2019.6.1

extern QString RDAComReceive[4];   //莫秋兰 接收串口返回值数组
//extern bool ComInit[4];   //莫秋兰    RDA串口

static void RecvDataCallBack(int host_or_rs232,int commIndex,char * pContent);  //莫秋兰 读取串口返回值的回调函数


class OQCSerial
{
public:
    OQCSerial();
    ~OQCSerial();
    int SerachSerialPortName(QComboBox *SerialPName);      //梁家榕：查找可用串口
    bool OpenSerial(QSerialPort *mySerial, QComboBox *Name, QComboBox *Baud);    //梁家榕：打开串口
    void CloseSerial(QSerialPort *mySerial);    //梁家榕：关闭串口
    void ReadData(QSerialPort *mySerial, QByteArray &Readbuf1);     //梁家榕：数据读取
    qint64 WriteData(QSerialPort *mySerial, QByteArray &str);      //梁家榕：数据发送

     bool ComInit[4];   //莫秋兰    RDA串口
    bool RDAComWriteData(int HandleNum, QString PortData);  //莫秋兰 RDA数据发送
    bool RDADllInit();  //莫秋兰 加载动态库

    bool RDAComInit(int HandleNum, QString PortName);   //莫秋兰 RDA串口初始化
    bool CloseRDACom(int HandleNum);    //莫秋兰 //关闭串口

private:
    HINSTANCE myDll;    //莫秋兰 定义动态库对象
};

#endif // OQCSERIAL_H
