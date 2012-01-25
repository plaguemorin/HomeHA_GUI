#include <QTime>
#include <QDate>


#include "simpletimewidget.h"

SimpleTimeWidget::SimpleTimeWidget(QWidget *parent) :
    QLabel(parent)
{
    this->updateTimeEvent();

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateTimeEvent()));
    this->timer->setInterval(30 * 1000);
    this->timer->start();
}

void SimpleTimeWidget::updateTimeEvent() {
    QString text =
            QDate::currentDate().toString("dddd, MMMM d yyyy")
            + " " +
            QTime::currentTime().toString("hh:mm");
    setText(text);
}
