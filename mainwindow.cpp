#include "mainwindow.h"
#include "Socket.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include "twitchirc.h"
#include <iostream>
#include <QtWidgets>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QObject>
#include <QMessageBox>
#include <QChar>
#include <message.h>
#include <QApplication>

using namespace std;


MainWindow::MainWindow(QWidget *parent, QString windowName) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile styleFile(":/styles/css/DefaultWindowStyle.qss");
    styleFile.open(QFile::ReadOnly);
    QByteArray bytes = styleFile.readAll();
    styleFile.close();
    this->setStyleSheet(QString(bytes));
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QMenuBar *menuBar = new QMenuBar();

    QMenu *qdMenu = menuBar->addMenu("QDChat");
    QAction *closeAction = qdMenu->addAction("Close Window");
    QAction *exitAction = qdMenu->addAction("Exit");

    this->windowName = windowName;
    loadWindowSettings();
    centralW = new QWidget(this);
//    this->setWindowFlags(Qt::SubWindow);


    _messageCounter = 0;
    messageWidget = new QWidget(this);
    messageWidget->setObjectName("messagesPlaceholder");
    QWidget *bttnWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();
    messageLayout = new QVBoxLayout();
    gridLayout->setContentsMargins(0,3,0,3);
//    gridLayout->setSpacing(0);
    messageLayout->setContentsMargins(0,10,0,10);
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    QPushButton *button1 = new QPushButton("Add Random Message");
    QPushButton *button3 = new QPushButton("Clear ALL");
//    button1->setStyleSheet("background:white;");
//    button3->setStyleSheet("background:white;");
    scrollarea = new QScrollArea();
    scrollarea->setFrameShape(QFrame::NoFrame);
    scrollarea->setWidget(messageWidget);
    scrollarea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
//    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    messageLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    scrollarea->setWidgetResizable(true);
    centralW->setLayout(gridLayout);
    messageWidget->setLayout(messageLayout);
//    messageWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    scrollarea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
//    messageWidget->setFixedWidth(this->width()-20);
    bttnWidget->setLayout(buttonsLayout);
//    bttnWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonsLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    buttonsLayout->setAlignment(Qt::AlignCenter);
    QWidget * counterWidget = new QWidget(bttnWidget);
    counterWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    _counterLCD = new QLCDNumber(counterWidget);
    _counterLCD->setDigitCount(4);
    counterWidget->setMinimumSize(10, 30);
    _counterLCD->setMinimumSize(1,30);
    _counterLCD->setMaximumSize(100,30);
//    _counterLCD->setMaximumSize(this->width(), 30);
    _counterLCD->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    _counterLCD->display(QString::number(messageLayout->count()));
    _counterLCD->setStyleSheet("text-align: center;");
//    counterWidget->setStyleSheet("background-color: transparent;");
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    QHBoxLayout* counterLayout = new QHBoxLayout();

    counterLayout->setAlignment(Qt::AlignCenter);
    counterLayout->addSpacerItem(spacer);
    counterLayout->addWidget(counterWidget);
    counterLayout->addSpacerItem(spacer);
    buttonsLayout->setContentsMargins(0,0,0,0);
    buttonsLayout->addLayout(counterLayout);
    buttonsLayout->addWidget(button1);
    buttonsLayout->addWidget(button3);
    messageLayout->insertStretch(-1);

    gridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    gridLayout->addWidget(menuBar, 0, 0);
    gridLayout->addWidget(scrollarea, 1, 0);
    gridLayout->addWidget(bttnWidget, 2, 0);
    gridLayout->addWidget(new QSizeGrip(this), 3,0,1,0,Qt::AlignBottom | Qt::AlignLeft);
    gridLayout->addWidget(new QSizeGrip(this), 3,0,1,0, Qt::AlignBottom | Qt::AlignRight);

    gridLayout->setAlignment(messageLayout,Qt::AlignTop);
    gridLayout->setAlignment(buttonsLayout,Qt::AlignBottom);
    setCentralWidget(centralW);
    messageWidget->setMinimumWidth(1);
    connect(button1, SIGNAL(clicked(bool)), this, SLOT(addMessageClicked()));
    connect(button3, SIGNAL(clicked(bool)), this, SLOT(clearAllMessages()));
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(exitClicked()));
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(closeClicked()));
    QString randomUser = QString("justinfan")+QString::number(rand() % 9999999+1345531, 10);
    qDebug() << "RandomUser: " << randomUser;
    TwitchIRC * irc = new TwitchIRC("199.9.255.147", 443, "sodapoppin", randomUser, "oauth:k81x5y0q1ooo3cf5yepen2w5mrro7i");
    connect(irc, SIGNAL(messageReceived(int,QString,QString,QString,QString,QString)), this, SLOT(addMessage(int,QString,QString,QString,QString,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::addMessageWidget(){

    return true;
}

bool MainWindow::addMessage(int service, QString channel, QString nickname, QString colorHex, QString text, QString receiver){
    Message *msg = new Message(scrollarea, service, nickname, text, receiver, colorHex);
    if(messageLayout->count() >=500)
        this->clearOldMessages(100);
    messageLayout->addWidget(msg);
    _counterLCD->display(QString::number(messageLayout->count()));
    return true;
}

void MainWindow::loadWindowSettings()
{
    windowSettings = new Settings(QApplication::applicationDirPath());
    windowSettings->loadSettings(QString("QDChat_")+windowName);
    QRect desktopRect (QApplication::desktop()->screenGeometry());
    this->setGeometry(windowSettings->getValue("window", "x", desktopRect.width()/2 - this->width()/2).toInt(),
                      windowSettings->getValue("window", "y", desktopRect.height()/2 - this->height()/2).toInt(),
                      windowSettings->getValue("window", "width", 400).toInt(),
                      windowSettings->getValue("window", "height", 600).toInt());

}

void MainWindow::saveWindowSettings()
{
    this->windowSettings->setValue("window", "x", this->geometry().x());
    this->windowSettings->setValue("window", "y", this->geometry().y());
    this->windowSettings->setValue("window", "width", this->geometry().width());
    this->windowSettings->setValue("window", "height", this->geometry().height());
}


void MainWindow::paintEvent(QPaintEvent *)
{
    int maximumY = scrollarea->verticalScrollBar()->maximum();
    if(maximumY > lastScrollbarMaximum)
        scrollarea->verticalScrollBar()->setValue(maximumY);
    lastScrollbarMaximum = maximumY;

}

void MainWindow::resizeEvent(QResizeEvent *)
{
    saveWindowSettings();

}

void MainWindow::moveEvent(QMoveEvent *)
{
    saveWindowSettings();
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        _mousePos = ev->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton) {
            QPoint diff = event->pos() - _mousePos;
            QPoint newpos = this->pos() + diff;

            this->move(newpos);
    }
}


void MainWindow::addMessageClicked(){
    string alphabet = "abcde fghi jklmn opq rstuvwxyz ";
    QString nickname;
    QString text;
    QString toname;
    int service = 0;
    int rand_max = alphabet.length();
    for(int i=0; i < rand()%(10-3)+3; i++){
        nickname += QString(alphabet.at(rand()%(rand_max-0)+0));
    }
    for(int i=0; i < rand()% 4000+500; i++){
        text += QString(alphabet.at(rand()%(rand_max-0)+0));
    }
    for(int i=0; i < rand()%(10-3)+3; i++){
        toname += QString(alphabet.at(rand()%(rand_max-0)+0));
    }

    addMessage(service, "channel",nickname,"",text,"");
}

void MainWindow::clearAllMessages()
{
    this->clearOldMessages(messageLayout->count());

    _counterLCD->display(QString::number(messageLayout->count()));

}

void MainWindow::clearOldMessages(int n)
{
    if ( messageLayout != NULL )
    {
        QLayoutItem* item;
        for (int i=1; i<n; i++ )
        {
            if((item = messageLayout->takeAt(1)) != NULL){
                delete item->widget();
                delete item;
            }
        }
    }
}

void MainWindow::exitClicked()
{
    QApplication::quit();
}

void MainWindow::closeClicked()
{
    this->close();
}

