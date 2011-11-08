#include <Qt/QtCore>
#include <Qt/QtGui>
#include <Qt/QtNetwork>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>
#include <QtCore/QSettings>

#include "mainwindow.h"
#include "lua_wrapper.h"

MainWindow * instance = NULL;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    instance = this;
    SCRIPT_LoadPage("home");
}

MainWindow * MainWindow::getInstance() {
    return instance;
}
