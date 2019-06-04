#ifndef INISETTINGS_H
#define INISETTINGS_H
#include <QFile>
#include <QStringList>
/*************************************************************
作者：刘维球
时间：2019年4月26日17:56:39
功能：配置文件的读写
参数1：
参数2：
***********************************************************************/


class INISETTINGS
{
public:
    INISETTINGS();

    QStringList getConnLogInfo();//刘维球，获取数据库的链接信息，2019年4月28日11:08:25

    QString getLoginTable(const QString LogInfoIni);    //梁家榕：获取登录表名
    QStringList getConfigTable(const QString configFile);   //梁家榕：获取配置表链接信息
    QStringList getResultTable(const QString configFile);   //梁家榕：获取结果表链接信息
private:






};

#endif // INI_H
