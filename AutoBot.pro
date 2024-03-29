#-------------------------------------------------
#
# Project created by QtCreator 2019-05-26T19:42:43
#
#-------------------------------------------------

QT       += core testlib gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoBot
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
  autobot_unit.cpp \
        main.cpp \
        mainwindow.cpp \
    autobot_account.cpp \
    autobot_edit_window.cpp \
    autobot_helper.cpp \
    autobot_manager.cpp \
    target_room.cpp \
  target_room_edit_window.cpp \
    target_speech.cpp \
    target_speech_edit_window.cpp \
    target_speech_set.cpp \
    task_rumble_fish.cpp

HEADERS += \
  autobot_unit.h \
        mainwindow.h \
    autobot_account.h \
    autobot_helper.h \
    autobot_edit_window.h \
    autobot_manager.h \
    target_room.h \
  target_room_edit_window.h \
    target_speech.h \
    target_speech_set.h \
    target_speech_edit_window.h \
    task_interface.h \
    task_rumble_fish.h

FORMS += \
        mainwindow.ui \
    autobot_edit_window_form.ui \
    autobot_login_dialog_form.ui \
    target_room_dialog_form.ui \
  target_room_edit_window_form.ui \
    target_speech_edit_window_form.ui \
    target_speech_dialog_form.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
