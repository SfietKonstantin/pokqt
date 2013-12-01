TEMPLATE = app
TARGET = pokqt-server

QT = core gui network widgets
INCLUDEPATH+=../../lib/
unix: LIBS+=-L../../lib/ -lpokqt
win32: LIBS+=-L../../lib/release -lpokqt

SOURCES += \
    main.cpp \
    serverdialog.cpp

FORMS += \
    serverdialog.ui

HEADERS += \
    serverdialog.h
