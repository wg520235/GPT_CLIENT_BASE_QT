#include "imageplayer.h"
#include <QDir>
#include <QVBoxLayout>
#include <QDebug>

ImagePlayer::ImagePlayer(const QString &folderPath, QWidget *parent)
    : QWidget(parent), folderPath(folderPath), currentIndex(0) {

    // 设置窗口标志以确保有关闭按钮和标准窗口边框
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    imageLabel = new QLabel(this);
    timer = new QTimer(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(imageLabel);
    setLayout(layout);

    qDebug()<<"input folderPath:"<<folderPath;
    QDir dir(folderPath);
    QStringList filters;
    filters << "*.jpg" << "*.png";
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Name);
    imagePaths = dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

    connect(timer, &QTimer::timeout, this, &ImagePlayer::showNextImage);
    timer->start(40); // 设置图片切换间隔为2000毫秒
}

void ImagePlayer::showNextImage() {
    if (currentIndex >= imagePaths.size()){
        currentIndex = 0;
    }
    // 首先，检查imagePaths是否为空
    if (imagePaths.isEmpty()) {
       qDebug() << "No images found in the directory.";
       timer->stop(); // 如果没有图片，停止计时器
       return;
    }

    // 确保currentIndex在有效范围内
    if (currentIndex < 0 || currentIndex >= imagePaths.size()) {
       qDebug() << "Resetting currentIndex to 0.";
       currentIndex = 0; // 重置currentIndex为0
    }

    // 构建完整的文件路径
    QString filePath = QDir(folderPath).absoluteFilePath(imagePaths.at(currentIndex));
    QPixmap pixmap(filePath);

    // 检查pixmap是否有效
    if (!pixmap.isNull()) {
       imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
       resize(pixmap.size()); // 调整窗口大小以匹配图片大小
       currentIndex++; // 准备显示下一张图片
    } else {
       qDebug() << "Failed to load image:" << filePath;
    }

    // 如果已经是列表中的最后一张图片，回到第一张图片
    if (currentIndex >= imagePaths.size()) {
       currentIndex = 0;
    }
}
