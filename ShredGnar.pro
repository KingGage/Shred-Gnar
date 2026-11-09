QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appmodel.cpp \
    endscreen.cpp \
    gamemodel.cpp \
    gameobject.cpp \
    gamescreen.cpp \
    headhomedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    pointsearnedscreen.cpp \
    ski.cpp \
    skishopscreen.cpp \
    startscreen.cpp \
    terminologyscreen.cpp \
    tutorialscreen.cpp \
    weathertutorialscreen.cpp

HEADERS += \
    appmodel.h \
    endscreen.h \
    gamemodel.h \
    gameobject.h \
    gamescreen.h \
    headhomedialog.h \
    mainwindow.h \
    pointsearnedscreen.h \
    ski.h \
    skishopscreen.h \
    startscreen.h \
    terminologyscreen.h \
    tutorialscreen.h \
    weathertutorialscreen.h

FORMS += \
    endscreen.ui \
    gamescreen.ui \
    headhomedialog.ui \
    mainwindow.ui \
    pointsearnedscreen.ui \
    skishopscreen.ui \
    startscreen.ui \
    terminologyscreen.ui \
    tutorialscreen.ui \
    weathertutorialscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ShredGnarResources.qrc
