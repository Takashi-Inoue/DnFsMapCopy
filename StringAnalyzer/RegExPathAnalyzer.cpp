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

#include "RegExPathAnalyzer.h"
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDebug>

RegExPathAnalyzer::RegExPathAnalyzer(const QString &string) :
    originalString(string)
{
}

QString RegExPathAnalyzer::result()
{
    if (QFileInfo::exists(originalString))
        return originalString;

    return QString();
}

QStringList RegExPathAnalyzer::resultList()
{
    if (QFileInfo::exists(originalString))
        return {originalString};

    QStringList parts = originalString.split('/');
    QStringList result;

    auto drive = parts.takeFirst();

    QFileInfo::exists(drive) ? result << drive
                             : result << matchDrives(drive);

    for (const auto &part : parts) {
        int count = result.size();

        for (int i = 0; i < count; ++i) {
            auto parent = result.takeFirst();
            result << analyzeChild(parent, part);
        }
    }

    return result;
}

QStringList RegExPathAnalyzer::matchDrives(const QString &pattern)
{
    QStringList result;
    QRegularExpression re(pattern, QRegularExpression::CaseInsensitiveOption);

    for (const auto &drive : QDir::drives()) {
        auto letter = drive.absoluteFilePath().remove('/');

        if (QString(letter).replace(re, "").isEmpty())
            result << letter;
    }

    return result;
}

QStringList RegExPathAnalyzer::analyzeChild(const QString &parentPath, const QString &child)
{
    if (QFileInfo::exists(parentPath + "/" + child))
        return {parentPath + "/" + child};

    QStringList result;
    QRegularExpression re(child, QRegularExpression::CaseInsensitiveOption);

    for (const auto &name : QDir(parentPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System)) {
        if (QString(name).replace(re, "").isEmpty())
            result << parentPath + "/" + name;
    }

    return result;
}

