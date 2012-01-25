/**
 * Stomp QT
 */

#include <Qt/QtNetwork>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/qnamespace.h>

#include "hastomp.h"

class StompFrame {
public:
    StompFrame() { }
    StompFrame(QString cmd) { command = cmd; }

    QString command;
    QMap<QString, QString> headers;
    QByteArray body;
};

HAStomp::HAStomp()
    :   QObject() {

    tcpSocket = new QTcpSocket(this);
    tcpSocket->setReadBufferSize(1024);
    corrIdPrefix = QHostInfo::localHostName() + "-";
    messageNumber = 0;
    isConnected = false;

    connect(tcpSocket, SIGNAL(readyRead()), (HAStomp*) this, SLOT(readData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), (HAStomp*) this, SLOT(socketError(QAbstractSocket::SocketError)));
}

HAStomp::~HAStomp() {
    StompFrame * disconnectFrame = new StompFrame("DISCONNECT");
    this->writeFrame(disconnectFrame);
    delete disconnectFrame;

    tcpSocket->abort();
    tcpSocket->close();

    delete tcpSocket;
}

bool HAStomp::connectToBroker(QString host, int port) {
    qDebug() << "Connect to broker:" << host << "port" << port;

    tcpSocket->abort();
    isConnected = false;
    tcpSocket->connectToHost(host, port);
    if (!tcpSocket->waitForConnected(1000)) {
        qDebug() << "Socket was not connected";

        return false;
    }

    qDebug() << "Socket is connected to broker, sending session connect";

    StompFrame * sf = new StompFrame("CONNECT");
    writeFrame(sf);
    delete sf;

    return true;
}


void HAStomp::sendMessage(QMap<QString, QString> headers, QByteArray body) {
    StompFrame * sf = new StompFrame("SEND");
    sf->body = body;
    sf->headers = headers;

    writeFrame(sf);

    delete sf;
}

void HAStomp::sendMessage(QString destination, QMap<QString, QString> headers, QByteArray body) {
    QMap<QString, QString> newHeaders;
    if (headers.count() > 0) {
        QMapIterator<QString, QString> i(headers);
        while (i.hasNext()) {
            i.next();
            newHeaders.insert(i.key(), i.value());
        }
    }

    if (!newHeaders.contains("reply-to")) {
        newHeaders.insert("reply-to", "/queue/gui.reply_to");
    }
    if (!newHeaders.contains("correlation-id")) {
        newHeaders.insert("correlation-id", corrIdPrefix + QString::number(messageNumber++));
    }

    newHeaders.insert("destination", destination);

    this->sendMessage(newHeaders, body);
}

void HAStomp::sendMessage(QString destination, QByteArray body) {
    QMap<QString, QString> newHeaders;
    newHeaders.insert("destination", destination);
    newHeaders.insert("reply-to", "/queue/gui.reply_to");
    newHeaders.insert("correlation-id", "CHANGE_ME");
    this->sendMessage(newHeaders, body);
}

/*
void HAStomp::sendMessage(QString destination, QString body) {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << body;

    out.device()->seek(0);
    this->sendMessage(destination, byteArray);
}*/

void HAStomp::socketError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found. Please check the "
                "host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. "
                "Make sure the stomp server is running, "
                "and check that the host name and port "
                "settings are correct.";
        break;
    default:
        qDebug() << "The following error occurred: " << tcpSocket->errorString();
    }

    //tcpSocket->abort();
    //tcpSocket->close();
}

void HAStomp::readData() {
    StompFrame * frame = readFrame();

    // Figure out what to do with that frame
    if (frame->command == "CONNECTED") {
        qDebug() << "Session is connected";
        isConnected = true;
        emit sessionOpened(frame->headers["session"]);
    } else if (frame->command == "ERROR") {
        // On error
        qDebug() << "on error";
    } else if (frame->command == "MESSAGE") {
        emit messageReceived(frame->headers, frame->body);
    }

    delete frame;
}

void HAStomp::subscribe(QString queue, bool autoack) {
    StompFrame * frame = new StompFrame();

    frame->command = "SUBSCRIBE";
    frame->headers["destination"] = queue;
    if (!autoack) {
        frame->headers["ack"] = "client";
    }

    writeFrame(frame);

    delete frame;
}

void HAStomp::writeFrame(const StompFrame * frame) {
    // Command
    tcpSocket->write(frame->command.toAscii());
    tcpSocket->write("\n");

    // Headers
    if (!frame->headers.isEmpty()) {
        QMapIterator<QString, QString> i(frame->headers);
        while (i.hasNext()) {
            i.next();
            tcpSocket->write(i.key().toAscii());
            tcpSocket->write(":");
            tcpSocket->write(i.value().toAscii());
            tcpSocket->write("\n");
        }
    }
    tcpSocket->write("\n");

    // Payload
    if (!frame->body.isEmpty()) {
        tcpSocket->write(frame->body);
    }

    // End of FRAME
    tcpSocket->write("\x00", 1);
    tcpSocket->flush();

    tcpSocket->waitForBytesWritten(1000);
}

bool HAStomp::needsConnection() {
    return !this->isConnected;
}

StompFrame * HAStomp::readFrame() {
#ifndef MAX
#define MAX(a,b) ( ((a) > (b) ? (a) : (b) ) )
#endif
    StompFrame * ret = new StompFrame;
    QByteArray recv;

    // Read until End-Of-Frame
    while (!recv.contains((char) 0)) {
        recv.append(tcpSocket->read(1));
    }

    // Check to make sure the last charactor is "\n", else read on it
    if (!recv.endsWith('\n')) {
        // Read the \n after the \0
        (void) tcpSocket->read(1);
    }

    // Split header/body
    int splitPos = recv.indexOf("\n\n");
    QByteArray header = recv.left(splitPos);
    QByteArray body = recv.mid(splitPos + 2);

    QListIterator<QByteArray> i(header.split('\n'));
    while (i.hasNext()) {
        QByteArray item = i.next();
        if (ret->command.isEmpty()) {
            ret->command = item;
        } else {
            int semiCol = item.indexOf(':');
            ret->headers.insert(item.left(semiCol), item.mid(semiCol + 1));
        }
    }

    ret->body = body;

    return ret;
}

