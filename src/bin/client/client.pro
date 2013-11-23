TEMPLATE = app
TARGET = pokqt-client

QT = core gui network quick
INCLUDEPATH+=../../lib/
LIBS+=-L../../lib/ -lpokqt

SOURCES += \
    main.cpp \
    playersmodel.cpp

OTHER_FILES += \
    main.qml \
    ConnectDialog.qml \
    ChatPanel.qml

RESOURCES += \
    res.qrc

HEADERS += \
    playersmodel.h
