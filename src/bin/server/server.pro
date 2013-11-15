TEMPLATE = app
TARGET = pokqt-server

QT = core gui network widgets
INCLUDEPATH+=../../lib/network
LIBS+=-L../../lib/network -lpokqtnetwork

SOURCES += \
    main.cpp \
    serverdialog.cpp

FORMS += \
    serverdialog.ui

HEADERS += \
    serverdialog.h
