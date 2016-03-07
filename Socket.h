#ifndef IRC_H
#define IRC_H

#include <QNetworkReply>
#include <QTcpSocket>

class Socket : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket *_socket;
    QString _nickname;
    QString _host;
    QString _channel;
    QString _oAuthToken;
    int _port;
private slots:
        virtual void dataReceived();
        void errorReceived();
        virtual void connected();
        void disconnected();
//        virtual void onResult(QNetworkReply*);

public:
    Socket(QString host, int port, QString channel, QString nickname, QString token);
    bool connnectToIrc();
    bool disconnectFromServer();
    ~Socket();
private:

};

#endif // IRC_H
