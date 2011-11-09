#include <QWidget>
#include <QMouseEvent>
#include <QDebug>

#include "lua_wrapper.h"

#include "awidget.h"
#include "aimage.h"


AWidget::AWidget(QWidget * parent) : QWidget(parent)
{
    this->tag = NULL;
    this->theWidget = new QLabel(this);
    this->theWidget->setAlignment(Qt::AlignCenter);
}

AWidget::~AWidget() {

}

const char * AWidget::name() {
    return (const char *)this->tag;
}

void AWidget::setName(const char * newName) {
    if (this->tag) {
        free(this->tag);
    }

    this->tag = (char *)malloc(sizeof(char) * (strlen(newName) + 1));
    strcpy(this->tag, newName);
}

void AWidget::setParent(QWidget * parent) {
    QWidget::setParent(parent);
}

int AWidget::width() {
    return QWidget::width();
}

int AWidget::height() {
    return QWidget::height();
}

void AWidget::setSize(int w, int h) {
    QWidget::setFixedSize(w, h);
    this->theWidget->setFixedSize(w, h);
}

void AWidget::setLabel(const char * label) {
    this->theWidget->setText(QString(label));
}

const char *AWidget::label() {
    return this->theWidget->text().toUtf8().constData();
}

int AWidget::x() {
    return QWidget::x();
}

int AWidget::y() {
    return QWidget::y();
}

void AWidget::setPosition(int x, int y) {
    QWidget::move(x, y);
}

void AWidget::setVisibility(bool visible) {
    QWidget::setVisible(visible);
}

void AWidget::mousePressEvent(QMouseEvent * mouseEvent) {
    (void)mouseEvent;

    this->pressedTimeStamp = QTime::currentTime();
    if (this->pressedTimer.isActive()) {
        this->pressedTimer.stop();
    }

    this->pressedTimer.start(1000, this);
}

void AWidget::mouseReleaseEvent(QMouseEvent * mouseEvent) {
    (void)mouseEvent;

    // Figure out hold vs click
    if (this->pressedTimer.isActive()) {
        this->pressedTimer.stop();

        if (this->callback_click) {
            SCRIPT_DoCallback_Widget(this, this->callback_click);
        }
    }
}

void AWidget::timerEvent(QTimerEvent *event) {
    (void)event;
    this->pressedTimer.stop();

    if (this->callback_hold) {
        SCRIPT_DoCallback_Widget(this, this->callback_hold);
    }
}

void AWidget::set_callback_click(int c) { this->callback_click = c; }
void AWidget::set_callback_longhold(int c) { this->callback_hold = c; }

void AWidget::setBackgroundColor(int r, int g, int b) {
    QPalette pal = this->theWidget->palette();

    this->theWidget->setPalette(pal);
}

void AWidget::setImage(AImage * a) {
    qDebug() << "Setting image" << a->name() << "as background of widget" << name();
    this->widgetBgImage = a;

    if (this->widgetBgImage) {
        this->theWidget->setAutoFillBackground(true);

        QPalette pal = this->theWidget->palette();
        pal.setBrush(this->theWidget->backgroundRole(), QBrush(*a->qImage()));
        this->theWidget->setPalette(pal);
    }
}

AImage * AWidget::image() {
    return this->widgetBgImage;
}
