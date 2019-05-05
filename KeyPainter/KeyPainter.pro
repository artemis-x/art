#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T23:36:05
#
#-------------------------------------------------

QT       += core gui\
            sql\
            widgets

TARGET = KeyPainter
TEMPLATE = app
DESTDIR = ../__r_exe   # do "Clean All", then "Run QMake", and then "Build All"

SOURCES += main.cpp\
        mainwindow.cpp \
    BuildKeypad.cpp \
    DlgBackground.cpp \
    DlgGrid.cpp \
    KeyPad.cpp \
    ButtonsBar.cpp \
    dlgtxtalign.cpp \
    draw_functions.cpp \
    IconsBar.cpp \
    DlgKeyPaste.cpp \
    dlgnuminput.cpp \
    dlgtextinpout.cpp \
    dlgdesignrectdgr.cpp \
    dlgmessageoper.cpp \
    dlgfreealign.cpp \
    dlgbuttonx2.cpp \
    KeyList.cpp \
    keypad_help_function.cpp \
    HdrList.cpp \
    KeyData.cpp \
    dlgscreenpos.cpp

HEADERS  += mainwindow.h \
    BuildKeypad.h \
    DlgBackground.h \
    DlgGrid.h \
    KeyPad.h \
    ButtonsBar.h \
    dlgtxtalign.h \
    draw_functions.h \
    IconsBar.h \
    DlgKeyPaste.h \
    dlgnuminput.h \
    dlgtextinpout.h \
    dlgdesignrectdgr.h \
    dlgmessageoper.h \
    dlgfreealign.h \
    dlgbuttonx2.h \
    KeyList.h \
    keypad_help_function.h \
    HdrList.h \
    KeyData.h \
    dlgscreenpos.h

FORMS    += \
    dlgkeypad.ui \
    dlgtxtalign.ui \
    DlgKeyPaste.ui \
    dlgnuminput.ui \
    dlgtextinpout.ui \
    dlgdesignrectdgr.ui \
    dlgmessageoper.ui \
    dlgfreealign.ui \
    dlgbuttonx2.ui \
    dlgscreenpos.ui

OTHER_FILES += \
    PaintInf.txt

RC_ICONS = paint.ico
