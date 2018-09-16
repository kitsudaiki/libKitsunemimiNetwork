#-------------------------------------------------
#
# Project created by QtCreator 2018-03-24T15:59:28
#
#-------------------------------------------------

QT       -= core gui

TARGET = KitsuneNetwork
TEMPLATE = lib
CONFIG += c++17

LIBS += -L../libKitsuneCommon -lKitsuneCommon
LIBS += -L../libKitsuneCommon/debug -lKitsuneCommon
LIBS += -L../libKitsuneCommon/release -lKitsuneCommon
INCLUDEPATH += ../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD \
            include/libKitsuneNetwork

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    include/libKitsuneNetwork/tcp/tcpServer.h \
    include/libKitsuneNetwork/tcp/tcpClient.h \
    tests/tcpTest.h

SOURCES += \
    src/tcp/tcpServer.cpp \
    src/tcp/tcpClient.cpp \
    tests/tcpTest.cpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}
