#include "apage.h"

APage::APage(QWidget *parent) : QWidget(parent)
{
}

const char * APage::name() {
    return this->pageName.toAscii().constData();
}

void APage::setName(const char * newName) {
    this->pageName = QString(newName);
}
