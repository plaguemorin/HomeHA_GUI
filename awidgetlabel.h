#ifndef AWIDGETLABEL_H
#define AWIDGETLABEL_H

#include "awidget.h"

class AWidgetLabel : public AWidget
{
    Q_OBJECT

public:
    explicit AWidgetLabel(QObject * parent = 0);
    ~AWidgetLabel();

    void setLabel(const char *);
    const char *label();
};

#endif // AWIDGETLABEL_H
