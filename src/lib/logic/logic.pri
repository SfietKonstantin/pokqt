CONFIG(c++11):DEFINES+=CPP11

HEADERS += $$PWD/card.h \
    $$PWD/deck.h \
    $$PWD/playerproperties.h \
    $$PWD/gamemanager.h \
    logic/hand.h

SOURCES += $$PWD/card.cpp \
    $$PWD/deck.cpp \
    $$PWD/playerproperties.cpp \
    $$PWD/gamemanager.cpp \
    logic/hand.cpp
