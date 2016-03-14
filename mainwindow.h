#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "settings.h"
#include "websocketserver.h"
#include <QVBoxLayout>
#include <QMainWindow>
#include <QString>
#include <QScrollArea>
#include <QLCDNumber>
#include <QLineEdit>
#include <QWebEngineView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, QString windowName);
    ~MainWindow();
    bool addMessageWidget();

private:
    Ui::MainWindow *ui;
    QWidget *messageWidget;
    QWidget *centralW;
    QVBoxLayout *messageLayout;
    QScrollArea *scrollarea;
    QString windowName;
    Settings *windowSettings;
    int lastScrollbarMaximum;
    QPoint _mousePos;
    QThread * _cleanThread;
    int _messageCounter;
    QLCDNumber * _counterLCD;
    WebSocketServer *_wsServer;
    QMenuBar *_menuBar;
    QString _newChannel;
    QWebEngineView * _webView;
    QHash<QString, QString> *_twitchEmotes;
    void loadWindowSettings();
    void saveWindowSettings();
    void initMenu();
    void initWebView();
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

protected Q_SLOTS:
    void connectToTwitch();
    void addMessageClicked();
    void clearAllMessages();
    void clearOldMessages(int n);
    void exitClicked();
    void closeClicked();
    bool addMessage(int service, QString channel, QString nickname, QString colorHex, QString text, QString receiver ="");
    void twitchManagerClicked();
    void channelChangedOnAdding(const QString &text);
Q_SIGNALS:
    void signalScrollbarShown();
};

#endif // MAINWINDOW_H
