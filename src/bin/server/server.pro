TEMPLATE = app
TARGET = pokqt-server

QT = core gui network widgets
INCLUDEPATH+=../../lib/
LIBS+=-L../../lib -lpokqt

SOURCES += \
    main.cpp \
    serverdialog.cpp

FORMS += \
    serverdialog.ui

HEADERS += \
    serverdialog.h
