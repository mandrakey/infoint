TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
LIBS += -pthread

SOURCES += main.cpp \
    #calculation/windowdeltacalculator.cpp \
    calculation/entry.cpp \
    calculation/windowdeltacalculator.cpp \
    delta.cpp

HEADERS += \
    #calculation/windowdeltacalculator.h \
    calculation/entry.h \
    delta.hpp \
    calculation/windowdeltacalculator.hpp

