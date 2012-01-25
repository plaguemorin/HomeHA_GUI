#include <QDebug>
#include <QEvent>

#include "simplecurrentactionwidget.h"
#include "apage.h"
#include "housekeepingobject.h"

SimpleCurrentActionWidget::SimpleCurrentActionWidget(QWidget *parent) :
    QLabel(parent)
{
    connect(HouseKeepingObject::getInstance(), SIGNAL(pageChange(APage*,APage*)), SLOT(pageChanged(APage*,APage*)));
}

void SimpleCurrentActionWidget::pageChanged(APage * oldPage, APage * newPage) {
    (void)oldPage;
    setText(newPage->name());
}
