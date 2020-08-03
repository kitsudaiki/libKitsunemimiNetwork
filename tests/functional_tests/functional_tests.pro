include(../../defaults.pri)

QT       -= qt core gui

CONFIG -= app_bundle
CONFIG += c++14 console

LIBS += -L../../src -lKitsunemimiNetwork
INCLUDEPATH += $$PWD

LIBS += -L../../../libKitsunemimiPersistence/src -lKitsunemimiPersistence
LIBS += -L../../../libKitsunemimiPersistence/src/debug -lKitsunemimiPersistence
LIBS += -L../../../libKitsunemimiPersistence/src/release -lKitsunemimiPersistence
INCLUDEPATH += ../../../libKitsunemimiPersistence/include

LIBS += -L../../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../../libKitsunemimiCommon/include

LIBS +=  -lboost_filesystem -lboost_system -lssl -lcrypt

HEADERS += \
    cert_init.h \
    libKitsunemimiNetwork/tcp/tcp_socket_tcp_server_test.h \
    libKitsunemimiNetwork/tls_tcp/tls_tcp_socket_tls_tcp_server_test.h \
    libKitsunemimiNetwork/unix/unix_domain_socket_unix_domain_server_test.h

SOURCES += \
    main.cpp \
    libKitsunemimiNetwork/tcp/tcp_socket_tcp_server_test.cpp \
    libKitsunemimiNetwork/tls_tcp/tls_tcp_socket_tls_tcp_server_test.cpp \
    libKitsunemimiNetwork/unix/unix_domain_socket_unix_domain_server_test.cpp
