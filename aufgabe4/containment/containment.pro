TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Weffc++

SOURCES += containment.cpp \
    algo/DepthFirst.cpp \
    model/Entry.cpp \
    model/Query.cpp

HEADERS += \
    algo/Algorithm.hpp \
    algo/DepthFirst.hpp \
    model/Entry.hpp \
    model/Query.hpp
