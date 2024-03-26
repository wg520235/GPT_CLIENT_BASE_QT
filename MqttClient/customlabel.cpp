#include "customlabel.h"

#include <QDateTime>
#include <QVBoxLayout>
#include <QPixmap>

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent) {
    // 初始化CustomLabel，如果需要
}

static int g_mousenum = 0;
void CustomLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        QMenu contextMenu;
        QAction *copyTextAction = contextMenu.addAction("复制文本");
        QAction *saveImageAction = contextMenu.addAction("保存图片到本地");

        QAction *selectedAction = contextMenu.exec(event->globalPos());
        if (selectedAction == copyTextAction) {
            copyTextToClipboard(this->textContent);
        } else if (selectedAction == saveImageAction) {
            // 获取当前日期和时间
            QDateTime currentDateTime = QDateTime::currentDateTime();
            // 将日期和时间转换为字符串
            QString dateTimeString = currentDateTime.toString("yyyyMMddHHmmss");

            QString filePath = QFileDialog::getSaveFileName(this, tr("保存图片"),
                                   dateTimeString,
                                   tr("Images (*.png *.xpm *.jpg)"));
            if (!filePath.isEmpty()) {
                saveBase64ImageToLocalFile(this->imageBase64, filePath);
            }
        }
    } else {
        QLabel::mousePressEvent(event);
    }
    g_mousenum++;
    if (event->button() == Qt::LeftButton && !currentPixmap.isNull() && g_mousenum == 2) {
        g_mousenum = 0;
        // 弹出对话框显示放大的图片
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("放大图片");
        QVBoxLayout *layout = new QVBoxLayout(dialog);

        QLabel *imageLabel = new QLabel(dialog);
        imageLabel->setPixmap(currentPixmap.scaled(currentPixmap.size() * 1, Qt::KeepAspectRatio, Qt::SmoothTransformation));  // 2倍放大

        layout->addWidget(imageLabel);
        dialog->setLayout(layout);

        dialog->exec();
    }
}

void CustomLabel::copyTextToClipboard(const QString &text) {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

bool CustomLabel::saveBase64ImageToLocalFile(const QString &imageBase64, const QString &filePath) {
    QByteArray imageData = QByteArray::fromBase64(imageBase64.toLocal8Bit());
    QImage image;
    if (!image.loadFromData(imageData)) {
        return false;
    }

    return image.save(filePath);
}

void CustomLabel::setBase64Image(const QString &base64Data) {
    QByteArray byteArray = QByteArray::fromBase64(base64Data.toLocal8Bit());
    QImage image;
    image.loadFromData(byteArray);
    currentPixmap = QPixmap::fromImage(image);
    setPixmap(currentPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));  // 自适应标签大小
}


