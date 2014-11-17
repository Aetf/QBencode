include(../defaults.pri)
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

SOURCES += gtest/gtest-all-strip.cc main.cpp\
    test_qbencodeparser.cpp

QMAKE_INCDIR += gtest

LIBS += -L../src -lQBencode
