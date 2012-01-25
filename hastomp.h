#ifndef HASTOMP_H
#define HASTOMP_H


#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QString>
#include <Qt/QtNetwork>

class StompFrame;

class HAStomp : public QObject
{
        Q_OBJECT

public:
    HAStomp();
    ~HAStomp();

    bool connectToBroker(QString host, int port);
    void subscribe(QString queue, bool autoack = true);

    void sendMessage(QMap<QString, QString> headers, QByteArray body);
    void sendMessage(QString destination, QMap<QString, QString> headers, QByteArray body);
    void sendMessage(QString destination, QByteArray body);

    bool needsConnection();

signals:
    void messageReceived(QMap<QString, QString> &headers, QByteArray &body);
    void sessionOpened(QString &sessionId);

private slots:
    void readData();
    void socketError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket * tcpSocket;
    void writeFrame(const StompFrame *);
    StompFrame * readFrame();
    QString corrIdPrefix;
    int messageNumber;

    bool isConnected;
};

#endif // HASTOMP_H
