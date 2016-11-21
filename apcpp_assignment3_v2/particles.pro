QT       += core gui widgets 

TARGET = particles
TEMPLATE = app

CONFIG += c++14 

SOURCES += main.cpp\
        widget.cpp\
        particle.cpp\
        engine.cpp

HEADERS += widget.h\
        particle.h\
        engine.h
