#include "awidget.h"
#include <QLabel>

AWidget::AWidget(QObject * parent) : QObject(parent)
{
}

AWidget::~AWidget() {

}

AWidgetType AWidget::type() {
    return this->theType;
}

const char * AWidget::name() {
    return this->tag.toUtf8().constData();
}

void AWidget::setName(const char * newName) {
    this->tag = newName;
}

void AWidget::setParent(QWidget * parent) {
    this->theWidget->setParent(parent);
}

int AWidget::width() {
    return this->theWidget->width();
}

int AWidget::height() {
    return this->theWidget->height();
}

void AWidget::setSize(int w, int h) {
    this->theWidget->setFixedSize(w, h);
}

void AWidget::setLabel(const char * ignored) {
    (void)ignored;
}

const char * AWidget::label() {
    return NULL;
}

int AWidget::x() {
    return this->theWidget->x();
}

int AWidget::y() {
    return this->theWidget->y();
}

void AWidget::setPosition(int x, int y) {
    this->theWidget->move(x, y);
}

void AWidget::setVisibility(bool visible) {
    this->theWidget->setVisible(visible);
}

void AWidget::set_callback_click(int c) {
    this->callback_click = c;
}

int AWidget::get_callback_click() {
    return this->callback_click;
}
