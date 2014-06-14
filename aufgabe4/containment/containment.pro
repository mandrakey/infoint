TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Weffc++

SOURCES += containment.cpp \
    algo/DepthFirst.cpp \
    model/Query.cpp \
    model/FileParser.cpp \
    main.cpp \
    model/Literal.cpp \
    bmlib/log.cpp

HEADERS += \
    algo/Algorithm.hpp \
    algo/DepthFirst.hpp \
    model/Query.hpp \
    model/FileParser.hpp \
    containment.hpp \
    model/Literal.hpp \
    bmlib/log.hpp
