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

#include "SettingsCache.h"
#include <QDebug>

namespace Settings {

QVariant Cache::get(const QString &key) const
{
    Q_ASSERT(!key.isEmpty());

    return valueMap.value(key, QVariant());
}

QVariant Cache::get(const QString &group, const QString &key) const
{
    return get(keyString(group, key));
}

void Cache::set(const QString &key, QVariant value)
{
    Q_ASSERT(!key.isEmpty());

    if (!value.isValid())
        qWarning() << "[" + key + "]" << "value is invalid!";

    valueMap[key] = value;
}

void Cache::set(const QString &group, const QString &key, QVariant value)
{
    set(keyString(group, key), value);
}

QString Cache::keyString(const QString &group, const QString &key) const
{
    Q_ASSERT(!key.isEmpty());

    return group.isEmpty() ? key : group + "/" + key;
}


} // Settings
