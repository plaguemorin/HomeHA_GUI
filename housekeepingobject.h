#ifndef HOUSEKEEPINGOBJECT_H
#define HOUSEKEEPINGOBJECT_H

#include <QObject>
#include <QMap>
#include <QByteArray>

class MainWindow;
class APage;
class HAStomp;

/**
  * Basic House Keeping object (Singleton)
  */
class HouseKeepingObject : public QObject
{
    Q_OBJECT
public:
    static HouseKeepingObject * getInstance();

    explicit HouseKeepingObject(QObject * parent = NULL);

    /* Stomp Messaging */
    void setStompMessageCallback(int clb);
    void sendMessage(const char *destination, const char *body);

    /* GUI Related */
    void present(APage *);

    /* Application Related */
    void startApplication();

signals:
    void pageChange(APage * oldPage, APage *newPage);

public slots:
    void stompConnected(QString&);
    void stompMessage(QMap<QString, QString>&, QByteArray&);

private slots:
    void connectToStompBroker();

private:
    MainWindow * mainWindow;
    HAStomp * stomp;
    int onMessageCallbackId;

};

#endif // HOUSEKEEPINGOBJECT_H
