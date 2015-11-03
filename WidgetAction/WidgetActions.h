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

#ifndef WIDGETACTIONS_H
#define WIDGETACTIONS_H

#include <QObject>
#include <QList>

class WidgetAction;

class QMenu;
class QWidget;

class WidgetActions : public QObject
{
    Q_OBJECT
public:
    WidgetActions(QWidget *widget);
    virtual ~WidgetActions() = default;

    void append(WidgetAction *action);

protected slots:
    void onCustomContextMenuRequested(const QPoint &pos);

protected:
    QMenu *menu;
    QList<WidgetAction *> actions;
};

#endif // WIDGETACTIONS_H
