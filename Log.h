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

#ifndef LOG_H
#define LOG_H

#include <QAbstractItemModel>
#include <QStringList>
#include <QReadWriteLock>

class QTextEdit;

class Log : public QAbstractItemModel
{
public:
    explicit Log(QObject * parent = nullptr);
    ~Log() = default;

public: // implementation for QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public:
    static void init();
    static void log(const QString &log);

    static void clear();
    static void writeFile();

    static bool isEmpty();

private:
    static void writeApplicationLog();
    static void writeDebugLog();

    static void debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QString outputMessage(const QString &msg);

    static QStringList applicationLogs;
    static QStringList debugLogs;

    static QReadWriteLock lock;

    Log(const Log &) = delete;
    Log(Log &&) = delete;
    Log &operator=(const Log &) = delete;
    Log &operator=(Log &&) = delete;
};

#endif // LOG_H
