QT += widgets
QT += network
QT += webenginewidgets

INCLUDEPATH += ../SieloNavigateur
TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(TestPlugin)

LIBS += $$PWD/libSieloNavigateur.so.1.0.0

HEADERS += \
    TestPlugin.hpp

SOURCES += \
    TestPlugin.cpp
