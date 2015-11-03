/*
 * Copyright 2015 Takashi Inoue
 *
 * This file is part of DnFsMapCopy.
 *
 * DnFsMapCopy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DnFsMapCopy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DnFsMapCopy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Log.h"
#include <QApplication>
#include <QFile>
#include <QReadLocker>
#include <QWriteLocker>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

QStringList Log::applicationLogs;
QStringList Log::debugLogs;

QReadWriteLock Log::lock;

Log::Log(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QModelIndex Log::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

QModelIndex Log::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int Log::rowCount(const QModelIndex &) const
{
    QReadLocker locker(&lock);

    return applicationLogs.size();
}

int Log::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant Log::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid())
        return QVariant();

    QReadLocker locker(&lock);

    return applicationLogs.at(index.row());
}

void Log::init()
{
    qInstallMessageHandler(debugMessageHandler);
}

void Log::log(const QString &log)
{
    QWriteLocker locker(&lock);

    applicationLogs << log;
}

void Log::clear()
{
    QWriteLocker locker(&lock);

    applicationLogs.clear();
}

void Log::writeFile()
{
    writeApplicationLog();
    writeDebugLog();
}

bool Log::isEmpty()
{
    return applicationLogs.isEmpty();
}

void Log::writeApplicationLog()
{
    QReadLocker locker(&lock);

    if (applicationLogs.isEmpty())
        return;

    QFile file(QApplication::applicationDirPath() + "/" + QApplication::applicationName() + ".log");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "failed to write log to" << file.fileName() << ":" << file.errorString();
        return;
    }

    for (const auto &log : applicationLogs) {
        file.write(log.toUtf8());
        file.write("\r\n");
    }
}

void Log::writeDebugLog()
{
    QReadLocker locker(&lock);

    if (debugLogs.isEmpty())
        return;

    QFile file(QApplication::applicationDirPath() + "/debug.log");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "failed to write log to" << file.fileName() << ":" << file.errorString();
        return;
    }

    for (const auto &log : debugLogs) {
        file.write(log.toUtf8());
        file.write("\r\n");
    }
}

void Log::debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QWriteLocker locker(&lock);

    switch (type) {
    case QtDebugMsg:
        debugLogs << outputMessage(QString("Debug: %1 (%2)").arg(msg).arg(context.function));
        break;

    case QtInfoMsg:
        debugLogs << outputMessage(QString("Info: %1").arg(msg));
        break;

    case QtWarningMsg:
        debugLogs << outputMessage(QString("Warning: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function));
        break;

    case QtCriticalMsg:
        debugLogs << outputMessage(QString("Critical: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function));
        break;

    case QtFatalMsg:
        outputMessage(QString("Fatal: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function));
        abort();
    }
}

QString Log::outputMessage(const QString &msg)
{
#ifdef Q_OS_WIN
    #ifdef UNICODE
        OutputDebugString((msg + "\r\n").toStdWString().c_str());
    #else
        OutputDebugString((msg + "\r\n").toStdString().c_str());
    #endif
#else
    fprintf(stderr, qPrintable(msg + "\n"));
#endif

    return msg;
}

