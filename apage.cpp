#include "apage.h"

APage::APage(QWidget *parent) : QWidget(parent)
{
    this->pageName = NULL;
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
