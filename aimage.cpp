#include <QDebug>
#include "aimage.h"


AImage::AImage(QObject *parent) : QObject(parent)
{
    this->img = NULL;
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
}
