QT += core gui widgets serialport opengl

TARGET = MyoArduino_GUI
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    openglwindow.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    include/AntTweakBar.h \
    main.h \
    openglwindow.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    MyoArduino_GUI.qrc

target.path = $$PWD/
INSTALLS += target

CONFIG += static

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/include/ -lAntTweakBar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/ -lAntTweakBar
else:unix: LIBS += -L$$PWD/include/ -lAntTweakBar

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
