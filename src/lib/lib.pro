TEMPLATE = lib
TARGET = pokqt

QT = core network

DEFINES += POKQT_LIBRARY

HEADERS += pokqt_global.h

include(network/network.pri)
include(logic/logic.pri)
