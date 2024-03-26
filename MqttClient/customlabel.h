#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QFileDialog>
#include <QImage>
#include <QBuffer>

class CustomLabel : public QLabel {
    Q_OBJECT

public:
    explicit CustomLabel(QWidget *parent = nullptr);

    // 存储文本内容和Base64编码的图片数据
    QString textContent;
    QString imageBase64;
    // 设置Base64图片
    void setBase64Image(const QString &base64Data);
protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void copyTextToClipboard(const QString &text);
    bool saveBase64ImageToLocalFile(const QString &imageBase64, const QString &filePath);

private:
    QPixmap currentPixmap;  // 当前显示的图片

};

#endif // CUSTOMLABEL_H
