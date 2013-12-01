QT += testlib

INCLUDEPATH=../../src/lib/

HEADERS += ../../src/lib/pokqt_global.h \
    ../../src/lib/logic/card.h \
    ../../src/lib/logic/hand.h

SOURCES += ../../src/lib/logic/card.cpp \
    ../../src/lib/logic/hand.cpp \
    tst_hand.cpp
