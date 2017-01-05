#-------------------------------------------------
#
# Project created by QtCreator 2016-12-31T15:07:29
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PowerfulSystemV1
TEMPLATE = app


SOURCES += main.cpp\
        mainsystem.cpp \
    login.cpp \
    commonregister.cpp \
    booksmanager.cpp \
    bookborrow.cpp

HEADERS  += mainsystem.h \
    databaseconnect.h \
    login.h \
    commonregister.h \
    booksmanager.h \
    bookborrow.h

FORMS    += mainsystem.ui \
    login.ui \
    commonregister.ui \
    booksmanager.ui \
    bookborrow.ui

RESOURCES += \
    images.qrc
