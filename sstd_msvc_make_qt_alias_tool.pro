
#just use for windows msvc

TEMPLATE = app

CONFIG(debug,debug|release){
    TARGET = sstd_msvc_make_qt_alias_tool_debug
}else{
    TARGET = sstd_msvc_make_qt_alias_tool
}

QMAKE_CXXFLAGS += /std:c++latest
CONFIG += utf8_source
CONFIG += suppress_vcproj_warnings

QT -= core
QT -= gui

SOURCES += $$PWD/main.cpp




