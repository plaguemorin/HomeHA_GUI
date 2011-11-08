#ifndef AWIDGETBUTTON_H
#define AWIDGETBUTTON_H

#include "awidget.h"

class AWidgetButton : public AWidget
{
    Q_OBJECT

public:
    explicit AWidgetButton(QObject * parent = 0);
    ~AWidgetButton();

    void setLabel(const char *);
    const char *label();

public slots:

    void click();
};

#endif // AWIDGETBUTTON_H
