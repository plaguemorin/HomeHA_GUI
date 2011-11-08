#include <Qt>
#include <QDebug>
#include <QPushButton>

#include "awidgetbutton.h"


AWidgetButton::AWidgetButton(QObject * parent) : AWidget(parent)
{
    this->theType = A_BUTTON;
    this->theWidget = new QPushButton(NULL);

    connect((QPushButton *) this->theWidget, SIGNAL(pressed()), SLOT(click()));
}

AWidgetButton::~AWidgetButton() {
    delete this->theWidget;
}

void AWidgetButton::setLabel(const char * label) {
    ((QPushButton *)this->theWidget)->setText(QString(label));
}

const char *AWidgetButton::label() {
    return ((QPushButton *)this->theWidget)->text().toUtf8().constData();
}

void AWidgetButton::click() {
    SCRIPT_DoCallback_Click(this);
}
