#ifndef AWIDGET_H
#define AWIDGET_H

#include <QWidget>
#include "lua_wrapper.h"

enum AWidgetType {
    A_LABEL,
    A_BUTTON
};

class AWidget : public QObject
{
    Q_OBJECT

public:
    explicit AWidget(QObject * parent = 0);
    virtual ~AWidget();

    AWidgetType type();

    const char * name();
    void setName(const char *);

    void setParent(QWidget *);

    int width();
    int height();
    void setSize(int, int);

    int x();
    int y();
    void setPosition(int, int);

    void setVisibility(bool);

    virtual void setLabel(const char *);
    virtual const char * label();

    void set_callback_click(int);
    int get_callback_click();
protected:
    QString tag;
    QWidget * theWidget;
    AWidgetType theType;

    int callback_click;
};

#endif // AWIDGET_H
