#ifndef SIMPLETIMEWIDGET_H
#define SIMPLETIMEWIDGET_H

#include <QLabel>
#include <QTimer>

class SimpleTimeWidget : public QLabel
{
    Q_OBJECT
public:
    explicit SimpleTimeWidget(QWidget *parent = 0);

signals:

private:
    QTimer * timer;

public slots:
    void updateTimeEvent();
};

#endif // SIMPLETIMEWIDGET_H
