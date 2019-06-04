#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDriver>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlResult>
#include <INI/ini.h>

#include <QtSql>
#include <QSqlDatabase>

class Database
{
public:
    Database();
    Database(QString connName);

    bool connectLogInfoDb();//刘维球：连接OQC的用户
    void createLogTable(QString connName);//创建表格
    bool execSql(QString sqlstatment);//执行sql语句
    void disconnDb();
    QString getConnname();
    QSqlDatabase getDBName();

    bool ConnectDataBase(QStringList &dbList);          //梁家榕：连接到数据库
    void Select_ConfigInfo(QStringList &ConfigType, QStringList &ConfigName,
                           QStringList &ConfigSend, QStringList &ConfigPara,
                           QStringList &ConfigReturn,  const QString &InStr);   //梁家榕：查询配置信息
    QString Select_OneInfo(const QString &InStr, const QString &targetStr); //梁家榕：查询一个信息
    int Select_RowsNum(QString table);   //梁家榕：查询一共多少行
private:
    QSqlDatabase db;//数据库名
    QString connName;//连接名
    QStringList logInfo ;//刘维球，OQC登录信息

};

#endif // DATABASE_H
