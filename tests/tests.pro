include(../defaults.pri)

QT       -= qt core gui

CONFIG -= app_bundle
CONFIG += c++14 console

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

LIBS += -L../../libKitsunePersistence/src -lKitsunePersistence
LIBS += -L../../libKitsunePersistence/src/debug -lKitsunePersistence
LIBS += -L../../libKitsunePersistence/src/release -lKitsunePersistence
INCLUDEPATH += ../../libKitsunePersistence/include/libKitsunePersistence

LIBS +=  -lboost_filesystem -lboost_system

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

LIBS += -L../src -lKitsuneNetwork

HEADERS += \
    dummy_buffer.h \
    cert_init.h \
    libKitsuneNetwork/tcp/tcp_socket_tcp_server_test.h \
    libKitsuneNetwork/tls_tcp/tls_tcp_socket_tls_tcp_server_test.h \
    libKitsuneNetwork/unix/unix_socket_unix_server_test.h

SOURCES += \
    main.cpp \
    dummy_buffer.cpp \
    libKitsuneNetwork/tcp/tcp_socket_tcp_server_test.cpp \
    libKitsuneNetwork/tls_tcp/tls_tcp_socket_tls_tcp_server_test.cpp \
    libKitsuneNetwork/unix/unix_socket_unix_server_test.cpp
