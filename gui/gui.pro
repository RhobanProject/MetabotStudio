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
    ComponentWizard.cpp

HEADERS  += MainWindow.h \
    ComponentWizard.h

FORMS    += MainWindow.ui \
    ComponentWizard.ui

CONFIG += c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/json/lib/ -llibmetabot -ljson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/json/lib/ -llibmetabot -ljson
else:symbian: LIBS += -llibmetabot -ljson
else:unix: LIBS += -L$$PWD/../metabot/build/ -L$$PWD/../metabot/build/json/lib -llibmetabot -ljson

INCLUDEPATH += $$PWD/../ $$PWD/../metabot/
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../metabot/build/libmetabot.lib $$PWD/../metabot/build/json/lib/libjson.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../metabot/build/libmetabot.lib $$PWD/../metabot/build/json/lib/libjson.a
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../metabot/build/liblibmetabot.a $$PWD/../metabot/build/json/lib/libjson.a

win32: LIBS += -lglu32
mac: LIBS += -framework OpenGL
else:unix: LIBS += -lGLU
