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

#ifndef SETTINGSCACHE_H
#define SETTINGSCACHE_H

#include <QMap>
#include <QVariant>

namespace Settings {

class Cache
{
public:
    Cache() = default;
    ~Cache() = default;

    QVariant get(const QString &key) const;
    QVariant get(const QString &group, const QString &key) const;
    void set(const QString &key, QVariant value);
    void set(const QString &group, const QString &key, QVariant value);

private:
    QString keyString(const QString &group, const QString &key) const;

    QMap<QString, QVariant> valueMap;
};

} // Settings

#endif // SETTINGSCACHE_H
