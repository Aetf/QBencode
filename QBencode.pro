#-------------------------------------------------
#
# Project created by QtCreator 2014-11-12T13:35:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QBencode
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    qbencodedocument.cpp \
    qbencodevalue.cpp \
    qbencodeparser.cpp

HEADERS += \
    qbencodedocument.h \
    qbencodevalue.h \
    qbencodeparser_p.h

OTHER_FILES += \
    style.astylerc
