#include <QApplication>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QFont>

#ifdef Q_WS_QWS
#include <QWSServer>
#endif

#include "housekeepingobject.h"

static void platformStart();
static void loadSettings();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QApplication::setOrganizationName("Screenshot");
    QApplication::setOrganizationDomain("screenshot.ca");
    QApplication::setApplicationName("HomeHA");

    platformStart();
    loadSettings();
    HouseKeepingObject::getInstance()->startApplication();

    qDebug() << "Default application font is:" << app.font().toString();

    return QApplication::exec();
}

static void platformStart() {
    qDebug() << "QT Version: " QT_VERSION_STR;

#ifdef Q_WS_QWS
    qDebug() << "QWS is set";
    QWSServer::setCursorVisible(false);

    //QWSServer::setScreenSaver(new MyScreenSaver());
    int timings[] = {
        30000, // light off after 30 seconds
        0 };
    QWSServer::setScreenSaverIntervals(timings);
#endif

    qDebug() << "Application was started with type:" << QApplication::type();
}

static void loadSettings() {
    qDebug() << "Reading settings....";

#define DEF_SET(a,b) if (!settings.contains(a)) { settings.setValue(a, b); }
    QSettings settings;

    DEF_SET("broker/ip", "localhost");
    DEF_SET("broker/port", (int)61613);

    settings.setValue("last_start", 0);
#undef DEF_SET
}
