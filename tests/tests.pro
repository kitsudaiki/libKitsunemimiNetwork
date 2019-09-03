include(../defaults.pri)

QT       -= qt core gui

CONFIG -= app_bundle
CONFIG += c++14 console

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneNetwork

LIBS += -L../src -lKitsuneNetwork

HEADERS += \
    libKitsuneNetwork/tcp/tcp_client_tcp_server_test.h \
    dummy_buffer.h \
    libKitsuneNetwork/unix/unix_client_unix_server_test.h

SOURCES += \
    main.cpp \
    dummy_buffer.cpp \
    libKitsuneNetwork/tcp/tcp_client_tcp_server_test.cpp \
    libKitsuneNetwork/unix/unix_client_unix_server_test.cpp
