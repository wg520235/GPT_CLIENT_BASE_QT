#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QMainWindow>
#include <QtMqtt/qmqttclient.h>
#include <QListWidgetItem>
#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QClipboard>
#include <QPixmap>
#include <QAudioRecorder>
#include <QMediaPlayer>

#include "AudioRecorder.h"
#include "LoginDialog.h"
#include <QVBoxLayout>

enum MessageType {
    Text,
    Image
};

enum ObjType {
    GPT,
    YOU
};
// 定义一个结构体来存放解析后的数据
struct ChatContent {
    QString revisedPrompt;
    QString imageData;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MqttClient; }
QT_END_NAMESPACE

class MqttClient : public QMainWindow
{
    Q_OBJECT

public:
    MqttClient(QWidget *parent = nullptr);
    ~MqttClient();

    void mqtt_publish_Content(const QString&  push_topic, const QString&  content);
    QString encodeWavFileToBase64(const QString &filePath);

    void setBase64Image(const QString &base64Data);  // 设置Base64图片
    void receiveLoginInfo(const QString &username, const QString &password);


private slots:
    void updateLogStateChange();
    void onMqttConnected();
    void onMqttDisconnected();

    void mqtt_subscribe();
    void on_send_Button_clicked();
    void on_speech_to_Txt_clicked();
    void on_speech_Button_clicked();
    void on_file_btn_clicked();
    void on_help_btn_clicked();
    void on_PicPlay_btn_clicked();

private:
    Ui::MqttClient *ui;
    QMqttClient *m_client;
    AudioRecorder *recorder;

    void playBase64Encoded_Wav(const QString &base64EncodedWav);
    ChatContent parse_ImageInfoJson(const QString& jsonString);
    void addMessage(const QString &text, const QString &imageBase64, MessageType type, bool isSender);
    void setupContextMenuForLabel(QLabel* label);
    void setLabelContent(QLabel* label, const QString& text, const QString& imageBase64);

    void onLoginButtonClicked();
    void showHelpDialog();

};
#endif // MQTTCLIENT_H
