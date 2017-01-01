QT += widgets
QT += network

INCLUDEPATH += "../SieloNavigateur"

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(TestPlugin)

HEADERS += \
    TestPlugin.hpp

SOURCES += \
    TestPlugin.cpp
