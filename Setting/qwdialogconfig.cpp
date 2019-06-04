#include "qwdialogconfig.h"
#include "ui_qwdialogconfig.h"

#include <QMessageBox>
#include <QComboBox>
#include <QDebug>
#include <QSqlQuery>
#include <ui_qwdialogconfig.h>

extern QStringList ConfigType;         //梁家榕：指令类型
extern QStringList ConfigName;         //梁家榕：指令名字
extern QStringList ConfigReturn;       //梁家榕：指令返回值
extern QStringList ConfigPara;         //梁家榕：指令参数
extern QStringList ConfigSend;         //梁家榕：发送指令

extern QString SoftWareName;    //梁家榕：订单号
extern QString MachineName;     //梁家榕：机型
extern QString Station;         //梁家榕：所有配置信息


QWDialogConfig::QWDialogConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWDialogConfig)
{
    ui->setupUi(this);
    this->setWindowTitle("订单配置");
    //this->setWindowFlag(Qt::WindowContextHelpButtonHintlwq);//刘维球，只有一个最大化，防止误操作点击了关闭按钮，2019年5月7日15:09:26
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);



    INISETTINGS  iniSettings; //刘维球，初始化设置，2019年4月28日11:07:50
    QString appDir = QApplication::applicationDirPath(); ////刘维球：当前程序路径，2019年4月28日09:21:02
    const QString  configFile = appDir + "/config.ini" ; ////刘维球，config.ini路径
    QFile file(configFile);
    if(file.exists())
    {
        Configlist = iniSettings.getConfigTable(configFile);     //获取链接信息  即ini文件中的信息
        if(db.ConnectDataBase (Configlist))
            qDebug() << "90数据库连接成功";
        else
            QMessageBox::warning(this, tr("错误提示"), tr("数据库连接失败"), "确 定");

        //订单筛选
        Completer = new QCompleter(ui->comboBox_Proname->model(), this);  //使用者，使用者父对象
        ui->comboBox_Proname->setCompleter(Completer);      //使用自动补全功能

        on_comboBox_Proname_activated(SoftWareName);    //重新查询一次订单配置
        ui->comboBox_Proname->setEditText (SoftWareName);//编辑区显示上一次配置的订单号

        //设置搜索订单按钮特效
        ui->pushButton->setStyleSheet ("QPushButton{border-image: url(:/images/search.PNG);}"
                                       "QPushButton:hover{border-image: url(:/images/searching.png);}"
                                       "QPushButton:pressed{border-image: url(:/images/searching.png);}");

    }
    else {
        QMessageBox::warning(this, "错误提示", "查看当前路径下的工程文件是否存在！");
    }
}

QWDialogConfig::~QWDialogConfig()
{
    delete ui;
}


void QWDialogConfig::on_btnok_clicked()
{
    QMessageBox::StandardButton result=QMessageBox::question(this, "确认", "确认配置？",
                                                             QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                                                             QMessageBox::No);
    if (result==QMessageBox::Yes)
    {
        //event->accept();
        this->accept();
    }
    else if (result == QMessageBox::Cancel)     //梁家榕：选择Cancel，清空配置信息,2019年5月17日16:36:00
    {
        ConfigType.clear ();
        ConfigName.clear ();
        ConfigReturn.clear ();
        ConfigPara.clear ();
        ConfigSend.clear ();
        SoftWareName = QString("");  //订单号撤销
        MachineName = QString("");   //机型
        Station = QString("");       //所有配置信息
    }
}

void QWDialogConfig::on_btncancel_clicked()
{
    this->reject();
}

/**********************************************************************
作者：梁家榕
时间：2019年5月12日17:08:00
功能：配置信息加入到数据表格
参数1：
参数2：
***********************************************************************/
void QWDialogConfig::add_ConfigDataTable()
{
    int rows = 0;        //总行数
    rows = ui->tableWidget->rowCount ();
    //qDebug() << "rows1:" << ui->tableWidget->rowCount ();
    if(ConfigType.size () > rows)       //缺少行
    {
        for(int isrow=rows; isrow<ConfigType.size (); isrow++)
        {
            ui->tableWidget->insertRow (isrow);
        }
    }

    rows = ui->tableWidget->rowCount ();  //重新获取总行数
    if(ConfigType.size () < rows)       //有多余行
    {
        for(int rvrow=rows; rvrow>ConfigType.size ()-1; rvrow--)     //从后往前删 eg：5条指令size=5,最后一条row=4
        {
            ui->tableWidget->removeRow (rvrow); //删除多余行
        }
    }
    //qDebug() << "rows2:" << ui->tableWidget->rowCount ();

    for(int i=0; i<ConfigType.size (); i++)     //添加到配置表
    {
        ui->tableWidget->setItem (i, 0, new QTableWidgetItem(ConfigType.at (i)));
        ui->tableWidget->setItem (i, 1, new QTableWidgetItem(ConfigName.at (i)));
        ui->tableWidget->setItem (i, 2, new QTableWidgetItem(ConfigReturn.at (i)));
        ui->tableWidget->setItem (i, 3, new QTableWidgetItem(ConfigPara.at (i)));
        ui->tableWidget->setItem (i, 4, new QTableWidgetItem(ConfigSend.at (i)));
    }

    for(int i=0; i<ConfigType.size (); i++)     //行
    {
        if("共有指令" == ConfigType.at (i))     //列
        {
            for(int j=0; j<ui->tableWidget->columnCount (); j++)    //设置颜色
            {
                ui->tableWidget->item (i, j)->setForeground (Qt::blue);
            }
        }
    }

}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日11:33:00
功能：全部订单加载
参数1：
参数2：
***********************************************************************/
void QWDialogConfig::Proname_Load_All()     //加载全部订单
{
    QSqlQuery query;
    QString readStr;

    QString str = QString("select SoftWare from %1").arg (Configlist.at (Configlist.size ()-1));

    if(query.exec (str))
    {
        while(query.next ())
        {
            readStr = query.value (query.record ().indexOf ("SoftWare")).toString ();
            ui->comboBox_Proname->addItem (readStr);
        }
    }
    else
    {
        qDebug() << query.lastError ();
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日11:33:00
功能：部分订单加载
参数1：
参数2：
***********************************************************************/
void QWDialogConfig::Proname_Load_Part(const QString &Str)    //部分订单加载
{
    QSqlQuery query;
    QString readStr;
    QDateTime SqlTime;      //服务器的时间
    qint64 Days;            //时间差
    QString selStr;         //目标时间

    //梁家榕：获取系统时间，2019年5月18日15:12:00
    LocalDateTime = QDateTime::currentDateTime ();  //获取日期时间
    SelectDateTime = LocalDateTime.toString ("yyyy-MM-dd");     //格式化日期时间

    QString str = QString("select RecordTime, SoftWare from %1").arg (Configlist.at (Configlist.size ()-1));      //查询创建时间
    if(query.exec (str))
    {
        while(query.next ())
        {
            readStr = query.value (query.record ().indexOf ("RecordTime")).toString ();   //查询到的字段
            SqlTime = QDateTime(QDate(readStr.mid (0, 4).toInt (),      //年     2019-03-27  0~4
                                      readStr.mid (5, 2).toInt (),       //月     2019-03-27  5~6
                                      readStr.mid (8, 2).toInt ()));     //日     2019-03-27  8~9
            Days = SqlTime.daysTo (LocalDateTime);      //计算时间差

            if("1周内" == Str && 7 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("半个月内" == Str && 15 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("1个月内" == Str && 30 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("2个月内" == Str && 30*2 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("3个月内" == Str && 30*3 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("半年内" == Str && 30*6 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("1年内" == Str && 30*12 >= Days && 0 <= Days)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
            else if("全部" == Str)
            {
                ui->comboBox_Proname->addItem (query.value (query.record ().indexOf ("SoftWare")).toString ());
            }
        }
    }
    else
    {
        qDebug() << query.lastError ();
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月16日11:33:00
功能：选择订单
参数1：arg1,订单号
参数2：
***********************************************************************/
void QWDialogConfig::on_comboBox_Proname_activated(const QString &arg1)     //激活选项、选择选项时
{
    //清空配置列表
    ConfigType.clear ();
    ConfigName.clear ();
    ConfigReturn.clear ();
    ConfigPara.clear ();
    ConfigSend.clear ();

    QString Str = QString("select * from %1 where SoftWare = '%2'").arg (Configlist.at (Configlist.size ()-1)).arg (arg1);

    db.Select_ConfigInfo(ConfigType, ConfigName, ConfigReturn,
                         ConfigPara, ConfigSend, Str);        //查询配置信息

    add_ConfigDataTable();      //添加到配置表
    //qDebug() << "Config size:" << ConfigName.size ();

    SoftWareName = arg1;    //梁家榕：订单号确认，2019年5月16日16:24:00
    MachineName = db.Select_OneInfo (QString("select * from %1 where SoftWare = '%2'").arg (Configlist.at (Configlist.size ()-1)).arg (arg1),
                                     "MachineName"); //机型确认
    Station = db.Select_OneInfo (QString("select * from %1 where SoftWare = '%2'").arg (Configlist.at (Configlist.size ()-1)).arg (arg1),
                                 "Station"); //所有配置信息确认
}

/**********************************************************************
作者：梁家榕
时间：2019年5月20日10:40:00
功能：选择订单查询方式
参数1：checked，选择状态
参数2：
***********************************************************************/
void QWDialogConfig::on_radioButton_clicked(bool checked)
{
    if(true == checked)        //选择精确查询
    {
        ui->radioButton->setText ("精确查询");
        ui->tableWidget->clear ();      //清空配置表
    }
    else
    {
        ui->radioButton->setText ("模糊查询");
        ui->tableWidget->clear ();      //清空配置表
    }
}

/**********************************************************************
作者：梁家榕
时间：2019年5月20日10:43:00
功能：精准查询订单配置
参数1：
参数2：
***********************************************************************/
void QWDialogConfig::on_pushButton_clicked()
{   
    on_comboBox_Proname_activated(ui->comboBox_Proname->currentText ());        //查询配置

    if(0 >= ConfigName.size ())     //查不到配置时
    {
        QMessageBox::warning(this, tr("错误提示"), tr("查询失败，请输入正确订单号！"), tr("确 定"));
    }

#if 0
    qDebug() << "Config size:" << ConfigName.size ();
#endif
}

/**********************************************************************
作者：梁家榕
时间：2019年5月20日14:20:00
功能：选择订单查询时间
参数1：
参数2：
***********************************************************************/
void QWDialogConfig::on_comboBox_SelectTime_activated(const QString &arg1)
{
    ui->comboBox_Proname->clear (); //清空选项-订单号
    Proname_Load_Part (arg1);
}
