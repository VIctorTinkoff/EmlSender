#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T14:26:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Eml
TEMPLATE = app


SOURCES +=\
        mainwindow.cpp \
    emlmessage.cpp \
    eml.cpp \
    emlsocket.cpp \
    emlsslsocket.cpp \
    smtp.cpp \
    main.cpp

HEADERS  += mainwindow.h \
    emlmessage.h \
    eml.h \
    emlsocket.h \
    emlsslsocket.h \
    smtp.h

FORMS    += mainwindow.ui
