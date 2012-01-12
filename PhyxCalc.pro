#-------------------------------------------------
#
# Project created by QtCreator 2011-11-07T14:50:36
#
#-------------------------------------------------

QT       += core gui

TARGET = phyxcalc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        lineparser.cpp \
        unitloader.cpp \
        exportdialog.cpp \
        flowlayout.cpp \
        settingsdialog.cpp \
        qearleyparser.cpp \
        phyxcalculator.cpp \
    qhidingtabwidget.cpp \
    phyxunit.cpp \
    phyxvariable.cpp \
    phyxunitsystem.cpp \
    phyxcompoundunit.cpp \
    phyxtesting.cpp \
    phyxvariablemanager.cpp

HEADERS  += mainwindow.h \
            lineparser.h \
            unitloader.h \
            exportdialog.h \
            flowlayout.h \
            settingsdialog.h \
            global.h \
            qearleyparser.h \
            phyxcalculator.h \
    qhidingtabwidget.h \
    phyxunit.h \
    phyxvariable.h \
    phyxunitsystem.h \
    phyxcompoundunit.h \
    phyxtesting.h \
    phyxvariablemanager.h

FORMS    += mainwindow.ui \
    exportdialog.ui \
    settingsdialog.ui

RESOURCES += \
    images.qrc \
    settings.qrc

OTHER_FILES += \
    settings/grammar.txt
