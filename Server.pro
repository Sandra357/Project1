TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mongoose.cpp

HEADERS += \
    mongoose.h

LIBS += -lwsock32


QMAKE_CXXFLAGS += -std=c++0x
