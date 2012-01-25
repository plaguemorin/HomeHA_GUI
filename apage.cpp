#include <QtDebug>
#include "apage.h"
#include "aimage.h"


APage::APage(QWidget *parent) : QWidget(parent)
{
    this->pageName = NULL;
    setAutoFillBackground(true);
    setFixedSize(800, 480-20);
}

const char * APage::name() {
    return (const char *) this->pageName;
}

void APage::setName(const char * newName) {
    if (this->pageName) {
        free(this->pageName);
    }

    this->pageName = (char *)malloc(sizeof(char) * (strlen(newName) + 1));
    strcpy(this->pageName, newName);
}

AImage * APage::background() {
    return this->backgroundImage;
}

void APage::setBackground(AImage *a) {
    if (a && this->pageName) {
        qDebug() << "Setting image" << a->name() << "as background of page" << name();
    } else if (this->pageName) {
        qDebug() << "Removing background image of" << name();
    }

    this->backgroundImage = a;

    QPalette pal = this->palette();
    QBrush background = pal.brush(this->backgroundRole());

    if (this->backgroundImage) {
        background.setTextureImage(*this->backgroundImage->qImage());
    } else {
        background.setStyle(Qt::SolidPattern);
    }

    pal.setBrush(this->backgroundRole(), background);
    this->setPalette(pal);
}
