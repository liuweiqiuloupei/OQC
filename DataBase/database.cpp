#include "DataBase/database.h"
#include <QApplication>
#include <QSettings>
#include <QVector>
#include <qiterator.h>
#include <QList>
#include <QDebug>
#include <QMessageBox>

Database::Database()
{

}

Database::Database(QString connName)
{
    this->connName =  connName ;
    this->db =  QSqlDatabase::addDatabase("QODBC",connName);
}

//刘维球：连接到登录数据库
bool Database::connectLogInfoDb()
{

    QString appDir = QApplication::applicationDirPath(); ////刘维球：当前程序路径，2019年4月28日09:21:02
    const QString  configFile = appDir + "/config.ini" ; ////刘维球，config.ini路径
    QFile IniFile(configFile) ;//刘维球；读取配置路径,2019年4月28日10:52:01
    INISETTINGS  iniSettings; //刘维球，初始化设置，2019年4月28日11:07:50

    if(!IniFile.exists())
    {
        qDebug() << "初始化文件不存在" << endl ;
    }
    else{


        logInfo = iniSettings.getConnLogInfo();     //获取登录信息  即ini文件中的信息
        db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                    "SERVER=%1;"
                                    "DATABASE=%2;"
                                    "UID=%3;"
                                    "PWD=%4;").arg(logInfo[0])
                            .arg(logInfo[1])
                            .arg(logInfo[2])
                            .arg(logInfo[3]));
        if (!db.open())
        {
            qDebug("数据库连接失败"); //
            return false;
        }
        else
        {
            return  true;
        }
    }
}


void Database::createLogTable(QString tablename)
{
    //tablename = "GPSTest.dbo.Test_Engineers2";
    QSqlQuery query(db);
    QString sqlstatement(QString("if not exists (select * from sysobjects where id = object_id('\"%1\"') "
                                 "and OBJECTPROPERTY(id, 'IsUserTable') = 1)"
                                 "create table %2"
                                 "(id int primary key identity(1,1),  username nvarchar(200) not null"
                                 "unique,Grade int default 1,  pwd nvarchar(32) not null,createTime datetime)").arg(tablename).arg(tablename));

}

bool Database::execSql(QString sqlstatment)
{
    QSqlQuery query(db);

    if(query.exec(sqlstatment))
    {
        return true; //执行是否成功
    }
    else
    {
        return  false;//执行失败
    }
}

void Database::disconnDb()
{
    if(db.open())
    {
        db.close();
    }
}

QString Database::getConnname()
{
    return  this->connName;
}

QSqlDatabase Database::getDBName()
{

    return  this->db;
    //添加数据库
}

/**********************************************************************
作者：梁家榕
时间：2019年5月10日15:25:00
功能：连接数据库
参数1：dbList  数据库连接信息
参数2：
***********************************************************************/
bool Database::ConnectDataBase(QStringList &dbList)
{
    QSqlDatabase db = QSqlDatabase::addDatabase ("QODBC");
    db.setDatabaseName (QString("DRIVER={SQL SERVER};"
                                "SERVER=%1;"                //服务器名
                                "DATABASE=%2;"              //数据库名
                                "UID=%3;"                   //用户名
                                "PWD=%4").arg (dbList.at (0))   //密码   //10.10.11.90,6033
                        .arg (dbList.at (1))               //NetMarkIMEI2
                        .arg (dbList.at (2))
                        .arg (dbList.at (3)));             //jiminewpower
    if(false == db.open ())
    {
        qDebug() << db.lastError ();
        return false;
    }

    return true;
}

/**********************************************************************
作者：梁家榕
时间：2019年5月12日16:44:00
功能：查询数据库配置信息
参数1：ConfigType, 配置类型
参数2：ConfigName, 配置名字
参数3：ConfigSend, 指令
参数4：ConfigPara, 返回参数
参数5：ConfigReturn, 返回值
参数6：InStr,  查询语句
***********************************************************************/
void Database::Select_ConfigInfo(QStringList &ConfigType, QStringList &ConfigName,
                                 QStringList &ConfigReturn, QStringList &ConfigPara,
                                 QStringList &ConfigSend,  const QString &InStr)    //查询配置信息
{
    QSqlQuery query;
    QStringList InfoList;
    int Num = 0;
    QString readStr;
    QString tableStr;

//梁家榕：查询语句，2019年5月12日16:46:00
    //QString str = QString("select * from TestSystemSettingFunc where SoftWare = '%1'").arg (InStr);
    if(query.exec (InStr))
    {
        query.next ();

        //qDebug() << "All Col:" << query.record ().count ();

        readStr = query.value (query.record ().indexOf ("Station")).toString ();        //获取Station信息
        readStr = readStr.mid (readStr.indexOf ("}}")+2, 2);        //截取字符串
        Num = readStr.toInt ();
        //qDebug() << "This Col:" << Num;

        for(int i=0; i<Num; i++)        //逐条查询配置信息
        {
            tableStr = QString("Setting%1").arg (i);        //标题拼接
            readStr = query.value (query.record ().indexOf (tableStr)).toString ();

            InfoList = readStr.split ("@@");        //分割字符串
            QStringList list = InfoList.at (InfoList.size ()-1).split ("}}");
            InfoList.pop_back ();
            InfoList.push_back (list.at (0));

            //加入到各个配置信息数组中
            ConfigType.append (InfoList.at (0).mid (0, 4));         //指令类型
            ConfigName.append (InfoList.at (0).mid (4, InfoList.at (0).size ()-4));     //指令名字

            ConfigReturn.append (InfoList.at (1));  //返回值
            ConfigPara.append (InfoList.at (2));    //参数
            ConfigSend.append (InfoList.at (3));    //发送指令
        }
    }

    /*for(int i=0; i<5; i++)
    {
        qDebug() << ConfigType.at (i);
        qDebug() << ConfigName.at (i);
        qDebug() << ConfigSend.at (i);
        qDebug() << ConfigPara.at (i);
        qDebug() << ConfigReturn.at (i);
    }*/
}

/**********************************************************************
作者：梁家榕
时间：2019年5月23日8:18:00
功能：查询一个信息
参数1：InStr,  查询语句
参数2：targetStr， 目标字段
***********************************************************************/
QString Database::Select_OneInfo(const QString &InStr, const QString &targetStr)
{
    QSqlQuery query;
    QString StrBuf = "";
    if(query.exec (InStr))
    {
        query.next ();
        StrBuf = query.value (query.record ().indexOf (targetStr)).toString ();
    }
    else
    {
        qDebug() << query.lastError ();
    }
    return StrBuf;
}

/**********************************************************************
作者：梁家榕
时间：2019年5月23日10:09:00
功能：查询总行数
参数1：table,  表名
参数2：
***********************************************************************/
int Database::Select_RowsNum(QString table)
{
    QSqlQuery query;
    int RowsNum = 0;
    QSqlDatabase db;
    if(query.exec (QString("select * from %1").arg (table)))
    {
        while(query.next ())
        {
            if(db.driver ()->hasFeature (QSqlDriver::QuerySize))
            {
                RowsNum = query.size ();
            }
            else
            {
                query.last ();
                RowsNum = query.at ()+1;
            }
        }
    }
    return RowsNum;
}






