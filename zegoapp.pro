#-------------------------------------------------
#
# Project created by QtCreator 2018-06-05T10:17:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zegoapp
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

include(cglobal.prf)
include(./zego/zego.pri)
include(ffmpeg.pri)
INCLUDEPATH += ./ljlive
INCLUDEPATH += ./Model
INCLUDEPATH += ./Signal
INCLUDEPATH += ./VideoCapture
INCLUDEPATH += ./UI


SOURCES += main.cpp\
        mainwindow.cpp \
    ljlive/ljlivepublishercallback.cpp \
    ljlive/ljliveplayercallback.cpp \
    ljlive/clogsetting.cpp \
    ljlive/ljroomcallback.cpp \
    Model/ZegoRoomModel.cpp \
    Model/ZegoRoomMsgModel.cpp \
    Model/ZegoStreamModel.cpp \
    Model/ZegoUserModel.cpp \
    Signal/ZegoSDKSignal.cpp \
    VideoCapture/ZegoVideoCaptureDeviceDemo.cpp \
    VideoCapture/ZegoVideoCaptureFactoryDemo.cpp \
    UI/ZegoAVView.cpp \
    Model/ZegoVideoFrame.cpp \
    ljlive/ljuserviewwidget.cpp \
    ljlive/ljgolbalconfigmanager.cpp

HEADERS  += mainwindow.h \
    ljlive/ljlivepublishercallback.h \
    ljlive/ljliveplayercallback.h \
    ljlive/clogsetting.h \
    ljlive/ljroomcallback.h \
    Model/ZegoRoomModel.h \
    Model/ZegoRoomMsgModel.h \
    Model/ZegoStreamModel.h \
    Model/ZegoUserModel.h \
    Signal/ZegoSDKSignal.h \
    VideoCapture/ZegoVideoCaptureDeviceDemo.h \
    VideoCapture/ZegoVideoCaptureFactoryDemo.h \
    UI/ZegoAVView.h \
    Model/ZegoVideoFrame.h \
    ljlive/ljuserviewwidget.h \
    ljlive/ljgolbalconfigmanager.h

FORMS    += mainwindow.ui \
    ljlive/ljuserviewwidget.ui

RESOURCES += \
    source.qrc
