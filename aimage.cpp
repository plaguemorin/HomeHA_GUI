#include <QDebug>
#include <QPainter>
#include "aimage.h"


AImage::AImage(QObject *parent) : QObject(parent)
{
    this->img = NULL;
}

AImage::~AImage() {
    delete this->img;
}

void AImage::setName(const char * newName) {
    this->imageName = newName;
}

const char * AImage::name() {
    return this->imageName.toAscii().constData();
}

bool AImage::loadImage(const char * imgPath) {
    qDebug() << "Loading image" << imgPath;

    if (this->img) {
        qDebug() << "Releasing existing image";
        delete img;
    }

    setName(imgPath);
    this->img = new QImage(imgPath);

    return true;
}

void AImage::tint(int r, int g, int b, int a) {
    QPainter painter(this->img);

    painter.fillRect(0, 0, this->width(), this->height(), QColor(r, g, b, a));
}

AImage * AImage::copy() const {
    AImage * newPointer;
    newPointer = new AImage();

    newPointer->img = new QImage(*this->img);

    return newPointer;
}
