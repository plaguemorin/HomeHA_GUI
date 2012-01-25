#include "newnotificationwidget.h"

NewNotificationWidget::NewNotificationWidget(QWidget *parent) :
    QLabel(parent)
{
    setText("0 new notifications");
}
