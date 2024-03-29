#-------------------------------------------------
#
# Project created by QtCreator 2017-03-31T13:04:08
#
#-------------------------------------------------

QT       += core gui sql network serialport
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartTrashCan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    loginwindow.cpp \
    optionswindow.cpp \
    userdatabasemanager.cpp \
    datawindow.cpp \
    signupwindow.cpp \
    extrapickupwindow.cpp \
    smtp.cpp \
    trashinfodatabasemanager.cpp \
    historydatabasemanager.cpp

HEADERS  += mainwindow.h \
    loginwindow.h \
    optionswindow.h \
    userdatabasemanager.h \
    datawindow.h \
    signupwindow.h \
    extrapickupwindow.h \
    smtp.h \
    trashinfodatabasemanager.h \
    historydatabasemanager.h

target.path = $$[QT_INSTALL_EXAMPLES]/charts/barchart
INSTALLS += target

FORMS    += mainwindow.ui \
    loginwindow.ui \
    optionswindow.ui \
    datawindow.ui \
    signupwindow.ui \
    extrapickupwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES +=
