#-------------------------------------------------
#
# Project created by QtCreator 2018-11-27T15:03:53
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Powd3r-Slicer
TEMPLATE = app
LIBS += -LD:\Qt\5.11.2\mingw53_32\lib\libQt5OpenGL.a -lopengl32 -lglu32
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        src/boundingbox.cpp \
        src/geometry.cpp \
        src/glModel.cpp \
        src/glPath.cpp \
        src/glSlice.cpp \
        src/mainwindow.cpp \
        src/mesh.cpp \
        src/meshloader.cpp \
        src/movedialog.cpp \
        src/rotdialog.cpp \
        src/scaledialog.cpp \
        src/sweep.cpp \
        src/glGcode.cpp

HEADERS += \
        src/geometry.h \
        src/glModel.h \
        src/glPath.h \
        src/glSlice.h \
        src/mainwindow.h \
        src/mesh.h \
        src/movedialog.h \
        src/rotdialog.h \
        src/scaledialog.h \
        src/sweep.h \
        src/glGcode.h

FORMS += \
    ui/mainwindow.ui \
    ui/movedialog.ui \
    ui/rotdialog.ui \
    ui/scaledialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressource.qrc

DISTFILES += \
    .gitignore \
    README.md
