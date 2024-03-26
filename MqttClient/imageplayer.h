#ifndef IMAGEPLAYER_H
#define IMAGEPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QStringList>

class ImagePlayer : public QWidget
{
    Q_OBJECT
public:
    explicit ImagePlayer(const QString &folderPath, QWidget *parent = nullptr);

private slots:
   void showNextImage();

private:
   QLabel *imageLabel;
   QTimer *timer;
   QStringList imagePaths;
   int currentIndex;
   QString folderPath;
signals:

};

#endif // IMAGEPLAYER_H
