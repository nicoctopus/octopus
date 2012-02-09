#-------------------------------------------------
#
# Project created by QtCreator 2012-01-25T17:01:35
#
#-------------------------------------------------

QT       += core gui
TARGET = InterfaceModel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    model/utils.cpp \
    model/serverosc.cpp \
    model/samplevideo.cpp \
    model/sampleaudio.cpp \
    model/sample.cpp \
    model/position.cpp \
    model/port.cpp \
    model/outputable.cpp \
    model/movement.cpp \
    model/messagesynapse.cpp \
    model/managerjoints.cpp \
    model/managerelements.cpp \
    model/manager.cpp \
    model/jointmvt.cpp \
    model/joint.cpp \
    model/inputable.cpp \
    model/element.cpp \
    model/databitg.cpp \
    model/controller.cpp \
    model/clientosc.cpp \
    view/triangle.cpp \
    view/square.cpp \
    view/mylefttree.cpp \
    view/ellipse.cpp \
    view/diamond.cpp \
    view/blackboard.cpp \
    model/soundplayer.cpp \
    view/jointgraphic.cpp \
    view/stickman.cpp \
    model/analyse.cpp

HEADERS  += mainwindow.h \
    model/utils.h \
    model/udp.hh \
    model/serverosc.h \
    model/samplevideo.h \
    model/sampleaudio.h \
    model/sample.h \
    model/position.h \
    model/port.h \
    model/outputable.h \
    model/oscpkt.hh \
    model/movement.h \
    model/messagesynapse.h \
    model/managerjoints.h \
    model/managerelements.h \
    model/manager.h \
    model/jointmvt.h \
    model/joint.h \
    model/inputable.h \
    model/element.h \
    model/databitg.h \
    model/controller.h \
    model/clientosc.h \
    view/triangle.h \
    view/stickman.h \
    view/square.h \
    view/mylefttree.h \
    view/ellipse.h \
    view/diamond.h \
    view/blackboard.h \
    model/soundplayer.h \
    FMOD/api/inc/fmod.hpp \
    FMOD/api/inc/fmod.h \
    FMOD/api/inc/fmod_output.h \
    FMOD/api/inc/fmod_memoryinfo.h \
    FMOD/api/inc/fmod_errors.h \
    FMOD/api/inc/fmod_dsp.h \
    FMOD/api/inc/fmod_codec.h \
    view/jointgraphic.h \
    model/analyse.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    FMOD/api/lib/libfmodex.dylib

RESOURCES += \
    ressources.qrc











win32:CONFIG(release, debug|release): LIBS += -L$$PWD/FMOD/api/lib/release/ -lfmodex
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/FMOD/api/lib/debug/ -lfmodex
else:macx: LIBS += -L$$PWD/FMOD/api/lib/ -lfmodex

INCLUDEPATH += $$PWD/FMOD/api/inc
DEPENDPATH += $$PWD/FMOD/api/inc






