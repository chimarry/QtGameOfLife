QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -Wno-deprecated
SOURCES += \
    ConwayGameOfLifeExecutor.cpp \
    ConwayMatrix.cpp \
    OpenCLConfiguration.cpp \
    imagechanger.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ConwayGameOfLifeExecutor.h \
    ConwayMatrix.h \
    OpenCLConfiguration.h \
    imagechanger.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/IntelSWTools/system_studio_2020/OpenCL/sdk/lib/x86_64/' -lOpenCL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/IntelSWTools/system_studio_2020/OpenCL/sdk/lib/x86_64/' -lOpenCL
else:unix: LIBS += -L$$PWD/'../../../../Program Files (x86)/IntelSWTools/system_studio_2020/OpenCL/sdk/lib/x86_64/' -lOpenCL

INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/IntelSWTools/system_studio_2020/OpenCL/sdk/include'
DEPENDPATH += $$PWD/'../../../../Program Files (x86)/IntelSWTools/system_studio_2020/OpenCL/sdk/include'

DISTFILES += \
    ConwayGameOfLife.cl
