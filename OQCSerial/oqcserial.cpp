#include "oqcserial.h"
#include <QDebug>

QString RDAComReceive[4];  //莫秋兰 存放RDA串口返回值的数组

typedef void *DLL_CALLBACK(int host_or_rs232,int commIndex,char *Content);  //莫秋兰 RDA串口读取回调函数
bool (*DLLAutoTestInit)(DLL_CALLBACK cb);
bool (*HostComPortInit)(int commIndex,char *PortName,char *pRS232PortName);
bool (*HostComPortShutdown)(int commIndex);
int (*HostComPortWriteData)(int commIndex,char *Data);
bool (*HostComPortcReconnect)(int commIndex);



OQCSerial::OQCSerial()
{


}

OQCSerial::~OQCSerial()
{
//    //莫秋兰 释放动态库
//    if(myDll != nullptr)
//        FreeLibrary(myDll); //释放动态库
//    qDebug()<<"在串口类中释放动态库成功";
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日9:39:24
功能：查找可用串口
参数1：SerialName, 串口名控件数组
参数2：
***********************************************************************/
int OQCSerial::SerachSerialPortName(QComboBox *SerialPName)
{
    QSerialPort serial;
    QStringList list;

    //查找一个系统上可用的串行端口列表  存储在对象info
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts ())
    {
        serial.setPort (info);          //设置串行端口信息
        if(serial.open (QIODevice::ReadWrite))
        {
            list.append (serial.portName ());
            SerialPName->addItem (serial.portName ());     //添加到列表
            serial.close ();
        }
    }
    return SerialPName->count ();
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日18:06:00
功能：打开串口、关闭串口
参数1：mySerial，串口号
参数2：Name，串口名控件
参数3：Baud，波特率控件
参数4：Open，打开按钮，按下了哪个按钮
***********************************************************************/
bool OQCSerial::OpenSerial(QSerialPort *mySerial, QComboBox *Name, QComboBox *Baud)
{
    qint32 mybaud;                                  //波特率

    mySerial->setPortName (Name->currentText ());   //设置串口名


    mybaud = Baud->currentText ().toInt ();
    mySerial->setBaudRate (mybaud);                         //设置波特率

    mySerial->setDataBits (QSerialPort::Data8);             //设置数据位：8

    mySerial->setParity (QSerialPort::NoParity);            //设置校验位：无

    mySerial->setStopBits (QSerialPort::OneStop);           //设置停止位：1

    mySerial->setFlowControl (QSerialPort::NoFlowControl);  //设置数据流：无

    return mySerial->open (QIODevice::ReadWrite);          //打开串口
}

void OQCSerial::CloseSerial(QSerialPort *mySerial)
{
    mySerial->clear ();          //清除缓存区和任何操作
    mySerial->close ();          //关闭串口
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日11:00:00
功能：读取串口数据
参数1：mySerial，串口号
参数2：ReceiveEdit，该串口显示读取数据的编辑框
***********************************************************************/
void OQCSerial::ReadData(QSerialPort *mySerial, QByteArray &Readbuf1)
{
    static QByteArray allData;
    QByteArray dataTemp;
#if 0
    qDebug() << "33333333333333" << endl;
#endif
    mySerial->waitForReadyRead (50);
    //qDebug() << mySerial->waitForReadyRead (50) << endl;
    while(!mySerial->atEnd ())
    {
        dataTemp = mySerial->readAll ();     //读取数据
        if(!dataTemp.isNull ())
        {

#if 0
            qDebug() << dataTemp << endl;
#endif
            allData += dataTemp;
        }
    }
    Readbuf1 = allData;
    allData.clear ();
}

/**********************************************************************
作者：梁家榕
时间：2019年5月8日11:38:00
功能：发送串口数据
参数1：mySerial， 串口号
参数2：str, 配置指令
***********************************************************************/
qint64 OQCSerial::WriteData(QSerialPort *mySerial, QByteArray &str)
{
    return mySerial->write (str);  //转为QByteArray再发送
}


//
/**********************************************************************
作者：莫秋兰
时间：2019年5月29日10:38:00
功能：RDA初始化--加载动态库
参数1：
参数2：
***********************************************************************/
bool OQCSerial::RDADllInit()
{
    myDll = LoadLibraryA("AutoTest(1).dll");
    if(myDll)   //动态库加载成功
    {
        (FARPROC &)DLLAutoTestInit = GetProcAddress(myDll, "DLLAutoTestInit");
        (FARPROC &)HostComPortInit = GetProcAddress(myDll, "HostComPortInit");
        (FARPROC &)HostComPortWriteData = GetProcAddress(myDll, "HostComPortWriteData");
        (FARPROC &)HostComPortShutdown = GetProcAddress(myDll, "HostComPortShutdown");
        (FARPROC &)HostComPortcReconnect = GetProcAddress(myDll, "HostComPortcReconnect");

        if(DLLAutoTestInit((DLL_CALLBACK*)RecvDataCallBack))
        {
            return true;
        }
        else {
            return false;
        }
    }
    return true;
}


//
/**********************************************************************
作者：莫秋兰
时间：2019年5月29日10:38:00
功能：串口初始化 （打开串口）
参数1：串口号
参数2：串口名
***********************************************************************/
bool OQCSerial::RDAComInit(int HandleNum,QString PortName)
{
    char *a = PortName.toUtf8().data();
    if(ComInit[HandleNum] == false && PortName != "")
    {
        bool IsSusscc = HostComPortInit(HandleNum,a,nullptr);
        Sleep(200);
        if(true == IsSusscc)
        {
            ComInit[HandleNum] = true;
            return true;
        }
    }
    return false;

}

//
/**********************************************************************
作者：莫秋兰
时间：2019年5月29日14:38:00
功能：RDA串口数据发送
参数1：串口号
参数2：发送的数据
***********************************************************************/
bool OQCSerial::RDAComWriteData(int HandleNum, QString PortData)
{
    char * PortDataChar;

    PortData.replace("AT^GT_CM=","");
    PortData.replace("\r\n","");

    //RDA平台对标准AT指令做一些处理
    if(!PortData.contains("ATD112",Qt::CaseSensitive) ||
       !PortData.contains("ATA",Qt::CaseSensitive) ||
       !PortData.contains("ATH",Qt::CaseSensitive) ||
       !PortData.contains("AT+CSQ",Qt::CaseSensitive) )
    {
        if(PortData.contains("2AT,",Qt::CaseSensitive))
        {
            PortData = "2AT," + PortData;
        }
    }

    PortDataChar = PortData.toUtf8().data();    //将QString转为char *

    //写入数据
    int WriteInt = HostComPortWriteData(HandleNum,PortDataChar);
    qDebug()<<"WriteInt:"<<WriteInt;
    if(WriteInt == 0)
    {
        qDebug()<<"写入成功！";
        return true;
    }
    else
    {
        qDebug()<<"写入数据失败！";
         return false;
    }
}

//
/**********************************************************************
作者：莫秋兰
时间：2019年5月29日15:38:00
功能：关闭RDA串口
参数1：串口号
参数2：
***********************************************************************/
bool OQCSerial::CloseRDACom(int HandleNum)
{
    if(HostComPortShutdown(HandleNum))
    {
        return true;
    }
    else {
        return false;
    }
}

//
/**********************************************************************
作者：莫秋兰
时间：2019年5月29日16:38:00
功能：回调函数
参数1：
参数2：串口号
参数3：读取到的内容
***********************************************************************/
void RecvDataCallBack(int host_or_rs232,int commIndex,char * pContent)
{
    //产品收到指令就会返回特定的值，只需判定返回值是否正确即可
    RDAComReceive[commIndex] =+ pContent;
    RDAComReceive[commIndex].replace("\r\n","");    //去掉"\r\n"
    qDebug()<<RDAComReceive[commIndex]; //字符串

    return ;
}
