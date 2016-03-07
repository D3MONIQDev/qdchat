#include "message.h"
#include <iostream>
#include <QRect>
#include <QPainter>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helper.h"
using namespace std;




Message::Message(QWidget *parent, int  serviceType, QString nickname, QString msg, QString toName, QString colorHex) : QWidget(parent)
{
    this->setObjectName("message");
    this->_painted = false;
    this->_messageNickname = new MessageItem(this, MessageItem::Text, QPoint(3, 30),nickname, "<b>From: ", "</b>", false, "", "", colorHex);
    this->_messageToName = new MessageItem(this, MessageItem::Text, QPoint(3, 50),toName != ""? "To: "+toName : toName);
    this->_messageServiceIcon = new MessageItem(this, MessageItem::ServiceIcon, QPoint(2.5, 2.5), getServiceIconUrl(serviceType));
    this->_messageText = new MessageItem(this, MessageItem::Text,QPoint(getMaxWidth()+5, 5), msg);
    this->_messageText->setStaticTextWidth(this->parentWidget()->width()-getMaxWidth()-30);
    this->setAutoFillBackground(true);

    QFont font;
    font.setFamily("Verdana");
    QFontMetrics fm(font);
    QList<int> findMaxList;
    findMaxList.append(this->_messageNickname->itemPoint().y()+this->_messageNickname->staticText().size().height()+fm.height());
    findMaxList.append(this->_messageToName->itemPoint().y()+this->_messageToName->staticText().size().height()+fm.height());
    findMaxList.append(this->_messageText->itemPoint().y()+this->_messageText->staticText().size().height()+fm.height());
    int max = Helper::getMaxValue(findMaxList);
    this->setFixedHeight(max);
    this->_listMessageItems.append(this->_messageNickname);
    this->_listMessageItems.append(this->_messageServiceIcon);
    this->_listMessageItems.append(this->_messageToName);
    this->_listMessageItems.append(this->_messageText);

}

Message::~Message()
{
    foreach (MessageItem* item, this->_listMessageItems) {
      delete item;
    }
}

int Message::getMaxWidth(){
    QList<int> list;
    list.append(this->_messageToName->itemPoint().x()+this->_messageToName->staticText().size().width());
    list.append(this->_messageNickname->itemPoint().x()+this->_messageNickname->staticText().size().width());
    list.append(50);

    return Helper::getMaxValue(list);
}

void Message::paintEvent(QPaintEvent *ev){
    QWidget::paintEvent(ev);
    QPainter painter(this);

    if(!this->_painted){


        QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
        animation->setDuration(400);
        animation->setStartValue(QRect(this->width()+400, this->y(), this->width(), this->height()));
        animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));
        animation->start();
        this->_painted = true;
    }

//    painter.fillRect(0,0, this->width(), this->height(), QColor("#55053667"));
//    painter.setPen(QColor("#ffffff"));
    QColor defaultColorPen(painter.pen().color());
    foreach(MessageItem * item, this->_listMessageItems){
        switch(item->itemType()){
            case MessageItem::Text:
                if(item->color() != "")
                    painter.setPen(QColor(item->color()));
                else
                    painter.setPen(defaultColorPen);
                painter.drawStaticText(item->itemPoint().x(), item->itemPoint().y(), item->staticText());
                break;
            case MessageItem::ServiceIcon:
                QRectF target(2.5, 2.5, 24.0, 24.0);
                QRectF source(0.0, 0.0, 24.0, 24.0);
                QPixmap pixmap(item->value());
                painter.drawPixmap(target, pixmap, source);
                break;
        }


    }

}

QString Message::getServiceIconUrl(int type)
{
    QString url;
    switch(type){
        case Twitch:
            url = ":/resources/services/images/twitch.png";
        break;
    }
    return url;
}
