#include "checkconfigdialog.h"
#include "ui_checkconfigdialog.h"
#include "MainWindow/mainwindow.h"

extern QStringList ConfigType;         //梁家榕：指令类型
extern QStringList ConfigName;         //梁家榕：指令名字
extern QStringList ConfigReturn;       //梁家榕：指令返回值
extern QStringList ConfigPara;         //梁家榕：指令参数
extern QStringList ConfigSend;         //梁家榕：发送指令

extern QString SoftWareName;       //梁家榕：订单号
extern bool isOpenCheck;       //梁家榕：窗口是否打开

CheckConfigDialog::CheckConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckConfigDialog)
{
    ui->setupUi(this);

    isOpenCheck = 1;    //窗口已打开

    ui->tableWidget->clear ();  //梁家榕：清空配置表
    ui->lineEdit->setText (SoftWareName);   //显示订单号
    add_ConfigDataTable ();     //梁家榕：添加到配置表
}

CheckConfigDialog::~CheckConfigDialog()
{
    isOpenCheck = 0;
    delete ui;
}

/**********************************************************************
作者：梁家榕
时间：2019年5月12日17:08:00
功能：配置信息加入到数据表格
参数1：
参数2：
***********************************************************************/
void CheckConfigDialog::add_ConfigDataTable()
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
