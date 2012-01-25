#ifndef SIMPLECURRENTACTIONWIDGET_H
#define SIMPLECURRENTACTIONWIDGET_H

#include <QLabel>
class APage;

class SimpleCurrentActionWidget : public QLabel
{
    Q_OBJECT
public:
    explicit SimpleCurrentActionWidget(QWidget *parent = 0);

public slots:
    void pageChanged(APage *, APage *);
};

#endif // SIMPLECURRENTACTIONWIDGET_H
