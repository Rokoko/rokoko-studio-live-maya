QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rslm

win32 {
    include(maya_qtconfig)
}
macx {
    include(maya_qtconfig_macos)
}
unix:!macx {
    include(maya_qtconfig_linux)
}

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/ui/button.cpp \
    src/ui/categoryheader.cpp \
    src/ui/commandapicontent.cpp \
    src/ui/infocontent.cpp \
    src/ui/receivercontent.cpp \
    src/ui/recordbutton.cpp \
    src/ui/rootwidget.cpp

HEADERS += \
    src/constants.h \
    src/ui/button.h \
    src/ui/categoryheader.h \
    src/ui/commandapicontent.h \
    src/ui/infocontent.h \
    src/ui/receivercontent.h \
    src/ui/recordbutton.h \
    src/ui/rootwidget.h

INCLUDEPATH += "$$PWD/src"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
