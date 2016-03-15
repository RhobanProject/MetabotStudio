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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/json/lib/ -llibmetabot -ljsoncpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/json/lib/ -llibmetabot -ljsoncpp
else:symbian: LIBS += -llibmetabot -ljson
else:unix: LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/-L$$PWD/../metabot/build/json/lib -llibmetabot -ljsoncpp -lsymbolic
unix: LIBS += -L$$PWD/../metabot/build/bullet/src/BulletCollision
unix: LIBS += -L$$PWD/../metabot/build/bullet/src/BulletDynamics
unix: LIBS += -L$$PWD/../metabot/build/bullet/src/LinearMath

INCLUDEPATH += $$PWD/../ $$PWD/../metabot/ $$PWD/../metabot/sym/ $$PWD/../metabot/scad/ $$PWD/../metabot/util/
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../metabot/build/libmetabot.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../metabot/build/libmetabot.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../metabot/build/liblibmetabot.a

win32: LIBS += -lglu32 -lBulletDynamics -lBulletCollision -lLinearMath -lzmq -lrhoban_jsoncpp
mac: LIBS += -framework OpenGL -lzmq -lrhoban_jsoncpp
else:unix: LIBS += -lGLU -lBulletDynamics -lBulletCollision -lLinearMath -lzmq -lrhoban_jsoncpp

RESOURCES += \
    icons.qrc
