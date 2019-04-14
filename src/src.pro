QT       -= qt core gui

TARGET = KitsuneNetwork
TEMPLATE = lib
# CONFIG += staticlib
CONFIG += c++14

LIBS += -L../../libKitsuneCommon -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

HEADERS += \
    ../include/libKitsuneNetwork/tcp/tcpServer.h \
    ../include/libKitsuneNetwork/tcp/tcpClient.h \
    ../include/libKitsuneNetwork/networkTrigger.h

SOURCES += \
    tcp/tcpServer.cpp \
    tcp/tcpClient.cpp \
    networkTrigger.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
