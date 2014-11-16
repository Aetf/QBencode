include(../defaults.pri)
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

SOURCES += gtest-all-strip.cc main.cpp\
    test_qbencodeparser.cpp

HEADERS += gtest-strip.h

LIBS += -L../src -lQBencode
