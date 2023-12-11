#-------------------------------------------------
#
# Project created by QtCreator 2023-08-29T15:02:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PWL_tool
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

CONFIG += c++11

SOURCES += \
    RecvThread.cpp \
    common.cpp \
        main.cpp \
        mainwindow.cpp \
    packet_loss.cpp \
    board_mapping/board_mapping.cpp \
    packethread.cpp \
    timestamp.cpp

HEADERS += \
    RecvThread.h \
    common.h \
        mainwindow.h \
    packet_loss.h \
    board_mapping/board_mapping.h \
    packethread.h \
    timestamp.h

FORMS += \
        mainwindow.ui \
    networkloss.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
