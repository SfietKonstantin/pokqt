TEMPLATE = lib
TARGET = pokqtnetwork

QT = core

DEFINES += POKQTNETWORK_LIBRARY

HEADERS += pokqtnetwork_global.h \
    osignal.h \
    networkserver.h \
    playerproperties.h \
    connectionmanager.h \
    helpers.h

SOURCES += \
    networkserver.cpp \
    playerproperties.cpp \
    connectionmanager.cpp
