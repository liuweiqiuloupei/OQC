#ifndef CHECKCONFIGDIALOG_H
#define CHECKCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class CheckConfigDialog;
}

class CheckConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckConfigDialog(QWidget *parent = nullptr);
    ~CheckConfigDialog();

    void add_ConfigDataTable();     //梁家榕：添加到配置表
private:
    Ui::CheckConfigDialog *ui;

};

#endif // CHECKCONFIGDIALOG_H
