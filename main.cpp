#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <QtGui/QFontDatabase>
#include <QtGui/QFont>
#include <QtCore/QDebug>
#include <QtGui/QWSServer>
#include <QtCore/QSettings>
#include <QtCore/QFile>

#include "mainwindow.h"
#include "lua_wrapper.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qDebug() << "QT Version: " QT_VERSION_STR;

    QCoreApplication::setOrganizationName("Screenshot");
    QCoreApplication::setOrganizationDomain("screenshot.ca");
    QCoreApplication::setApplicationName("HomeHA");

#ifdef Q_WS_QWS
    qDebug() << "QWS is set";

    QWSServer::setCursorVisible(false);
    QWSServer::setScreenSaver(new MyScreenSaver());
    int timings[3];
    timings[0] = 30000; // light off after 30 seconds
    timings[1] = 45000; // suspend after 60 seconds
    timings[2] = 0;
    QWSServer::setScreenSaverIntervals(timings);
#endif
    qDebug() << "Default application font is:" << app.font().toString();

#if 0
    QFontDatabase database;
    QStringList fonts = database.families();
    for (int i = 0; i < fonts.size(); ++i) {
        qDebug() << "Font: " << fonts.at(i);

        QStringList styles = database.styles(fonts.at(i));
        for (int j = 0; j < styles.size(); ++j) {
            qDebug() << "  Style: " << styles.at(j);
        }
    }
#endif

#define DEF_SET(a,b) if (!settings.contains(a)) { settings.setValue(a, b); }

    qDebug() << "Reading settings....";
    QSettings settings;
    DEF_SET("broker/ip", "localhost");
    DEF_SET("broker/port", (int)61613);
    settings.setValue("last_start", 0);

    //HomeHAStomp::getInstance()->connectToBroker(settings.value("broker/ip").toString(), settings.value("broker/port", 61613).toInt());

    SCRIPT_StartWrapper();
    MainWindow window;
    SCRIPT_ExecuteInitial();

#ifdef Q_WS_QWS
    window.showFullScreen();
#else
    window.show();
    window.resize(800, 480);
#endif
    qDebug() << "Exec !";
    return app.exec();
}
