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
#include <QBitmap>
#include <QGraphicsOpacityEffect>
#include <QTextBrowser>
#include <QTextEdit>
using namespace std;




Message::Message(QWidget *parent, int  serviceType, QString nickname, QString msg, QString toName, QString colorHex) : QWidget(parent)
{
    this->setObjectName("message");
    this->_painted = false;
    this->setAutoFillBackground(true);
    QGridLayout *gridLayout = new QGridLayout(this);
    QLabel *lblNickname = new QLabel(nickname);
    QLabel *lblToName = new QLabel(toName);
    QLabel *lblText = new QLabel("");

    QLabel *lblIcon = new QLabel();
    QPixmap pixmap(":/resources/services/images/twitch.png");
    lblIcon->setPixmap(pixmap);
    lblIcon->setMask(pixmap.mask());
    gridLayout->setContentsMargins(10,10,10,10);
    gridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    gridLayout->setColumnStretch(0,0);
    gridLayout->setColumnStretch(1,1);
    gridLayout->setRowStretch(0,0);
    gridLayout->setRowStretch(1,1);
    gridLayout->setRowStretch(2,0);
    gridLayout->addWidget(lblIcon,      0,0,1,1, Qt::AlignTop | Qt::AlignLeft);
    gridLayout->addWidget(lblNickname,  1,0,1,1, Qt::AlignTop | Qt::AlignLeft);
    gridLayout->addWidget(lblToName,    2,0,1,1, Qt::AlignTop | Qt::AlignLeft);
    gridLayout->addWidget(lblText,      1,1,-2,-2, Qt::AlignTop | Qt::AlignLeft);
    if(colorHex =="")
        colorHex = "#2ED6FF";
    lblNickname->setStyleSheet("QLabel { background-color:transparent;color :"+ colorHex +"; }");
    lblText->setStyleSheet("QLabel { background-color: transparent; }");
    lblToName->setStyleSheet("QLabel { background-color: transparent; }");
//    lblText->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
//    lblText->setFrameShape(QFrame::NoFrame);
//    lblText->setAttribute(Qt::WA_TranslucentBackground);
//    lblText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    lblText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
////    lblText->sizePolicy().setHeightForWidth(true);
//    lblText->document()->setDocumentMargin(0);
//    lblText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
//    lblText->setMinimumHeight(15);
    lblText->setWordWrap(true);
    lblText->setText(msg);
    this->adjustSize();
//    lblText->adjustSize();
//    lblText->setFixedHeight(heightForWidth(lblText->width()));
    this->setFixedSize(this->width(),this->height());
}

Message::~Message()
{
//    foreach (MessageItem* item, this->_listMessageItems) {
//      delete item;
//    }
}

int Message::getMaxWidth(){
//    QList<int> list;
//    list.append(this->_messageToName->itemPoint().x()+this->_messageToName->staticText().size().width());
//    list.append(this->_messageNickname->itemPoint().x()+this->_messageNickname->staticText().size().width());
//    list.append(50);

//    return Helper::getMaxValue(list);
    return 0;
}

void Message::paintEvent(QPaintEvent *ev){
//    QWidget::paintEvent(ev);
    QPainter painter(this);

    if(!this->_painted){
        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        this->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->setDuration(400);
        animation->setStartValue(0.01);
        animation->setEndValue(1);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
    this->_painted = true;

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
