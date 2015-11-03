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

#ifndef ISTRINGANALYZER
#define ISTRINGANALYZER

#include <QStringList>

class IStringAnalyzer
{
public:
    IStringAnalyzer() = default;
    virtual ~IStringAnalyzer() = default;

    virtual QString result() = 0;
    virtual QStringList resultList() = 0;

private:
    IStringAnalyzer(const IStringAnalyzer &) = delete;
    IStringAnalyzer(IStringAnalyzer &&) = delete;
    IStringAnalyzer &operator=(const IStringAnalyzer &) = delete;
    IStringAnalyzer &operator=(IStringAnalyzer &&) = delete;
};

#endif // ISTRINGANALYZER

