#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T10:59:43
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Metabot
TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    ComponentWizard.cpp \
    ComponentItem.cpp \
    Viewer.cpp \
    ParameterWidget.cpp \
    ObjectsTree.cpp \
    ZerosEditor.cpp \
    DynamicsWindow.cpp \
    ParametersEditor.cpp \
    ValueWidget.cpp \
    BackendSelector.cpp

HEADERS  += MainWindow.h \
    ComponentWizard.h \
    ComponentItem.h \
    Viewer.h \
    ParameterWidget.h \
    ObjectsTree.h \
    ZerosEditor.h \
    DynamicsWindow.h \
    ParametersEditor.h \
    ValueWidget.h \
    BackendSelector.h

FORMS    += MainWindow.ui \
    ComponentWizard.ui \
    ComponentItem.ui \
    ZerosEditor.ui \
    DynamicsWindow.ui \
    ParametersEditor.ui \
    BackendSelector.ui

DEFINES += OPENGL BT_USE_DOUBLE_PRECISION
CONFIG += c++11

METABOT_BUILD=$$PWD/../metabot/build/
win32:CONFIG(release, debug|release): LIBS += -L$$METABOT_BUILD -llibmetabot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$METABOT_BUILD -llibmetabot
else:symbian: LIBS += -llibmetabot
else:unix: LIBS += -L$$METABOT_BUILD -llibmetabot
QMAKE_RPATHDIR += $$METABOT_BUILD

LIBS += $$system(php $HOME/deps/deps.php cflags rhobandeps/bullet3 rhobandeps/symbolic)

INCLUDEPATH += $$PWD/../ $$PWD/../metabot/ $$PWD/../metabot/sym/ $$PWD/../metabot/scad/ $$PWD/../metabot/util/
DEPENDPATH += $$PWD/../

win32: LIBS += -lglu32
mac: LIBS += -framework OpenGL
else:unix: LIBS += -lGLU 

RESOURCES += \
    icons.qrc
