#-------------------------------------------------
#
# Project created by QtCreator 2015-10-30T14:39:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DnFsMapCopy
TEMPLATE = app

CONFIG += c++11
DEFINES += Q_COMPILER_INITIALIZER_LISTS

TRANSLATIONS += dfmc_ja.ts

SOURCES += main.cpp\
        MainWindow.cpp \
    UI/StringHistoryComboBox.cpp \
    StringAnalyzer/RegExPathAnalyzer.cpp \
    Log.cpp \
    UI/SelectMultiFilesDialog.cpp \
    FileCopy.cpp \
    Settings/SettingsCache.cpp \
    Settings/SettingsManager.cpp \
    WidgetAction/ViewActionRemoveSelected.cpp \
    WidgetAction/WidgetAction.cpp \
    WidgetAction/WidgetActions.cpp

HEADERS  += MainWindow.h \
    UI/StringHistoryComboBox.h \
    StringAnalyzer/IStringAnalyzer.h \
    StringAnalyzer/RegExPathAnalyzer.h \
    Log.h \
    UI/SelectMultiFilesDialog.h \
    FileCopy.h \
    Settings/ISettings.h \
    Settings/SettingsCache.h \
    Settings/SettingsManager.h \
    WidgetAction/ViewActionRemoveSelected.h \
    WidgetAction/WidgetAction.h \
    WidgetAction/WidgetActions.h

FORMS    += MainWindow.ui
