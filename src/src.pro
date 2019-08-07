QT       -= qt core gui

TARGET = KitsuneNetwork
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.1.0

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

HEADERS += \
    ../include/libKitsuneNetwork/tcp/tcpClient.hpp \
    ../include/libKitsuneNetwork/tcp/tcpServer.hpp \
    ../include/libKitsuneNetwork/cleanupThread.hpp \
    ../include/libKitsuneNetwork/networkTrigger.hpp \
    ../include/libKitsuneNetwork/messageRingBuffer.hpp

SOURCES += \
    tcp/tcpServer.cpp \
    tcp/tcpClient.cpp \
    networkTrigger.cpp \
    cleanupThread.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
