TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Weffc++

SOURCES += attrmatch.cpp \
    main.cpp \
    bmlib/log.cpp

HEADERS += \
    attrmatch.hpp \
    bmlib/log.hpp
