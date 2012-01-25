#ifndef ALWAYSTOPWIDGET_H
#define ALWAYSTOPWIDGET_H

#include <QWidget>

class QLabel;

class AlwaysTopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlwaysTopWidget(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QLabel * left;
    QLabel * middle;
    QLabel * right;

    void setupCommon(QLabel*);

signals:

public slots:

};

#endif // ALWAYSTOPWIDGET_H
