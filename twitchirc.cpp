#include "twitchirc.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <stdio.h>
#include <QMap>
#include "services.h"
TwitchIRC::TwitchIRC(QString host, int port, QString channel, QString nickname, QString token) : Socket(host,port,channel,nickname,token)
{
    _hostValidated = false;

    this->evaluateChannelBasedHostAddress();
}

TwitchIRC::~TwitchIRC()
{
    delete this;
}

bool TwitchIRC::getHostValidated()
{
    return _hostValidated;
}

void TwitchIRC::setHostValidated(bool hostValidated)
{
    _hostValidated = hostValidated;
}

QHash<QString, QString> *TwitchIRC::getHashEmotes()
{
    return &_hashEmotes;
}

int TwitchIRC::getHashEmotesCount()
{
    return _hashEmotes.count();
}

void TwitchIRC::setHashEmotes(QHash<QString, QString> *hash)
{
    _hashEmotes = *hash;
}

void TwitchIRC::evaluateChannelBasedHostAddress()
{
    QNetworkAccessManager * networkManager;
    QUrl url("https://api.twitch.tv/api/channels/"+_channel+"/chat_properties");
    QNetworkRequest request;
    request.setUrl(url);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onChatAddressesResult(QNetworkReply*)));
    QNetworkReply* reply;
    try{
       reply = networkManager->get(request);  // GET
    }
    catch(std::exception &e){
       qDebug() << reply->errorString();
    }

}

void TwitchIRC::parseJsonReply(QNetworkReply* reply)
{
    QString data = (QString) reply->readAll();
     qDebug() << "Response received. Parsing...";
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray servers = jsonObject["chat_servers"].toArray();
    QString newHost = servers[rand() % servers.count()-1+0].toString();
    QString newIp = newHost.split(":")[0];
    QString newPort = newHost.split(":")[1];

    _host = newIp;
    _port = newPort.toInt();
    setHostValidated(true);
    qDebug() << "Obtained chat-server for "<< _channel << "->IP: " << _host << " Port: " << _port;
    this->connnectToIrc();
}

void TwitchIRC::parseMessage(QString msg)
{
    QString nickname;
    QString color;
    QString receiver;
    QStringList privMsgList = msg.split("PRIVMSG #"+_channel.toLower() + " :");
    QStringList msgList = privMsgList[0].split( ";", QString::SkipEmptyParts);
    QMap<QString, QString> msgMap;
    for( int i = 0; i < msgList.size(); i++ )
            msgMap[msgList[i].split( "=" )[0]] = msgList[i].split( "=" )[1];

    msgMap["messageText"] = privMsgList[1];
    nickname = msgMap["display-name"];
    if(nickname == ""){
        int start = msgMap["user-type"].indexOf(":")+1;
        int end = msgMap["user-type"].indexOf("!");
        int length=end-start;
        nickname = msgMap["user-type"].mid(start, length);
//        qDebug() << nickname;
    }
    msgMap["messageText"].replace(msgMap["messageText"].indexOf("\r"), 2, " ");
    msgMap["messageText"].replace(msgMap["messageText"].indexOf("\n"), 2, " ");
    foreach(QString word, msgMap["messageText"].split(" ")){
        if(_hashEmotes.contains(word)){
            QString img = "<img src='https://static-cdn.jtvnw.net/emoticons/v1/"+_hashEmotes[word]+"/1.0'/>";
            msgMap["messageText"].replace(msgMap["messageText"].indexOf(word), word.length(), img);
        }
    }


    emit messageReceived(Services::Twitch, _channel, nickname, msgMap["@color"], msgMap["messageText"], "");
}

void TwitchIRC::loadTwitchEmotes()
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QUrl url("https://twitchemotes.com/api_cache/v2/images.json");
    QNetworkRequest request;
    request.setUrl(url);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onTwitchEmotesResult(QNetworkReply*)));
    networkManager->get(request);  // GET
}

void TwitchIRC::dataReceived()
{
    while(_socket->canReadLine()){
        QString line = _socket->readLine();
        if(line.contains("ACK") && line.contains("membership")){
                qDebug() << "Membership acknowledged. Sending JOIN request...";
                QByteArray  join = QString("JOIN #"+_channel.toLower()+"\r\n").toUtf8();
                _socket->write(join);
        }
        if(line.contains("PING")){
            _socket->write("PONG :tmi.twitch.tv\r\n");
            qDebug() << "Responded with PONG";
        }
        if(line.contains("twitchnotify")){

        }
        else if(line.contains("PRIVMSG")){
            parseMessage(line);
        }

//        qDebug() << line << endl;
    }
}

void TwitchIRC::connected()
{
    QByteArray  nick = QString("NICK "+_nickname.toLower()+"\r\n").toUtf8();
    QByteArray  token = QString("PASS "+_oAuthToken+"\r\n").toUtf8();

    _socket->write(token);
    _socket->write(nick);
    _socket->write("CAP REQ :twitch.tv/membership\r\n");
    _socket->write("CAP REQ :twitch.tv/tags\r\n");
    _socket->write("CAP REQ :twitch.tv/commands\r\n");

    if(!_socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << _socket->errorString();
    }

}

void TwitchIRC::onChatAddressesResult(QNetworkReply *reply)
{
    if(!reply->error())
         parseJsonReply(reply);

}

void TwitchIRC::onTwitchEmotesResult(QNetworkReply *reply)
{
    QString data = (QString) reply->readAll();
     qDebug() << "Emotes JSON received. Parsing...";
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject json = jsonResponse.object();

    QVariantHash emotes = json["images"].toObject().toVariantHash();
//    qDebug() << jsonResponse;

    for(QVariantHash::const_iterator iter = emotes.begin(); iter != emotes.end(); ++iter) {
        _hashEmotes.insert(iter.value().toMap()["code"].toString() ,iter.key() );
    }

    qDebug() << "Emotes loading finished";
}

