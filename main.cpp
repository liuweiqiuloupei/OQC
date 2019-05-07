#include "MainWindow/mainwindow.h"
#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextCodec>
#include <QTextStream>
#include <LogIn/qdlglogin.h>


/*************************************************************
作者：刘维球
时间：2019年4月28日14:17:09
功能：log日志
参数1：
参数2：
***********************************************************************/

int Number()
{
   return 6 ;
}

#define LOGFILEMAX 20 //最大行数
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex0;
    mutex0.lock();

    QString text;
    switch(type)
    {
    case QtInfoMsg:
        text = QString("Info:");
        break;

    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

   // QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(msg).arg(current_date);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex0.unlock();
}

/**
作者：刘维球
功能：主函数
时间：2019年4月26日17:46:19
*/


int main(int argc, char *argv[])
{
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForLocale(codec); //解决汉字乱码问题

    QApplication a(argc, argv);

    //qInstallMessageHandler(outputMessage);

    QDlgLogin   *dlgLogin=new QDlgLogin; //创建对话框
    if (dlgLogin->exec()==QDialog::Accepted)//意思是如果，login登录成功，就跳转到主界面上去。
    {
        QString username = dlgLogin->getUserName();

        MainWindow w;
        w.setUserName(username);
        w.show();

        return a.exec();
    }
    else
    {
        return  0;
    }
}
