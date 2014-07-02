TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Weffc++

SOURCES += attrmatch.cpp \
    main.cpp \
    bmlib/log.cpp \
    model/relation.cpp \
    model/matcher.cpp \
    model/attributeblock.cpp

HEADERS += \
    attrmatch.hpp \
    bmlib/log.hpp \
    model/relation.hpp \
    model/matcher.hpp \
    model/attributetype.hpp \
    model/attributeblock.hpp
