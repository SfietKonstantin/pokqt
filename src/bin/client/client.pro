TEMPLATE = app
TARGET = pokqt-client

QT = core gui network quick
INCLUDEPATH+=../../lib/
LIBS+=-L../../lib/ -lpokqt

HEADERS += \
    playersmodel.h \
    clientbetmanager.h \
    handmodel.h \
    cardobject.h

SOURCES += \
    main.cpp \
    playersmodel.cpp \
    clientbetmanager.cpp \
    handmodel.cpp \
    cardobject.cpp

OTHER_FILES += \
    main.qml \
    ConnectDialog.qml \
    ChatPanel.qml \
    PlayerData.qml \
    UiConstants.js \
    GamingBoard.qml \
    Card.qml

RESOURCES += \
    res.qrc

