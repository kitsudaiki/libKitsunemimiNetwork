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
    dummyBuffer.hpp \
    libKitsuneNetwork/tcp/tcpClient_tcpServer_test.hpp

SOURCES += \
    main.cpp \
    dummyBuffer.cpp \
    libKitsuneNetwork/tcp/tcpClient_tcpServer_test.cpp
