#include "mainwindow.h"
#include "Socket.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include "twitchirc.h"
#include "websocketserver.h"
#include <iostream>
#include <QtWidgets>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QObject>
#include <QMessageBox>
#include <QChar>
#include <QApplication>
#include <QURL>
#include <QWebEngineView>
//#include <QWebEngineSettings>
//#include <QWebEngineProfile>
//#include <QWebEngineHistory>
using namespace std;


MainWindow::MainWindow(QWidget *parent, QString windowName) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _twitchEmotes =  new QHash<QString, QString>();
    QFile styleFile(":/styles/css/DefaultWindowStyle.qss");
    styleFile.open(QFile::ReadOnly);
    QByteArray bytes = styleFile.readAll();
    styleFile.close();
    this->setStyleSheet(QString(bytes));
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_PaintOnScreen);
    initMenu();

    this->windowName = windowName;
    loadWindowSettings();
    centralW = new QWidget(this);
//    this->setWindowFlags(Qt::SubWindow);


    _messageCounter = 0;
//    messageWidget = new QWidget(centralW);
//    messageWidget->setObjectName("messagesPlaceholder");
    QWidget *bttnWidget = new QWidget(centralW);
    QGridLayout *gridLayout = new QGridLayout(centralW);
//    messageLayout = new QVBoxLayout();
    gridLayout->setContentsMargins(0,3,0,3);
//    gridLayout->setSpacing(0);
//    messageLayout->setContentsMargins(0,10,0,10);
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    QPushButton *button1 = new QPushButton("Add Random Message");
    QPushButton *button3 = new QPushButton("Clear ALL");
//    button1->setStyleSheet("background:white;");
//    button3->setStyleSheet("background:white;");
//    scrollarea = new QScrollArea();
//    scrollarea->setFrameShape(QFrame::NoFrame);
//    scrollarea->setWidget(messageWidget);
//    scrollarea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
//    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
//    messageLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
//    scrollarea->setWidgetResizable(true);
    centralW->setLayout(gridLayout);
//    messageWidget->setLayout(messageLayout);
//    messageWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
//    scrollarea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
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
//    _counterLCD->display(QString::number(messageLayout->count()));
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
//    messageLayout->insertStretch(-1);

    initWebView();
    this->setMinimumWidth(1);


    gridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    gridLayout->addWidget(_menuBar, 0, 0);
    gridLayout->addWidget(_webView, 1, 0);
    gridLayout->addWidget(bttnWidget, 2, 0);
    gridLayout->addWidget(new QSizeGrip(this), 3,0,1,0,Qt::AlignBottom | Qt::AlignLeft);
    gridLayout->addWidget(new QSizeGrip(this), 3,0,1,0, Qt::AlignBottom | Qt::AlignRight);

    gridLayout->setAlignment(messageLayout,Qt::AlignTop);
    gridLayout->setAlignment(buttonsLayout,Qt::AlignBottom);
    setCentralWidget(centralW);
//    messageWidget->setMinimumWidth(1);

    connect(button1, SIGNAL(clicked(bool)), this, SLOT(addMessageClicked()));
    connect(button3, SIGNAL(clicked(bool)), this, SLOT(clearAllMessages()));

    _wsServer = new WebSocketServer(1234, this);
}

MainWindow::~MainWindow()
{
    delete _webView;
    delete windowSettings;
    delete ui;
}

bool MainWindow::addMessageWidget(){

    return true;
}

bool MainWindow::addMessage(int service, QString channel, QString nickname, QString colorHex, QString text, QString receiver){
//    Message *msg = new Message(scrollarea, service, nickname, text, receiver, colorHex);
//    if(messageLayout->count() >=500)
//        this->clearOldMessages(100);
////    messageLayout->addWidget(msg);
//    _counterLCD->display(QString::number(messageLayout->count()));
    QString coloredNick = colorHex != ""? "<td style='color:"+colorHex+";'>"+nickname+"</td>" : "<td>"+nickname+"</td>";
    QString msg = "<table>"\
                "<tr>"\
                "<td><img id='serviceIcon' src='qrc:/resources/services/images/twitch.png'/></td>"\
                "<td id='channelName'>#"+channel+"</td>"\
                "<td id='nickname'style='color:"+colorHex+";text-align:right'>@"+nickname+":</td>"\
                "</tr>"\
                "<tr>"\
                "<td valign='top' colspan='3'><p valign='top' id='messageText'>"+text+"</p></td>"\
                "<td></td>"\
                "</tr>"\
             "</table>";

    _wsServer->sendMessage(msg);

    return true;
}

void MainWindow::twitchManagerClicked()
{
    QDialog * dial = new QDialog(this);
    QGridLayout *gridLayout= new QGridLayout(dial);
    dial->setLayout(gridLayout);

    QLineEdit *txtChannel = new QLineEdit(dial);
    txtChannel->setPlaceholderText("Twitch Channel");
    QLabel *lblChannel = new QLabel("Channel: ", dial);
    QPushButton *btnConnect = new QPushButton("Connect", dial);
    gridLayout->addWidget(lblChannel, 0, 0, Qt::AlignTop| Qt::AlignLeft);
    gridLayout->addWidget(txtChannel, 0, 1, Qt::AlignTop | Qt::AlignLeft);
    gridLayout->addWidget(btnConnect, 1, 0, 1, 2, Qt::AlignTop | Qt::AlignCenter);
    dial->show();
    connect(txtChannel, &QLineEdit::textChanged, this, &MainWindow::channelChangedOnAdding);
    connect(btnConnect, &QPushButton::clicked, this, &MainWindow::connectToTwitch);
    connect(btnConnect, &QPushButton::clicked, dial, &QDialog::close);
}

void MainWindow::channelChangedOnAdding(const QString &text)
{
    _newChannel = text;
}

void MainWindow::connectToTwitch()
{
    QString randomUser = QString("justinfan")+QString::number(rand() % 9999999+1345531, 10);
//    qDebug() << "RandomUser: " << randomUser;
    TwitchIRC * twitch = new TwitchIRC("199.9.255.147", 443, _newChannel, randomUser, "someTokenPassBlaBlas");
    if(_twitchEmotes->keys().length() > 0){
        twitch->setHashEmotes(_twitchEmotes);
    }
    else{
        twitch->loadTwitchEmotes();
        _twitchEmotes = twitch->getHashEmotes();
    }

    connect(twitch, SIGNAL(messageReceived(int,QString,QString,QString,QString,QString)), this, SLOT(addMessage(int,QString,QString,QString,QString,QString)));
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

void MainWindow::initMenu()
{
    _menuBar = new QMenuBar(this);
    QMenu *main = _menuBar->addMenu("QDChat");

    QAction *closeAction = main->addAction("Close Window");
    QAction *exitAction = main->addAction("Exit");

    QMenu *services = _menuBar->addMenu("Services");
    QAction *manageTwitchAction = services->addAction("Twitch Channels");

    QMenu *options = _menuBar->addMenu("Options");
    QAction *settings = options->addAction("QDChat Settings");

    connect(manageTwitchAction, &QAction::triggered, this, &MainWindow::twitchManagerClicked);
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(exitClicked()));
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(closeClicked()));

    _menuBar->adjustSize();
    _menuBar->setMaximumWidth(_menuBar->width());
}

void MainWindow::initWebView()
{
    _webView = new QWebEngineView(this);
    _webView->setUrl(QUrl("file:///E:/qtproj/qChat/index.html"));
    _webView->page()->setBackgroundColor(Qt::transparent);
    _webView->setAttribute(Qt::WA_TranslucentBackground);
    _webView->setMinimumWidth(1);
}


void MainWindow::paintEvent(QPaintEvent *)
{
//    if(this->isVisible()){
////    int maximumY = scrollarea->verticalScrollBar()->maximum();
////    if(maximumY > lastScrollbarMaximum)
////        scrollarea->verticalScrollBar()->setValue(maximumY);
////    lastScrollbarMaximum = maximumY;
//    }

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

    addMessage(service, "QDChat_Test",nickname,"#ffffff",text,"Receiver");
}

void MainWindow::clearAllMessages()
{
    initWebView();
//    _counterLCD->display(QString::number(messageLayout->count()));

}

void MainWindow::clearOldMessages(int n)
{
//    if ( messageLayout != NULL )
//    {
//        QLayoutItem* item;
//        for (int i=1; i<n; i++ )
//        {
//            if((item = messageLayout->takeAt(1)) != NULL){
//                delete item->widget();
//                delete item;
//            }
//        }
//    }
}

void MainWindow::exitClicked()
{
    QApplication::quit();
}

void MainWindow::closeClicked()
{
    this->close();
}

