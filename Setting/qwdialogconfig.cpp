#include "qwdialogconfig.h"
#include "ui_qwdialogconfig.h"

#include <QMessageBox>
#include <QComboBox>
#include <QDebug>
#include <QSqlQuery>
QWDialogConfig::QWDialogConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWDialogConfig)
{
    ui->setupUi(this);
    this->setWindowTitle("订单配置");
    //this->setWindowFlag(Qt::WindowContextHelpButtonHintlwq);//刘维球，只有一个最大化，防止误操作点击了关闭按钮，2019年5月7日15:09:26
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);

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
    else {
        this->reject();
    }
}

void QWDialogConfig::on_btncancel_clicked()
{
    this->reject();
}
