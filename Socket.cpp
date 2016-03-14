#include "Socket.h"
#include <QDebug>

void Socket::dataReceived()
{
    qDebug() << "Data received";
}

void Socket::errorReceived()
{
   qDebug() << _socket->errorString() << endl;
}

Socket::Socket(QString host, int port, QString channel, QString nickname, QString token)
{
    _host = host;
    _port = port;
    _channel = channel;
    _nickname = nickname;
    _oAuthToken = token;
    _socket = new QTcpSocket(this);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(errorReceived()));
    connect(_socket, SIGNAL(connected()), this,  SLOT(connected()));
    connect(_socket, SIGNAL(disconnected()), this,  SLOT(disconnected()));
}

bool Socket::connnectToIrc()
{
    qDebug() << "Trying to connect to" << _host << ":" << _port << "..." << endl;
    _socket->connectToHost(_host, _port);

    return true;
}

bool Socket::disconnectFromServer()
{
    disconnect(_socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    disconnect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(errorReceived()));
    disconnect(_socket, SIGNAL(connected()), this,  SLOT(connected()));
    disconnect(_socket, SIGNAL(disconnected()), this,  SLOT(disconnected()));
    _socket->flush();
    _socket->disconnectFromHost();
    _socket->close();
    return true;
}

Socket::~Socket()
{
    disconnectFromServer();
    delete _socket;
}

void Socket::connected()
{

    qDebug() << "Successfully connected to " << _host << ":" << _port << " Server";
}

void Socket::disconnected()
{
    qDebug() << "Disconnected from " << _host << ":" << _port << " Server";
}

