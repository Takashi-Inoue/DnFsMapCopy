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

#include "WidgetAction.h"
#include <QAction>
#include <QMenu>
#include <QWidget>

WidgetAction::WidgetAction(QWidget *widget) :
    QObject(widget),
    widget(widget),
    act(new QAction(widget))
{
    act->setShortcutContext(Qt::WidgetShortcut);

    connect(act, SIGNAL(triggered(bool)), SLOT(exec()));

    widget->addAction(act);
}

void WidgetAction::appendToMenu(QMenu *menu)
{
    menu->addAction(act);
}

void WidgetAction::exec()
{
    execAction();
}
