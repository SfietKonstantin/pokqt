TEMPLATE = lib
TARGET = pokqtnetwork

QT = core

DEFINES += POKQTNETWORK_LIBRARY

HEADERS += pokqtnetwork_global.h \
    networkserver.h \
    networkclient.h \
    playerproperties.h

SOURCES += \
    networkserver.cpp \
    networkclient.cpp \
    playerproperties.cpp
