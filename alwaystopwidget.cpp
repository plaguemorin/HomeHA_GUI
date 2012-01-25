#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>

#include "alwaystopwidget.h"

#include "simpletimewidget.h"
#include "simplecurrentactionwidget.h"
#include "newnotificationwidget.h"

AlwaysTopWidget::AlwaysTopWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    setLayout(new QHBoxLayout());
    layout()->setMargin(0);
    layout()->setSpacing(0);

    QPalette pal = this->palette();
    pal.setColor(this->backgroundRole(), Qt::black);
    this->setPalette(pal);

    this->left = new SimpleCurrentActionWidget();
    this->middle = new SimpleTimeWidget();
    this->right = new NewNotificationWidget();

    this->left->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->middle->setAlignment(Qt::AlignCenter);
    this->right->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    this->setupCommon(this->left);
    this->setupCommon(this->middle);
    this->setupCommon(this->right);

    layout()->addWidget(left);
    layout()->addWidget(middle);
    layout()->addWidget(right);
}

void AlwaysTopWidget::setupCommon(QLabel * l)
{
    QPalette pal = l->palette();
    pal.setColor(QPalette::Text, Qt::white);
    pal.setColor(QPalette::Foreground, Qt::white);

    l->setPalette(pal);
    l->setFixedWidth(width() / 3);

    QFont font = l->font();
    font.setPixelSize(12);
    l->setFont(font);
}

void AlwaysTopWidget::resizeEvent(QResizeEvent *ev) {
    setFixedWidth(ev->size().width());
    setFixedHeight(20);

    setupCommon(left);
    setupCommon(middle);
    setupCommon(right);
}
