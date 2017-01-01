QT += widgets
QT += network
QT += webenginewidgets

INCLUDEPATH += ../SieloNavigateur
TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(TestPlugin)

LIBS += /home/feldrise/Documents/libSieloNavigateur.so.1

HEADERS += \
    TestPlugin.hpp

SOURCES += \
    TestPlugin.cpp
