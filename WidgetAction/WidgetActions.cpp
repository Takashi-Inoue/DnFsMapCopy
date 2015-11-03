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

#include "WidgetActions.h"
#include "WidgetAction.h"
#include <QCursor>
#include <QMenu>
#include <QWidget>

WidgetActions::WidgetActions(QWidget *widget) :
    QObject(widget),
    menu(new QMenu(widget))
{
    connect(widget, SIGNAL(customContextMenuRequested(QPoint)),  SLOT(onCustomContextMenuRequested(QPoint)));
}

void WidgetActions::append(WidgetAction *action)
{
    if (!actions.contains(action))
        actions << action;
}

void WidgetActions::onCustomContextMenuRequested(const QPoint &/*pos*/)
{
    menu->clear();

    for (const auto &action : actions)
        action->appendToMenu(menu);

    menu->popup(QCursor::pos());
}
