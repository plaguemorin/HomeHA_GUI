#include <Qt>
#include <QLabel>

#include "awidgetlabel.h"


AWidgetLabel::AWidgetLabel(QObject * parent) : AWidget(parent)
{
    this->theWidget = new QLabel();
    this->theType = A_LABEL;
}

AWidgetLabel::~AWidgetLabel() {
    delete this->theWidget;
}

void AWidgetLabel::setLabel(const char * label) {
    ((QLabel *)this->theWidget)->setText(QString(label));
}

const char *AWidgetLabel::label() {
    return ((QLabel *)this->theWidget)->text().toUtf8().constData();
}
