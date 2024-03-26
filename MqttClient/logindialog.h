#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "mqttclient.h"
#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void loginInfo(const QString &username, const QString &password);

private slots:
    void on_buttonBox_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
