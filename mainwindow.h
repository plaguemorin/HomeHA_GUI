#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hastomp.h"

class APage;
class QBoxLayout;
class AlwaysTopWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    APage * currentPage();
    void presentPage(APage *);
    void buildLayout();

signals:

private:
    APage * m_currentPage;
    QBoxLayout * layout;

    AlwaysTopWidget * topWidget;

};

#endif // MAINWINDOW_H
