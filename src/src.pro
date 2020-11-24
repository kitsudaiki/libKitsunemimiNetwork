QT       -= qt core gui

TARGET = KitsunemimiNetwork
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.6.4

LIBS += -L../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../libKitsunemimiCommon/include

LIBS += -L../../libKitsunemimiPersistence/src -lKitsunemimiPersistence
LIBS += -L../../libKitsunemimiPersistence/src/debug -lKitsunemimiPersistence
LIBS += -L../../libKitsunemimiPersistence/src/release -lKitsunemimiPersistence
INCLUDEPATH += ../../libKitsunemimiPersistence/include

LIBS +=  -lssl -lcrypt

INCLUDEPATH += $$PWD \
               $$PWD/../include

HEADERS += \
    ../include/libKitsunemimiNetwork/tcp/tcp_server.h \
    ../include/libKitsunemimiNetwork/message_ring_buffer.h \
    ../include/libKitsunemimiNetwork/abstract_server.h \
    ../include/libKitsunemimiNetwork/tls_tcp/tls_tcp_server.h \
    ../include/libKitsunemimiNetwork/tcp/tcp_socket.h \
    ../include/libKitsunemimiNetwork/tls_tcp/tls_tcp_socket.h \
    ../include/libKitsunemimiNetwork/abstract_socket.h \
    ../include/libKitsunemimiNetwork/unix/unix_domain_server.h \
    ../include/libKitsunemimiNetwork/unix/unix_domain_socket.h

SOURCES += \
    tcp/tcp_server.cpp \
    abstract_server.cpp \
    tls_tcp/tls_tcp_server.cpp \
    tcp/tcp_socket.cpp \
    tls_tcp/tls_tcp_socket.cpp \
    abstract_socket.cpp \
    unix/unix_domain_socket.cpp \
    unix/unix_domain_server.cpp
