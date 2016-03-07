#ifndef TWITCHIRC_H
#define TWITCHIRC_H
#include "Socket.h"

#include <QNetworkReply>

class TwitchIRC : public Socket
{
    Q_OBJECT
public:
    TwitchIRC(QString host, int port, QString channel, QString nickname, QString token);
    ~TwitchIRC();
    bool getHostValidated();
    void setHostValidated(bool hostValidated);
signals:
    void messageReceived(int service, QString channel, QString nickname, QString colorHex, QString text, QString receiver);
private:
    QString _validHostAddress;
    bool _hostValidated;
    void evaluateChannelBasedHostAddress();
    void parseJsonReply(QNetworkReply*);
    void parseMessage(QString msg);
private slots:
    void dataReceived();
    void connected();
    void onResult(QNetworkReply*);
};

#endif // TWITCHIRC_H
