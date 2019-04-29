#ifndef QWDIALOGDELETE_H
#define QWDIALOGDELETE_H

#include <QDialog>
#include <database.h>
namespace Ui {
class QWDialogDelete;
}

class QWDialogDelete : public QDialog
{
    Q_OBJECT
private:

    void closeEvent(QCloseEvent *event);//关闭事件
    void showEvent(QShowEvent *event);  //显示事件

public:
    explicit QWDialogDelete(QWidget *parent = 0);
    ~QWDialogDelete();

    QString encrypt(const QString &str);
private slots:
    void on_btnOk_clicked();
    void on_btnClose_clicked();

private:
    Ui::QWDialogDelete *ui;
    QString username;
    QString pwd;
    QString calipwd;
    Database db;
};

#endif // QWDIALOGLOCATE_H
