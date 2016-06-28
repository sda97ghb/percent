TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS   += -std=c++11

SOURCES += main.cpp \
    Pixel.cpp

HEADERS += \
    Matrices.hpp \
    Pixel.h
