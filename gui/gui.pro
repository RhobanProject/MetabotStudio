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
    ValueWidget.cpp

HEADERS  += MainWindow.h \
    ComponentWizard.h \
    ComponentItem.h \
    Viewer.h \
    ParameterWidget.h \
    ObjectsTree.h \
    ZerosEditor.h \
    DynamicsWindow.h \
    ParametersEditor.h \
    ValueWidget.h

FORMS    += MainWindow.ui \
    ComponentWizard.ui \
    ComponentItem.ui \
    ZerosEditor.ui \
    DynamicsWindow.ui \
    ParametersEditor.ui

DEFINES += OPENGL
CONFIG += c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/json/lib/ -llibmetabot -ljson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/json/lib/ -llibmetabot -ljson
else:symbian: LIBS += -llibmetabot -ljson
else:unix: LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/-L$$PWD/../metabot/build/json/lib -llibmetabot -ljson -lsymbolic
unix: LIBS += -L$$PWD/../metabot/build/bullet/src/BulletCollision
unix: LIBS += -L$$PWD/../metabot/build/bullet/src/BulletDynamics
unix: LIBS += -L$$PWD/../metabot/build/bullet/src/LinearMath

INCLUDEPATH += $$PWD/../ $$PWD/../../bullet3/src/ $$PWD/../metabot/ $$PWD/../metabot/json/include/ $$PWD/../metabot/sym/ $$PWD/../metabot/scad/ $$PWD/../metabot/util/
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../metabot/build/libmetabot.lib $$PWD/../metabot/build/json/lib/libjson.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../metabot/build/libmetabot.lib $$PWD/../metabot/build/json/lib/libjson.a
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../metabot/build/liblibmetabot.a $$PWD/../metabot/build/json/lib/libjson.a

win32: LIBS += -lglu32 -lBulletDynamics -lBulletCollision -lLinearMath -lzmq
mac: LIBS += -framework OpenGL -lzmq
else:unix: LIBS += -lGLU -lBulletDynamics -lBulletCollision -lLinearMath -lzmq

RESOURCES += \
    icons.qrc
