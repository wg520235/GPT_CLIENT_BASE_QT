#include "mqttclient.h"

#include <QSurfaceFormat>
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication a(argc, argv);
    MqttClient w;
    w.setWindowIcon(QIcon("logo.png"));
    w.show();
    return a.exec();
}
