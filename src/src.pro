include(../defaults.pri)
TEMPLATE = lib

QT       -= gui

TARGET = QBencode
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -Wextra

SOURCES += \
    qbencodedocument.cpp \
    qbencodevalue.cpp \
    qbencodeparser.cpp

HEADERS += \
    qbencodedocument.h \
    qbencodevalue.h \
    qbencodeparser_p.h
