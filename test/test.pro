TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

OBJECTS_DIR = ./temp

SOURCES += main.cpp

include (../src/util.pri)

LIBS += -lpthread
