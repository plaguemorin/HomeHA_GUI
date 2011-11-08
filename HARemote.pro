QT += network svg
TARGET = HARemote
CONFIG += qt debug

unix:INCLUDEPATH += /usr/include/lua5.1
unix:LIBS += -llua5.1

SOURCES += \
   main.cpp \
    mainwindow.cpp \
    lua_wrapper.cpp \
    lua_gui.cpp \
    lua_widget.cpp \
    awidget.cpp \
    awidgetlabel.cpp \
    awidgetbutton.cpp \
    apage.cpp \
    lua_page.cpp

HEADERS += \
    mainwindow.h \
    lua_wrapper.h \
    script_internal.h \
    awidget.h \
    awidgetlabel.h \
    awidgetbutton.h \
    apage.h

OTHER_FILES += \
    main.lua
