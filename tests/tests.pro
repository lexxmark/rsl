QT += testlib
QT -= core gui

CONFIG += c++14

TARGET = tests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    test_ref_ptr.cpp

HEADERS += \
    test_ref_ptr.h \
    ../include/ref_ptr.h
