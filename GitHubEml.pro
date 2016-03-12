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
    src/mainwindow.cpp \
    src/emlmessage.cpp \
    src/eml.cpp \
    src/emlsslsocket.cpp \
    src/smtp.cpp \
    src/main.cpp \
    src/emlheader.cpp \
    ../JsonSave/account.cpp \
    src/smtpsslsocket.cpp


HEADERS  += ./include/mainwindow.h \
    include/emlmessage.h \
    include/eml.h \
    include/emlsslsocket.h \
    include/smtp.h \
    include/emlheader.h \
    ../JsonSave/account.h \
    include/smtpsslsocket.h


FORMS    += ui/mainwindow.ui


DISTFILES +=

RESOURCES += \
    ./resource/rsource.qrc

INCLUDEPATH += include \
   ../JsonSave



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/release/ -lJsonSave
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/debug/ -lJsonSave

INCLUDEPATH += $$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/debug
DEPENDPATH += $$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/debug

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/release/libJsonSave.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/debug/libJsonSave.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/release/JsonSave.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../build/subproj/Qt_5_5_1_mingw492_32/JsonSave/debug/JsonSave.lib
