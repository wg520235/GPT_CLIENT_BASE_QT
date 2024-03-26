#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>
#include "mqttclient.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 连接新的信号到Processor的槽

    MqttClient *appclit = new MqttClient;

    connect(this, &LoginDialog::loginInfo, appclit, &MqttClient::receiveLoginInfo);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_clicked()
{
    QString username = ui->serverIP_lineEdit->text().toUtf8();
    QString password = ui->serverPort_lineEdit->text().toUtf8();
    emit loginInfo(username, password);
    this->close();
}
