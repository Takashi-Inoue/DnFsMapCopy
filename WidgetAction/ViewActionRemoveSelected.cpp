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

#include "ViewActionRemoveSelected.h"
#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QStyle>

ViewActionRemoveSelected::ViewActionRemoveSelected(QAbstractItemView *view) :
    WidgetAction(view)
{
    act->setText(tr("Remove"));
    act->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    act->setShortcut(QKeySequence::Delete);
}

void ViewActionRemoveSelected::appendToMenu(QMenu *menu)
{
    WidgetAction::appendToMenu(menu);

    auto view = qobject_cast<QAbstractItemView *>(widget);

    if (view == nullptr)
        return;

    act->setEnabled(!view->selectionModel()->selectedIndexes().isEmpty());
}

void ViewActionRemoveSelected::execAction()
{
    auto view = qobject_cast<QAbstractItemView *>(widget);

    if (view == nullptr)
        return;

    auto rows = view->selectionModel()->selectedRows();

    qSort(rows.begin(), rows.end(), [](const QModelIndex &lhs, const QModelIndex &rhs) {
        return lhs.row() > rhs.row();
    });

    for (const auto &row : rows)
        view->model()->removeRow(row.row());
}

