#ifndef AWIDGET_H
#define AWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTime>
#include <QBasicTimer>

class AImage;

class AWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AWidget(QWidget * parent = 0);
    virtual ~AWidget();

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

    virtual void setBackgroundColor(int, int, int);
    virtual void setImage(AImage *);
    virtual AImage * image();

    void set_callback_click(int);
    void set_callback_longhold(int);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

protected:
    char * tag;
    QLabel * theWidget;
    AImage * widgetBgImage;

    QTime pressedTimeStamp;
    QBasicTimer pressedTimer;

    int callback_click;
    int callback_hold;

protected:
     void timerEvent(QTimerEvent *event);
};

#endif // AWIDGET_H
