QT       -= qt core gui

TARGET = KitsuneNetwork
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.2.0

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

HEADERS += \
    ../include/libKitsuneNetwork/tcp/tcp_client.h \
    ../include/libKitsuneNetwork/tcp/tcp_server.h \
    ../include/libKitsuneNetwork/network_trigger.h \
    ../include/libKitsuneNetwork/message_ring_buffer.h \
    cleanup_thread.h \
    ../include/libKitsuneNetwork/abstract_client.h \
    ../include/libKitsuneNetwork/abstract_server.h \
    ../include/libKitsuneNetwork/unix/unix_server.h \
    ../include/libKitsuneNetwork/unix/unix_client.h

SOURCES += \
    network_trigger.cpp \
    cleanup_thread.cpp \
    tcp/tcp_server.cpp \
    tcp/tcp_client.cpp \
    abstract_server.cpp \
    abstract_client.cpp \
    unix/unix_server.cpp \
    unix/unix_client.cpp
