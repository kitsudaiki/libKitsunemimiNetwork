QT       -= qt core gui

TARGET = KitsuneNetwork
TEMPLATE = lib
# CONFIG += staticlib
CONFIG += c++14

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

HEADERS += \
    ../include/libKitsuneNetwork/tcp/tcpServer.h \
    ../include/libKitsuneNetwork/tcp/tcpClient.h \
    ../include/libKitsuneNetwork/networkTrigger.h \
    ../include/libKitsuneNetwork/cleanupThread.h \
    ../include/libKitsuneNetwork/messageringbuffer.h

SOURCES += \
    tcp/tcpServer.cpp \
    tcp/tcpClient.cpp \
    networkTrigger.cpp \
    cleanupThread.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
