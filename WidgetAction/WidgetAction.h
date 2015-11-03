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

#ifndef WIDGETACTION_H
#define WIDGETACTION_H

#include <QObject>

class QAction;
class QMenu;
class QWidget;

class WidgetAction : public QObject
{
    Q_OBJECT
public:
    WidgetAction(QWidget *widget);
    virtual ~WidgetAction() = default;

    virtual void appendToMenu(QMenu *menu);

protected:
    virtual void execAction() = 0;

    QWidget *widget;
    QAction *act;

private slots:
    void exec();
};

#endif // WIDGETACTION_H
