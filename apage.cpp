#include "apage.h"

APage::APage(QWidget *parent) : QWidget(parent)
{
}

const char * APage::name() {
    return this->pageName.toUtf8().constData();
}

void APage::setName(const char * newName) {
    this->pageName = newName;
}
