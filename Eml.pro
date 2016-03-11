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
        ./src/mainwindow.cpp \
    ./src/emlmessage.cpp \
    ./src/eml.cpp \
    ./src/emlsslsocket.cpp \
    ./src/smtp.cpp \
    ./src/main.cpp \
    ./src/emlheader.cpp \
    ../JsonSave/account.cpp


HEADERS  += ./include/mainwindow.h \
    ./include/emlmessage.h \
    ./include/eml.h \
    ./include/emlsslsocket.h \
    ./include/smtp.h \
    ./include/emlheader.h \
    ../JsonSave/account.h


FORMS    += ./ui/mainwindow.ui


DISTFILES +=

RESOURCES += \
    ./resource/rsource.qrc

INCLUDEPATH += ./include \
   ../JsonSave
