QT += testlib

win32:DEFINES += POKQT_LIBRARY

INCLUDEPATH=../../src/lib/

HEADERS += ../../src/lib/pokqt_global.h \
    ../../src/lib/logic/card.h

SOURCES += ../../src/lib/logic/card.cpp \
    tst_card.cpp
