TEMPLATE = app
TARGET = pokqt-test

QT = core gui network quick
INCLUDEPATH+=../../lib/network
LIBS+=-L../../lib/network -lpokqtnetwork

SOURCES += \
    main.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    main.qml
