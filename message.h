#ifndef MESSAGE_H
#define MESSAGE_H

#include "messageitem.h"

#include <QWidget>
#include <QString>
#include <iostream>
#include <QRect>
#include <QPainter>
#include <QLabel>
#include <QStaticText>
using namespace std;

class Message : public QWidget
{
    Q_OBJECT
public:
    Message(QWidget *parent, int  msgType, QString nickname, QString msg, QString toName="", QString colorHex="");
    ~Message();
    enum services
    {
        Twitch
    };
protected:
    void paintEvent(QPaintEvent *event);
private:
    bool _painted;
//    MessageItem* _messageText;
//    MessageItem* _messageNickname;
//    MessageItem* _messageToName;
//    MessageItem* _messageServiceIcon;
    QString getServiceIconUrl(int);
    int getMaxWidth();
//    QList<MessageItem*> _listMessageItems;

signals:

public slots:

};

#endif // MESSAGE_H
