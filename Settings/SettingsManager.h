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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QMap>
#include <QString>
#include <QVariant>
#include <memory>

class QSettings;

namespace Settings {

class Cache;
class ISettings;

class Manager
{
public:
    Manager() = default;
    ~Manager() = default;

    static void setProfileName(const QString &profileName);

    static void save(ISettings *);
    static void load(ISettings *);

    static void save(const QString &key, QVariant value);
    static void save(const QString &group, const QString &key, QVariant value);
    static void save(std::shared_ptr<QSettings> qset, const QString &key, QVariant value);
    static void save(std::shared_ptr<QSettings> qset, const QString &group, const QString &key, QVariant value);

    static QVariant load(const QString &key, QVariant defaultValue = QVariant());
    static QVariant load(const QString &group, const QString &key, QVariant defaultValue = QVariant());
    static QVariant load(std::shared_ptr<QSettings> qset, const QString &key, QVariant defaultValue = QVariant());
    static QVariant load(std::shared_ptr<QSettings> qset, const QString &group, const QString &key, QVariant defaultValue = QVariant());

private:
    static std::shared_ptr<QSettings> createQSettings();
    static std::shared_ptr<Cache> getCache();

    static QString profileName;
    static QMap<QString, std::shared_ptr<Cache>> cacheMap;
};

} // Settings

#endif // SETTINGSMANAGER_H
