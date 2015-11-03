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

#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <QSettings>
#include <memory>

namespace Settings {

class Cache;
class Manager;

class ISettings
{
public:
    ISettings() = default;
    virtual ~ISettings() = default;

protected:
    virtual void save(std::shared_ptr<QSettings>) const = 0;
    virtual void load(std::shared_ptr<QSettings>) = 0;

private:
    ISettings(const ISettings &) = delete;
    ISettings(ISettings &&) = delete;
    ISettings &operator=(const ISettings &) = delete;
    ISettings &operator=(ISettings &&) = delete;

    friend class Manager;
};

} // Settings

#endif // SETTINGS_H
