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
    QHash<QString, QString> *getHashEmotes();
    int getHashEmotesCount();
    void setHashEmotes(QHash<QString, QString> *hash);
    void loadTwitchEmotes();
signals:
    void messageReceived(int service, QString channel, QString nickname, QString colorHex, QString text, QString receiver);
private:
    QString _validHostAddress;
    bool _hostValidated;
    QHash<QString, QString> _hashEmotes;
    void evaluateChannelBasedHostAddress();
    void parseJsonReply(QNetworkReply*);
    void parseMessage(QString msg);

private Q_SLOTS:
    void dataReceived();
    void connected();
    void onChatAddressesResult(QNetworkReply*);
    void onTwitchEmotesResult(QNetworkReply*);
};

#endif // TWITCHIRC_H
