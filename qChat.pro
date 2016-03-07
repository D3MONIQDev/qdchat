#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T21:50:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qChat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    message.cpp \
    settings.cpp \
    messageitem.cpp \
    helper.cpp \
    Socket.cpp \
    twitchirc.cpp

HEADERS  += mainwindow.h \
    message.h \
    settings.h \
    messageitem.h \
    helper.h \
    Socket.h \
    twitchirc.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
