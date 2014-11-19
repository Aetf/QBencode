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

public_headers = \
    include/qbencodedocument.h \
    include/qbencodevalue.h
HEADERS += \
    $$public_headers \
    qbencodeparser_p.h
