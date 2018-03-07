#-------------------------------------------------
#
# Project created by QtCreator 2017-02-13T14:54:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ProfileMultiDraw
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    plot/qcustomplot.cpp \
    profiledata.cpp \
    frame.cpp \
    profileinfoblock.cpp \
    profilewindow.cpp \
    settingsdialog.cpp \
    network.cpp \
    profilegenerator.cpp \
    neuron.cpp \
    neuron2i.cpp \
    neuronni.cpp \
    networkworkplace.cpp

HEADERS  += mainwindow.h \
    plot/qcustomplot.h \
    profiledata.h \
    frame.h \
    profileinfoblock.h \
    profilewindow.h \
    settingsdialog.h \
    network.h \
    profilegenerator.h \
    neuron.h \
    neuron2i.h \
    neuronni.h \
    networkworkplace.h

FORMS    += mainwindow.ui \
    profileinfoblock.ui \
    profilewindow.ui \
    settingsdialog.ui


INCLUDEPATH += "C:/Librarys/WaveletSpectrum/include"
LIBS += "C:/Librarys/WaveletSpectrum/lib/libWaveletSpectrum.a"
LIBS += "C:/Librarys/gsl/lib/libgsl.a"


INCLUDEPATH += "C:/Librarys/DirectoryMonitor/include"
LIBS += "C:/Librarys/DirectoryMonitor/lib/libDirectoryMonitor.a"

INCLUDEPATH += "C:/Librarys/gsl/include"
RC_FILE = icon.rc


RESOURCES +=

DISTFILES += \
    settings.ini \
    screenIcon.png \
    settingsPic.png \
    Icon.ico
