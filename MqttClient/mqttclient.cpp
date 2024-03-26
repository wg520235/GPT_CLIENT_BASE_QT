#include "mqttclient.h"
#include "ui_mqttclient.h"

#include <QtCore/QDateTime>
#include <QtWidgets/QMessageBox>
#include <QJsonValue>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QGuiApplication>
#include <QBuffer>
#include <QPixmap>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QApplication>
#include <QCoreApplication>
#include <QAudioInput>
#include <QFile>
#include <QTimer>
#include <QAudioOutput>
#include <QTextEdit>
#include <QToolTip>


#include "AudioRecorder.h"
#include "customlabel.h"
#include "logindialog.h"
#include "helpdialog.h"

bool saveBase64EncodedWavToFile(const QString& base64EncodedWav, const QString& outputFilePath);

MqttClient::MqttClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MqttClient)
{
    ui->setupUi(this);
    //设置背景图片
    this->resize(600,800);//背景大小
    QPalette pa(this->palette());

    QImage img = QImage("inlogo.png");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);
    pa.setBrush(QPalette::Window,*pic);
    this->setPalette(pa);

    //输入框大小设置
    ui->send_textEdit->setFixedHeight(100);//高度
    ui->send_textEdit->setStyleSheet("QTextEdit { border: 1px solid transparent; }"
                                     "QTextEdit { font-size: 18pt; }");
    //设置ui->listWidget的边框
    ui->listWidget->setStyleSheet("QListWidget { border: 1px solid transparent; }");
    //发送按钮的属性设置
    ui->send_Button->setStyleSheet("QPushButton {"
                          "background-color: white;" // 按钮的背景颜色
                          "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                          "border-width: 2px;" // 边框宽度
                          "border-radius: 10px;" // 边框圆角半径
                          "border-color: beige;" // 边框颜色
                          "font: bold 16px;" // 字体加粗，大小为14px
                          "min-width: 10em;"
                          "padding: 6px;" // 内边距
                          "color: green;" // 字体颜色
                          "}"
                          "QPushButton:pressed {"
                          "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                          "border-style: inset;"
                          "}"
                          "QPushButton { color: green; }"// 默认状态下的字体颜色
                          "QPushButton:hover { color: blue; }" // 鼠标悬停状态
                          "QPushButton:pressed { color: red; }" // 按下状态
                          "QPushButton { min-width: 200px; min-height: 50px; max-width: 200px; max-height: 50px; }"
                          "");

    ui->speech_Button->setStyleSheet("QPushButton {"
                          "background-color: white;" // 按钮的背景颜色
                          "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                          "border-width: 2px;" // 边框宽度
                          "border-radius: 10px;" // 边框圆角半径
                          "border-color: beige;" // 边框颜色
                          "font: bold 14px;" // 字体加粗，大小为14px
                          "min-width: 10em;"
                          "padding: 6px;" // 内边距
                          "color: green;" // 字体颜色
                          "}"
                          "QPushButton:pressed {"
                          "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                          "border-style: inset;"
                          "}"
                          "QPushButton { color: green; }"// 默认状态下的字体颜色
                          "QPushButton:hover { color: blue; }" // 鼠标悬停状态
                          "QPushButton:pressed { color: red; }" // 按下状态
                           "QPushButton { min-width: 100px; min-height: 30px; max-width: 200px; max-height: 50px; }"
                          );
    ui->speech_to_Txt->setStyleSheet("QPushButton {"
                          "background-color: white;" // 按钮的背景颜色
                          "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                          "border-width: 2px;" // 边框宽度
                          "border-radius: 10px;" // 边框圆角半径
                          "border-color: beige;" // 边框颜色
                          "font: bold 14px;" // 字体加粗，大小为14px
                          "min-width: 10em;"
                          "padding: 6px;" // 内边距
                          "color: green;" // 字体颜色
                          "}"
                          "QPushButton:pressed {"
                          "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                          "border-style: inset;"
                          "}"
                          "QPushButton { color: green; }"// 默认状态下的字体颜色
                          "QPushButton:hover { color: blue; }" // 鼠标悬停状态
                          "QPushButton:pressed { color: red; }" // 按下状态
                           "QPushButton { min-width: 100px; min-height: 30px; max-width: 200px; max-height: 50px; }"
                           );
    ui->file_btn->setStyleSheet("QPushButton {"
                          "background-color: white;" // 按钮的背景颜色
                          "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                          "border-width: 2px;" // 边框宽度
                          "border-radius: 10px;" // 边框圆角半径
                          "border-color: beige;" // 边框颜色
                          "font: bold 14px;" // 字体加粗，大小为14px
                          "min-width: 10em;"
                          "padding: 6px;" // 内边距
                          "color: green;" // 字体颜色
                          "}"
                          "QPushButton:pressed {"
                          "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                          "border-style: inset;"
                          "}"
                          "QPushButton { color: green; }"// 默认状态下的字体颜色
                         "QPushButton:hover { color: blue; }" // 鼠标悬停状态
                         "QPushButton:pressed { color: red; }" // 按下状态
                          "QPushButton { min-width: 50px; min-height: 30px; max-width: 200px; max-height: 50px; }"
                          );
    ui->speech_to_Txt->setDisabled(true);
    ui->file_btn->setDisabled(true);
    ui->send_Button->setDisabled(true);
    ui->speech_Button->setDisabled(true);
    // 创建AudioRecorder pcm实例
   recorder = new AudioRecorder(this);

#if 0
   //初始化mqtt
    m_client = new QMqttClient(this);
    m_client->setHostname("185.239.68.121");
    m_client->setPort(8883);
    m_client->setKeepAlive(60);

    m_client->setUsername("NikJiang");
    m_client->setPassword("nik520");
    m_client->setCleanSession(true); // 或者 false，取决于你的需求

    connect(m_client, &QMqttClient::stateChanged, this, &MqttClient::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &MqttClient::onMqttDisconnected);
    connect(m_client, &QMqttClient::connected, this, &MqttClient::onMqttConnected);

    if (m_client->state() == QMqttClient::Disconnected) {
        m_client->connectToHost();
    } else {
        m_client->disconnectFromHost();
    }

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(" Received Topic: ")
                    + topic.name()
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');

        //qDebug()<<"content:"<<content;
        if(0 == topic.name().compare("command/openAi_Api/response/openai_chat")){

            // 如果消息不为空，则添加到聊天记录中
            if (!message.isEmpty()) {
                addMessage(message, NULL, Text, true); // 假设当前用户是发送者
            }

        }
        else if(0 == topic.name().compare("command/openAi_Api/response/openai_image")){
            //解析imagedata和revisedPrompt
            ChatContent content;
            if (!message.isEmpty()) {
                  content = parse_ImageInfoJson(message);

                  addMessage("GPT:"+content.revisedPrompt, content.imageData, Image, true); // 假设当前用户是发送者
             }

        }
        else if(0 == topic.name().compare("command/openAi_Api/response/openai_speech_to_speech"))
        {
            //接收base64音频并解码播放
             playBase64Encoded_Wav(message);
             //保存到本地文件
             saveBase64EncodedWavToFile(message, "openai_speech.wav");

             qDebug()<<"playing.................";
        }else if(0 == topic.name().compare("command/openAi_Api/response/openai_speech_to_txt"))
        {
            // 如果消息不为空，则添加到聊天记录中
            if (!message.isEmpty()) {
                addMessage(message, NULL, Text, false); // 假设当前用户是发送者
            }

        }else if(0 == topic.name().compare("command/openAi_Api/response/openai_file_to_txt"))
        {
           // qDebug()<<"message:"<<message;
            // 如果消息不为空，则添加到聊天记录中
            if (!message.isEmpty()) {
                addMessage(message, NULL, Text, true); // 假设当前用户是发送者
            }

        }

    });
#endif

    //以连接按钮的clicked信号到一个槽函数，登录界面
    connect(ui->login_btn, &QPushButton::clicked, this, &MqttClient::onLoginButtonClicked);//跳到登录界面
    //以连接按钮的clicked信号到一个槽函数，帮助界面
    connect(ui->help_btn, &QPushButton::clicked, this, &MqttClient::showHelpDialog);//跳到登录界面

}

void MqttClient::mqtt_subscribe()
{
    QString sub_topic = "command/openAi_Api/response/#";
    auto subscription = m_client->subscribe(sub_topic);
    if (!subscription) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        m_client->subscribe(sub_topic);
        return;
    }else{
        QMessageBox::critical(this, tr("Ok"), tr("连接成功，可以正常对话！"));

        ui->speech_to_Txt->setEnabled(true);
        ui->file_btn->setEnabled(true);
        ui->send_Button->setEnabled(true);
        ui->speech_Button->setEnabled(true);
    }
}

void MqttClient::showHelpDialog() {
       HelpDialog *dialog = new HelpDialog(this);
       dialog->exec();
}


void MqttClient::onMqttConnected()
{
    qDebug() << "MQTT Client connected successfully.";
    // 在这里处理连接成功的逻辑,开始订阅
    mqtt_subscribe();
}

void MqttClient::onMqttDisconnected()
{
    qDebug() << "MQTT Client disconnected.";
    // 在这里处理连接断开的逻辑
    // QMessageBox::critical(this, QLatin1String("错误"), QLatin1String("连接失败，请确认IP和端口是否正确！"));
    QMessageBox::critical(this, tr("错误"), tr("连接失败，请确认IP和端口是否正确！"));

}

void MqttClient::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
}


QString server_IP;
QString server_Port;
void MqttClient::receiveLoginInfo(const QString &serverIP, const QString &serverPort)
{
    server_IP = serverIP;
    server_Port = serverPort;
}

void MqttClient::onLoginButtonClicked() {
    LoginDialog loginDialog;
    if(loginDialog.exec() == QDialog::Accepted) {
        // 如果用户点击了登录对话框中的“登录”按钮，则处理登录逻辑
         // 在这里处理接收到的用户名和密码
         bool ok;
         int port = server_Port.toInt(&ok);
         if(ok) {
             qDebug() << "转换成功:" << port;
         } else {
             qDebug() << "转换失败";
         }
        qDebug() << "serverIP:" << server_IP << ", serverPort:" << port;
        m_client = new QMqttClient(this);
        m_client->setHostname(server_IP);
        m_client->setPort(port);
        m_client->setKeepAlive(60);

        m_client->setUsername("NikJiang");
        m_client->setPassword("nik520");
        m_client->setCleanSession(true); // 或者 false，取决于你的需求

        connect(m_client, &QMqttClient::stateChanged, this, &MqttClient::updateLogStateChange);
        connect(m_client, &QMqttClient::disconnected, this, &MqttClient::onMqttDisconnected);
        connect(m_client, &QMqttClient::connected, this, &MqttClient::onMqttConnected);

        if (m_client->state() == QMqttClient::Disconnected) {
            m_client->connectToHost();
        } else {
            m_client->disconnectFromHost();
        }

        connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
            const QString content = QDateTime::currentDateTime().toString()
                        + QLatin1String(" Received Topic: ")
                        + topic.name()
                        + QLatin1String(" Message: ")
                        + message
                        + QLatin1Char('\n');

            //qDebug()<<"content:"<<content;
            if(0 == topic.name().compare("command/openAi_Api/response/openai_chat")){

                // 如果消息不为空，则添加到聊天记录中
                if (!message.isEmpty()) {
                    addMessage(message, NULL, Text, true); // 假设当前用户是发送者
                }

            }
            else if(0 == topic.name().compare("command/openAi_Api/response/openai_image")){
                //解析imagedata和revisedPrompt
                ChatContent content;
                if (!message.isEmpty()) {
                      content = parse_ImageInfoJson(message);

                      addMessage("GPT:"+content.revisedPrompt, content.imageData, Image, true); // 假设当前用户是发送者
                 }

            }
            else if(0 == topic.name().compare("command/openAi_Api/response/openai_speech_to_speech"))
            {
                //接收base64音频并解码播放
                 playBase64Encoded_Wav(message);
                 //保存到本地文件
                 saveBase64EncodedWavToFile(message, "openai_speech.wav");

                 qDebug()<<"playing.................";
            }else if(0 == topic.name().compare("command/openAi_Api/response/openai_speech_to_txt"))
            {
                // 如果消息不为空，则添加到聊天记录中
                if (!message.isEmpty()) {
                    addMessage(message, NULL, Text, false); // 假设当前用户是发送者
                }

            }else if(0 == topic.name().compare("command/openAi_Api/response/openai_file_to_txt"))
            {
               // qDebug()<<"message:"<<message;
                // 如果消息不为空，则添加到聊天记录中
                if (!message.isEmpty()) {
                    addMessage(message, NULL, Text, true); // 假设当前用户是发送者
                }

            }

        });
    }
}


// 函数实现
void MqttClient::playBase64Encoded_Wav(const QString &base64EncodedWav)
{
    // 解码Base64字符串到WAV数据
    QByteArray wavData = QByteArray::fromBase64(base64EncodedWav.toUtf8());

    // 创建并准备QBuffer对象
    QBuffer *buffer = new QBuffer();
    buffer->setData(wavData);
    buffer->open(QIODevice::ReadOnly);

    // 创建QMediaPlayer对象
    QMediaPlayer *player = new QMediaPlayer;

    // 设置媒体源为QBuffer
    player->setMedia(QMediaContent(), buffer);

    // 信号连接，确保资源被适当管理
    QObject::connect(player, &QMediaPlayer::stateChanged, [player, buffer](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState) {
            buffer->deleteLater();
            player->deleteLater();
        }
    });

    // 开始播放音频
    player->play();
}

bool saveBase64EncodedWavToFile(const QString& base64EncodedWav, const QString& outputFilePath)
{
    // 首先，将Base64编码的字符串解码回原始的WAV数据
    QByteArray wavData = QByteArray::fromBase64(base64EncodedWav.toUtf8());

    // 然后，创建一个文件对象，并尝试以写入模式打开它
    QFile file(outputFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Unable to open file for writing");
        return false;
    }

    // 将解码后的WAV数据写入文件
    file.write(wavData);

    // 关闭文件
    file.close();

    return true;
}

QString insertLineBreaks(const QString &text, int lineLength) {
    QString result;
    int startIndex = 0;

    while (startIndex < text.length()) {
        int endIndex = startIndex + lineLength < text.length() ? startIndex + lineLength : text.length();
        result += text.mid(startIndex, endIndex - startIndex);
        if (endIndex < text.length()) {
            result += "\n";
        }
        startIndex = endIndex;
    }

    return result;
}

void MqttClient::setupContextMenuForLabel(QLabel* label) {
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(label, &QLabel::customContextMenuRequested, [label](const QPoint &pos) {
        Q_UNUSED(pos);

        QMenu menu;
        QAction *copyAction = menu.addAction("复制文本");

        QObject::connect(copyAction, &QAction::triggered, [label]() {
            QApplication::clipboard()->setText(label->text());
        });

        menu.exec(QCursor::pos());
    });
}


void MqttClient::addMessage(const QString &content, const QString &imageBase64, MessageType type, bool isSender)
{

    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;


    QLabel *label;
    ui->send_textEdit->clear();
    if (type == Text) {
        QString processedText = content;//insertLineBreaks(content, 30);//每行25个字符
        if (isSender) {
            //显示新的内容
            label = new QLabel("GPT:\n"+processedText);
            //设置label字体大小
            QFont font = label->font();
            font.setPointSize(18); // 设置字体大小为24点
            label->setFont(font);
            label->setStyleSheet("QLabel { background-color: #FFFAF0; }");

        } else {
            label = new QLabel("YOU:\n"+processedText);
            //设置label字体大小
            QFont font = label->font();
            font.setPointSize(18); // 设置字体大小为24点
            label->setFont(font);
            label->setStyleSheet("QLabel { background-color: lightgreen; }");
        }
        // 设置label的尺寸策略以确保可以扩展和显示较长的文本
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        // 允许文本剪切，确保文本超出标签长度时，鼠标悬停能显示
        label->installEventFilter(this);
        // 确保启用换行
        label->setWordWrap(true);
        //添加label内容复制功能
        setupContextMenuForLabel(label);
        //标志允许用户使用鼠标选择文本
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        //添加widget
        layout->addWidget(label);
      }
    else if (type == Image){

        QString htmlContent = QString(
            "<html>"
            "<head>"
            "<style>"
            "p { word-wrap: break-word; }"
            "</style>"
            "</head>"
            "<body>"
            "<p style=\"font-size:18px;\">%1</p>" // 设置文本字体大小为18px
            "<br>"
            "<img src=\"data:image/png;base64,%2\" width=\"256\" height=\"256\" alt=\"Image\">" // Base64编码的图片
            "</body>"
            "</html>"
        ).arg(content.toHtmlEscaped(), imageBase64); // 确保HTML特殊字符被正确处理
        // 创建CustomLabel的实例
        CustomLabel *label_image = new CustomLabel(this);
        label_image->textContent = content;
        label_image->imageBase64 = imageBase64;
        //设置双击显示图片
        label_image->setBase64Image(imageBase64);
        label_image->setText(htmlContent);
        //估计内容的大小并调整QListWidgetItem的大小，以确保内容完全显示
        label_image->adjustSize();
        // 允许文本剪切，确保文本超出标签长度时，鼠标悬停能显示
        label_image->installEventFilter(this);
        // 确保启用换行
        label_image->setWordWrap(true);
        //设置背景颜色
        label_image->setStyleSheet("QLabel { background-color: #FFFAF0; }");
        //标志允许用户使用鼠标选择文本
        label_image->setTextInteractionFlags(Qt::TextSelectableByMouse);
        layout->addWidget(label_image);
    }
    widget->setLayout(layout);
    // 调整CustomWidgetItem的大小以适应内容
   // adjustSize();
    // 根据内容调整列表项高度
    item->setSizeHint(widget->sizeHint());
    // 将自定义的widget设置为item的显示widget
    ui->listWidget->setItemWidget(item, widget);

}

ChatContent MqttClient::parse_ImageInfoJson(const QString& jsonString) {
    ChatContent content;

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (!doc.isObject()) {
        // 如果不是一个JSON对象，返回空的结构体
        return content;
    }

    QJsonObject obj = doc.object();

    // 从JSON对象中提取revised_promptt字段
    if (obj.contains("revised_promptt") && obj["revised_promptt"].isString()) {
        content.revisedPrompt = obj["revised_promptt"].toString();
    }

    // 从JSON对象中提取imagedata字段
    if (obj.contains("imagedata") && obj["imagedata"].isString()) {
        content.imageData = obj["imagedata"].toString();
    }

    return content;
}


MqttClient::~MqttClient()
{
    delete m_client;
    delete ui;
}

void MqttClient::on_send_Button_clicked()
{

    QString req_text = ui->send_textEdit->toPlainText();
    //req_text.remove(QRegularExpression("[\\n]")); // 仅移除换行符和回车符
    QString push_topic;
    //判断image和text
    if((-1 != req_text.indexOf("draw:")) || (-1 != req_text.indexOf("画："))){
       push_topic = "command/openAi_Api/request/openai_txt_to_image";
    }else{
       push_topic = "command/openAi_Api/request/openai_chat";
    }
    if (m_client->state() == QMqttClient::Connected) {
        if (m_client->publish(push_topic, ui->send_textEdit->toPlainText().toUtf8()) == -1){
            QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
        }
    } else {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("MQTT Client is not connected. Can't publish."));
        qDebug() << "MQTT Client is not connected. Can't publish.";
    }

    addMessage(ui->send_textEdit->toPlainText().toUtf8(), NULL, Text, false); // 假设当前用户是发送者
    ui->send_textEdit->setText("AI思考中...");

}


void MqttClient::mqtt_publish_Content(const QString&  push_topic, const QString&  content)
{
    if (m_client->publish(push_topic, content.toUtf8()) == -1){
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
    }
}

QString MqttClient::encodeWavFileToBase64(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading: " << filePath;
        return QString();
    }

    QByteArray fileContent = file.readAll();
    file.close();

    QString base64String = fileContent.toBase64();
    return base64String;
}

void MqttClient::on_speech_to_Txt_clicked()
{

    QString pcmFilePath = "audio_to_txt.pcm";
    if(0 == ui->speech_to_Txt->text().compare("语音输入")){
        // 开始录音，指定输出文件名
        qDebug()<<"start recorder";
        recorder->startRecording(pcmFilePath);
        ui->speech_to_Txt->setText("停止");
        ui->speech_to_Txt->setStyleSheet("QPushButton {"
                              "background-color: white;" // 按钮的背景颜色
                              "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                              "border-width: 2px;" // 边框宽度
                              "border-radius: 10px;" // 边框圆角半径
                              "border-color: beige;" // 边框颜色
                              "font: bold 14px;" // 字体加粗，大小为14px
                              "min-width: 10em;"
                              "padding: 6px;" // 内边距
                              "color: green;" // 字体颜色
                              "}"
                              "QPushButton:pressed {"
                              "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                              "border-style: inset;"
                              "}"
                              "QPushButton { color: red; }"// 默认状态下的字体颜色
                              "QPushButton:pressed { color: green; }" // 按下状态
                               "QPushButton { min-width: 100px; min-height: 30px; max-width: 200px; max-height: 50px; }"
                               );
    }else{
        //1、停止pcm录音
        recorder->stopRecording();

        //2、pcm编码成wav
        QString wavFilePath = "audio_to_txt.wav";
        int sampleRate = 16000; // 采样率
        int channels = 1;       // 声道数
        int bitDepth = 16;      // 位深度
        recorder->savePCMAsWAV(pcmFilePath, wavFilePath, sampleRate, channels, bitDepth);

        //3、将wav用base64编码
        QString base64String = encodeWavFileToBase64(wavFilePath);

        //4、将编码后的wav上传到openai
        if (!base64String.isEmpty()) {
            QString topic = "command/openAi_Api/request/openai_speech_to_txt";
            mqtt_publish_Content(topic, base64String);
        }

        //5、按钮显示speech
        ui->speech_to_Txt->setText("语音输入");
        ui->speech_to_Txt->setStyleSheet("QPushButton {"
                              "background-color: white;" // 按钮的背景颜色
                              "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                              "border-width: 2px;" // 边框宽度
                              "border-radius: 10px;" // 边框圆角半径
                              "border-color: beige;" // 边框颜色
                              "font: bold 14px;" // 字体加粗，大小为14px
                              "min-width: 10em;"
                              "padding: 6px;" // 内边距
                              "color: green;" // 字体颜色
                              "}"
                              "QPushButton:pressed {"
                              "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                              "border-style: inset;"
                              "}"
                              "QPushButton { color: green; }"// 默认状态下的字体颜色
                              "QPushButton:pressed { color: red; }" // 按下状态
                              "QPushButton { min-width: 100px; min-height: 30px; max-width: 200px; max-height: 50px; }"
                               );
        qDebug()<<"stop recorder";
    }
    ui->send_textEdit->setText("AI思考中...");
}


void MqttClient::on_speech_Button_clicked()
{

    ui->send_textEdit->setEnabled(true);
    QString pcmFilePath = "output.pcm";
    if(0 == ui->speech_Button->text().compare("语音对话")){
        // 开始录音，指定输出文件名
        qDebug()<<"start recorder";
        recorder->startRecording(pcmFilePath);
        ui->speech_Button->setText("停止");
        ui->speech_Button->setStyleSheet("QPushButton {"
                              "background-color: white;" // 按钮的背景颜色
                              "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                              "border-width: 2px;" // 边框宽度
                              "border-radius: 10px;" // 边框圆角半径
                              "border-color: beige;" // 边框颜色
                              "font: bold 14px;" // 字体加粗，大小为14px
                              "min-width: 10em;"
                              "padding: 6px;" // 内边距
                              "color: green;" // 字体颜色
                              "}"
                              "QPushButton:pressed {"
                              "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                              "border-style: inset;"
                              "}"
                              "QPushButton { color: red; }"// 默认状态下的字体颜色
                             "QPushButton:hover { color: blue; }" // 鼠标悬停状态
                             "QPushButton:pressed { color: green; }" // 按下状态
                              "QPushButton { min-width: 100px; min-height: 30px; max-width: 200px; max-height: 50px; }"
                              );
    }else{
        //1、停止pcm录音
        recorder->stopRecording();

        //2、pcm编码成wav
        QString wavFilePath = "output.wav";
        int sampleRate = 16000; // 采样率
        int channels = 1;       // 声道数
        int bitDepth = 16;      // 位深度
        recorder->savePCMAsWAV(pcmFilePath, wavFilePath, sampleRate, channels, bitDepth);

        //3、将wav用base64编码
        QString base64String = encodeWavFileToBase64(wavFilePath);

        //4、将编码后的wav上传到openai
        if (!base64String.isEmpty()) {
            QString topic = "command/openAi_Api/request/openai_speech_to_speech";
            mqtt_publish_Content(topic, base64String);
        }

        //5、按钮显示speech
        ui->speech_Button->setText("语音对话");
        ui->speech_Button->setStyleSheet("QPushButton {"
                              "background-color: white;" // 按钮的背景颜色
                              "border-style: outset;" // 边框样式 (outset, inset, groove, ridge)
                              "border-width: 2px;" // 边框宽度
                              "border-radius: 10px;" // 边框圆角半径
                              "border-color: beige;" // 边框颜色
                              "font: bold 14px;" // 字体加粗，大小为14px
                              "min-width: 10em;"
                              "padding: 6px;" // 内边距
                              "color: green;" // 字体颜色
                              "}"
                              "QPushButton:pressed {"
                              "background-color: rgb(255, 0, 0);" // 按钮被按下时的背景颜色
                              "border-style: inset;"
                              "}"
                              "QPushButton { color: green; }"// 默认状态下的字体颜色
                             "QPushButton:hover { color: blue; }" // 鼠标悬停状态
                             "QPushButton:pressed { color: red; }" // 按下状态
                              "QPushButton { min-width: 100px; min-height: 30px; max-width: 200px; max-height: 50px; }"
                              );
        qDebug()<<"stop recorder";
    }
    ui->send_textEdit->setText("AI思考中...");
}

void MqttClient::on_file_btn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        // 错误处理
        return;
    }

    QByteArray fileContent = file.readAll();
    QString base64Content = fileContent.toBase64();

    // 现在base64Content包含了文件的Base64编码，可以进行上传操作
    if (!base64Content.isEmpty()) {
        QString topic = "command/openAi_Api/request/openai_file_to_text";
        mqtt_publish_Content(topic, base64Content);
        qDebug()<<"file to openai";
    }
    ui->send_textEdit->setText("AI分析中...");
}

void MqttClient::on_help_btn_clicked()
{

}

#include "imageplayer.h"

void MqttClient::on_PicPlay_btn_clicked()
{
    // 打开文件夹选择对话框，让用户选择图片文件夹
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Image Folder"), QDir::homePath());

    // 检查用户是否真的选择了一个文件夹
    if (!folderPath.isEmpty()) {
        // 创建并显示图片播放窗口，开始循环播放选定文件夹中的图片
        ImagePlayer *imagePlayer = new ImagePlayer(folderPath, this);
        // 设置窗口标志
        imagePlayer->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
        imagePlayer->show();
    }

}


