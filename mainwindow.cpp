#include <Qt/QtCore>
#include <Qt/QtGui>
#include <Qt/QtNetwork>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>
#include <QtCore/QSettings>

#include "mainwindow.h"
#include "apage.h"

MainWindow * instance = NULL;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    instance = this;
    this->currentPage = NULL;
}

MainWindow * MainWindow::getInstance() {
    return instance;
}

void MainWindow::presentPage(APage * page) {
    if (this->currentPage) {
        qDebug() << "Hidding current page:" << this->currentPage->name();
        this->currentPage->setParent(NULL);
    }

    this->currentPage = page;
    this->currentPage->setParent(this);
    this->currentPage->show();

    qDebug() << "Current page presented:" << this->currentPage->name();
}
