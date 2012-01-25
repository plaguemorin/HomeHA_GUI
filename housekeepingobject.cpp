#include <QtConfig>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>

#include "housekeepingobject.h"
#include "mainwindow.h"
#include "lua_wrapper.h"
#include "hastomp.h"

#define STOMP_DEFAULT_DESTINATION "/queue/HomeAutomation.GUI"
#define STOMP_NOTIFICATION_DESTINATION  "/topic/HomeLogic.Web.Notification.Global"

static HouseKeepingObject * hainstance = NULL;

HouseKeepingObject::HouseKeepingObject(QObject * parent) :
    QObject(parent)
{
    hainstance = this;

    this->stomp = new HAStomp();
    connect(this->stomp, SIGNAL(messageReceived(QMap<QString,QString>&,QByteArray&)), SLOT(stompMessage(QMap<QString,QString>&,QByteArray&)));
    connect(this->stomp, SIGNAL(sessionOpened(QString&)), SLOT(stompConnected(QString&)));
}

HouseKeepingObject * HouseKeepingObject::getInstance() {
    if (hainstance == NULL) {
        hainstance = new HouseKeepingObject();
    }

    return hainstance;
}

void HouseKeepingObject::stompConnected(QString &clientId) {
    qDebug() << "Stomp Connected" << clientId;
    this->stomp->subscribe(STOMP_DEFAULT_DESTINATION);
    this->stomp->subscribe(STOMP_NOTIFICATION_DESTINATION);

#ifdef Q_WS_QWS
    mainWindow->showFullScreen();
#else
    mainWindow->show();
    mainWindow->resize(800, 480);
#endif

    mainWindow->buildLayout();
    SCRIPT_ExecuteInitial();
}

void HouseKeepingObject::stompMessage(QMap<QString, QString> &headers, QByteArray &body) {
    if (headers.contains("destination") && headers.value("destination") == STOMP_NOTIFICATION_DESTINATION) {
        printf("A NOTIFICATION !\n");
    }

    if (this->onMessageCallbackId > 0) {
        int j = 0;
        char ** headerKeys = new char*[headers.size()];
        char ** headerValues = new char*[headers.size()];

        QMapIterator<QString, QString> i(headers);
        while (i.hasNext()) {
            i.next();
            QString key = i.key();
            QString value = i.value();

            headerKeys[j] = new char[ key.size() + 1 ];
            headerValues[j] = new char[ value.size() + 1 ];

            strcpy(headerKeys[j], key.toLatin1());
            strcpy(headerValues[j], value.toLatin1());

            ++j;
        }

        SCRIPT_DoCallback_Message(this->onMessageCallbackId, headerKeys, headerValues, body.constData(), headers.size(), body.size());

        j = 0;
        while (j < headers.size()) {
            delete headerKeys[j];
            delete headerValues[j];
            j++;
        }

        delete [] headerKeys;
        delete [] headerValues;
    }

}

void HouseKeepingObject::present(APage * page) {
    emit pageChange(this->mainWindow->currentPage(), page);

    this->mainWindow->presentPage(page);
}

void HouseKeepingObject::setStompMessageCallback(int clb) {
    this->onMessageCallbackId = clb;
}

void HouseKeepingObject::sendMessage(const char *destination, const char *body) {
    qDebug() << "Sending stomp message to" << destination << "with body" << body;
    stomp->sendMessage(destination, body);
}

void HouseKeepingObject::connectToStompBroker() {
    if (this->stomp->needsConnection()) {
        QSettings settings(this);
        this->stomp->connectToBroker(settings.value("broker/ip").toString(), settings.value("broker/port", 61613).toInt());
    }
}

void HouseKeepingObject::startApplication() {
    this->connectToStompBroker();
    this->mainWindow = new MainWindow();
    SCRIPT_StartWrapper();
}


