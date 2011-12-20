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
    muParser/muParserTokenReader.cpp \
    muParser/muParserTest.cpp \
    muParser/muParserInt.cpp \
    muParser/muParserError.cpp \
    muParser/muParserDLL.cpp \
    muParser/muParserCallback.cpp \
    muParser/muParserBytecode.cpp \
    muParser/muParserBase.cpp \
    muParser/muParser.cpp \
    lineparser.cpp \
    unitloader.cpp \
    exportdialog.cpp \
    flowlayout.cpp \
    settingsdialog.cpp \
    phexparser.cpp

HEADERS  += mainwindow.h \
    muParser/muParserTokenReader.h \
    muParser/muParserToken.h \
    muParser/muParserTest.h \
    muParser/muParserStack.h \
    muParser/muParserInt.h \
    muParser/muParserFixes.h \
    muParser/muParserError.h \
    muParser/muParserDLL.h \
    muParser/muParserDef.h \
    muParser/muParserCallback.h \
    muParser/muParserBytecode.h \
    muParser/muParserBase.h \
    muParser/muParser.h \
    lineparser.h \
    unitloader.h \
    exportdialog.h \
    flowlayout.h \
    settingsdialog.h \
    global.h \
    phexparser.h

FORMS    += mainwindow.ui \
    exportdialog.ui \
    settingsdialog.ui

RESOURCES += \
    images.qrc

LIBS += -ludunits2




unix:!macx:!symbian: LIBS += -L/usr/local/lib64/ -ludunits2

INCLUDEPATH += /usr/local/lib64
DEPENDPATH += /usr/local/lib64

unix:!macx:!symbian: PRE_TARGETDEPS += /usr/local/lib64/libudunits2.a
