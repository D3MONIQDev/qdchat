#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "settings.h"

#include <QVBoxLayout>
#include <QMainWindow>
#include <QString>
#include <message.h>
#include <QScrollArea>
#include <QLCDNumber>

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
    void loadWindowSettings();
    void saveWindowSettings();
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
private slots:
    void addMessageClicked();
    void clearAllMessages();
    void clearOldMessages(int n);
    void exitClicked();
    void closeClicked();
    bool addMessage(int service, QString channel, QString nickname, QString colorHex, QString text, QString receiver ="");
signals:
    void signalScrollbarShown();
};

#endif // MAINWINDOW_H
