#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/Debug
} else {
    DESTDIR = $$PWD/Release
}

CONFIG += c++11
TEMPLATE = app

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../build/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_32
}

LIBS += -L$$DESTINATION_SDK_PATH -lHtmlFile
LIBS += -L$$DESTINATION_SDK_PATH -lUnicodeConverter
LIBS += -L$$DESTINATION_SDK_PATH -lgraphics

linux-g++ | linux-g++-64 | linux-g++-32 {
    QMAKE_LFLAGS += -Wl,--rpath=./

    LIBS        += $$PWD/../../build/bin/icu/linux_64/libicuuc.so.55
    LIBS        += $$PWD/../../build/bin/icu/linux_64/libicudata.so.55
    message(linux)
}

win32 {
LIBS += -ladvapi32 \
        -luser32 \
        -lshell32
}

SOURCES += main.cpp
