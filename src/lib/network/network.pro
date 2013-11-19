TEMPLATE = lib
TARGET = pokqtnetwork

QT = core

DEFINES += POKQTNETWORK_LIBRARY

HEADERS += pokqtnetwork_global.h \
    osignal.h \
    playerproperties.h \
    networkserver.h \
    networkclient.h \
    helpers.h

SOURCES += \
    playerproperties.cpp \
    networkserver.cpp \
    networkclient.cpp
