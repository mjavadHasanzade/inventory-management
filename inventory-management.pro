#-------------------------------------------------
#
# Project created by QtCreator 2023-12-02T09:56:20
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = inventory-management
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ./sources/addproductdialog.cpp \
    ./sources/database.cpp \
    ./sources/inventory.cpp \
    ./sources/inventorydialog.cpp \
    ./sources/product.cpp \
    ./sources/main.cpp \
    ./sources/mainwindow.cpp \
    ./sources/queries.cpp


HEADERS += \
    ./headers/addproductdialog.h \
    ./headers/database.h \
    ./headers/inventory.h \
    ./headers/inventorydialog.h \
    ./headers/product.h \
    ./headers/mainwindow.h \
    ./headers/queries.h


FORMS += \
        ./forms/addproductdialog.ui \
        ./forms/inventorydialog.ui \
        ./forms/mainwindow.ui

RESOURCES += \
    styles.qrc \
    imgs.qrc
