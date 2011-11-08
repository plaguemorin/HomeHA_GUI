#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class APage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    static MainWindow * getInstance();

    void presentPage(APage *);

signals:

public slots:

private:
    APage * currentPage;

};

#endif // MAINWINDOW_H
