#include <Qt/QtCore>
#include <Qt/QtGui>
#include <Qt/QtNetwork>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>
#include <QtCore/QSettings>
#include <QResizeEvent>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "apage.h"
#include "alwaystopwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->m_currentPage = NULL;
}

void MainWindow::presentPage(APage * page) {
    if (this->m_currentPage) {
        qDebug() << "Hidding current page:" << this->m_currentPage->name();
        this->m_currentPage->hide();
        this->layout->removeWidget(this->m_currentPage);
    }

    this->m_currentPage = page;
    this->layout->insertWidget(1, this->m_currentPage);
    this->m_currentPage->show();

    qDebug() << "Current page presented:" << this->m_currentPage->name();
}

void MainWindow::buildLayout() {
    this->topWidget = new AlwaysTopWidget();
    this->layout = new QVBoxLayout();

    this->layout->setMargin(0);
    this->layout->setSpacing(0);
    this->layout->insertWidget(0, this->topWidget);

    QWidget *window = new QWidget(this);

    window->setLayout(this->layout);
    window->setFixedSize(size());
    window->setAutoFillBackground(true);
    window->show();
}

APage * MainWindow::currentPage() {
    return m_currentPage;
}
