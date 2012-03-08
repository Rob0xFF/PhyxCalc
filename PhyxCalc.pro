#-------------------------------------------------
#
# Project created by QtCreator 2011-11-07T14:50:36
#
#-------------------------------------------------
# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE4EA669C

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

QT       += core gui

TARGET = phyxcalc
TEMPLATE = app

win32:INCLUDEPATH += ../boost
android:INCLUDEPATH += ../boost

SOURCES += main.cpp\
        mainwindow.cpp \
        lineparser.cpp \
        unitloader.cpp \
        exportdialog.cpp \
        settingsdialog.cpp \
        qearleyparser.cpp \
        phyxcalculator.cpp \
    qhidingtabwidget.cpp \
    phyxunit.cpp \
    phyxvariable.cpp \
    phyxunitsystem.cpp \
    phyxcompoundunit.cpp \
    phyxtesting.cpp \
    phyxvariablemanager.cpp \
    phyxsyntaxhighlighter.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
            lineparser.h \
            unitloader.h \
            exportdialog.h \
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
    phyxvariablemanager.h \
    phyxsyntaxhighlighter.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    exportdialog.ui \
    settingsdialog.ui \
    helpdialog.ui

RESOURCES += \
    images.qrc \
    settings.qrc

OTHER_FILES += \
    settings/grammar.txt \
    settings/definitions.txt \
    settings/symbols.txt \
    settings/settings.ini \
    settings/docks.txt \
    settings/electronicFunctions.txt \
    settings/numbersDock.txt \
    settings/operatorsDock.txt \
    doc/doc_en.txt

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-et/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-el/strings.xml \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/AndroidManifest.xml \
    android/version.xml
