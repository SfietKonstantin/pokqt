TEMPLATE = lib
TARGET = pokqtnetwork

QT = core

DEFINES += POKQTNETWORK_LIBRARY

HEADERS += pokqtnetwork_global.h \
    osignal.h \
    networkserver.h \
    networkclient.h \
    playerproperties.h \
    connectionmanager.h

SOURCES += \
    networkserver.cpp \
    networkclient.cpp \
    playerproperties.cpp \
    connectionmanager.cpp
