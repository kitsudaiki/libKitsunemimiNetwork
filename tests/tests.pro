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

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

LIBS += -L../src -lKitsuneNetwork

HEADERS += \
    libKitsuneNetwork/tcp/tcp_client_tcp_server_test.h \
    dummy_buffer.h \
    libKitsuneNetwork/unix/unix_client_unix_server_test.h \
    libKitsuneNetwork/tls_tcp/tls_tcp_client_tls_tcp_server_test.h \
    cert_init.h

SOURCES += \
    main.cpp \
    dummy_buffer.cpp \
    libKitsuneNetwork/tcp/tcp_client_tcp_server_test.cpp \
    libKitsuneNetwork/unix/unix_client_unix_server_test.cpp \
    libKitsuneNetwork/tls_tcp/tls_tcp_client_tls_tcp_server_test.cpp
