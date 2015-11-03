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

#include "SettingsManager.h"
#include "ISettings.h"
#include "SettingsCache.h"

#include <QCoreApplication>
#include <QSettings>

namespace Settings {

QString Manager::profileName;
QMap<QString, std::shared_ptr<Cache>> Manager::cacheMap;

void Manager::setProfileName(const QString &name)
{
    profileName = name;
}

void Manager::save(ISettings *settings)
{
    settings->save(createQSettings());
}

void Manager::load(ISettings *settings)
{
    settings->load(createQSettings());
}

void Manager::save(const QString &key, QVariant value)
{
    save(createQSettings(), key, value);
}

void Manager::save(const QString &group, const QString &key, QVariant value)
{
    save(createQSettings(), group, key, value);
}

void Manager::save(std::shared_ptr<QSettings> qset, const QString &key, QVariant value)
{
    qset->setValue(key, value);

    auto cache = getCache();

    cache->set(key, value);
}

void Manager::save(std::shared_ptr<QSettings> qset, const QString &group, const QString &key, QVariant value)
{
    qset->beginGroup(group);
    qset->setValue(key, value);
    qset->endGroup();

    auto cache = getCache();

    cache->set(group, key, value);
}

QVariant Manager::load(const QString &key, QVariant defaultValue)
{
    return load(createQSettings(), key, defaultValue);
}

QVariant Manager::load(const QString &group, const QString &key, QVariant defaultValue)
{
    return load(createQSettings(), group, key, defaultValue);
}

QVariant Manager::load(std::shared_ptr<QSettings> qset, const QString &key, QVariant defaultValue)
{
    auto cache = getCache();

    QVariant value = cache->get(key);

    if (value.isValid())
        return value;

    value = qset->value(key, defaultValue);

    cache->set(key, value);

    return value;
}

QVariant Manager::load(std::shared_ptr<QSettings> qset, const QString &group, const QString &key, QVariant defaultValue)
{
    auto cache = getCache();

    QVariant value = cache->get(group, key);

    if (value.isValid())
        return value;

    qset->beginGroup(group);
    value = qset->value(key, defaultValue);
    qset->endGroup();

    cache->set(group, key, value);

    return value;
}

std::shared_ptr<QSettings> Manager::createQSettings()
{
    auto qset = std::make_shared<QSettings>(QSettings::NativeFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    if (!profileName.isEmpty())
        qset->beginGroup(profileName);

    return qset;
}

std::shared_ptr<Cache> Manager::getCache()
{
    std::shared_ptr<Cache> cache = cacheMap[profileName];

    if (cache == nullptr) {
        cache = std::make_shared<Cache>();
        cacheMap[profileName] = cache;
    }

    return cache;
}

} // Settings

