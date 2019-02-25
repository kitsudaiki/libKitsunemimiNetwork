include(../defaults.pri)

QT       -= qt core gui

CONFIG -= app_bundle
CONFIG += c++14 console

LIBS += -L../../libKitsuneCommon -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD \
            $$PWD/../include/libKitsuneNetwork

LIBS += -L../src -lKitsuneNetwork

HEADERS += \
    tcp/tcpTest.h

SOURCES += \
    tcp/tcpTest.cpp \
    main.cpp
