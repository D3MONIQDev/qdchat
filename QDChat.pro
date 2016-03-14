#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T21:50:56
#
#-------------------------------------------------

QT  += core gui network webenginewidgets websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QDChat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    helper.cpp \
    Socket.cpp \
    twitchirc.cpp \
    chatwebview.cpp \
    websocketserver.cpp \
    services.cpp

HEADERS  += mainwindow.h \
    settings.h \
    helper.h \
    Socket.h \
    twitchirc.h \
    chatwebview.h \
    websocketserver.h \
    services.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    README.md \
    index.html \
    css/default.css
