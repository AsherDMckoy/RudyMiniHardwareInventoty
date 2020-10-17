#-------------------------------------------------
#
# Project created by QtCreator 2017-04-24T23:22:20
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Object_Oriented_Project
TEMPLATE = app

TRANSLATIONS = txteditor_en.ts\
               txteditor_es.ts


SOURCES += main.cpp\
        interface.cpp \
    mysqldatabase.cpp \
    persons.cpp \
    customers.cpp \
    employees.cpp \
    suppliers.cpp \
    address.cpp

HEADERS  += interface.h \
    mysqldatabase.h \
    persons.h \
    customers.h \
    employees.h \
    suppliers.h \
    address.h

FORMS    += \
    mainwindow.ui \
    mainwindow.ui

RESOURCES += \
    countries.qrc \
    images.qrc \
    stylesheet.qrc \
    languages.qrc
