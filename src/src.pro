QT       -= qt core gui

TARGET = KitsuneNetwork
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.3.0

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

LIBS += -L../../libKitsunePersistence/src -lKitsunePersistence
LIBS += -L../../libKitsunePersistence/src/debug -lKitsunePersistence
LIBS += -L../../libKitsunePersistence/src/release -lKitsunePersistence
INCLUDEPATH += ../../libKitsunePersistence/include/libKitsunePersistence

LIBS +=  -lssl -lcrypt

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

HEADERS += \
    ../include/libKitsuneNetwork/tcp/tcp_server.h \
    ../include/libKitsuneNetwork/network_trigger.h \
    ../include/libKitsuneNetwork/message_ring_buffer.h \
    cleanup_thread.h \
    ../include/libKitsuneNetwork/abstract_server.h \
    ../include/libKitsuneNetwork/unix/unix_server.h \
    ../include/libKitsuneNetwork/tls_tcp/tls_tcp_server.h \
    ../include/libKitsuneNetwork/tcp/tcp_socket.h \
    ../include/libKitsuneNetwork/tls_tcp/tls_tcp_socket.h \
    ../include/libKitsuneNetwork/unix/unix_socket.h \
    ../include/libKitsuneNetwork/abstract_socket.h

SOURCES += \
    network_trigger.cpp \
    cleanup_thread.cpp \
    tcp/tcp_server.cpp \
    abstract_server.cpp \
    unix/unix_server.cpp \
    tls_tcp/tls_tcp_server.cpp \
    tcp/tcp_socket.cpp \
    tls_tcp/tls_tcp_socket.cpp \
    unix/unix_socket.cpp \
    abstract_socket.cpp
