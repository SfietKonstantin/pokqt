TEMPLATE = lib
TARGET = logic

CONFIG(c++11):DEFINES+=CPP11

QT = core

DEFINES += POKQTLOGIC_LIBRARY

HEADERS += pokqtlogic_global.h \
    card.h \
    deck.h


SOURCES += \
    card.cpp \
    deck.cpp
