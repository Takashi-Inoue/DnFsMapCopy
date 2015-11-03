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

#include "SelectMultiFilesDialog.h"

#include <QAbstractItemView>

// native のダイアログではフォルダの複数選択ができないので、QFileDialogに手を入れる
SelectMultiFilesDialog::SelectMultiFilesDialog(QWidget *parent, const QString &initialDir, Mode mode) :
    dialog(std::make_shared<QFileDialog>(parent, QObject::tr("Please select one or more directories."), initialDir))
{
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);

    if (mode == DirsOnly) {
        dialog->setFileMode(QFileDialog::Directory);
        dialog->setOption(QFileDialog::ShowDirsOnly, true);
    }

    QAbstractItemView *listView = dialog->findChild<QAbstractItemView *>("listView");
    if (listView != nullptr)
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QAbstractItemView *treeView = dialog->findChild<QAbstractItemView *>("treeView");
    if (treeView != nullptr)
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

int SelectMultiFilesDialog::exec()
{
    return dialog->exec();
}

QStringList SelectMultiFilesDialog::selectedFiles() const
{
    return dialog->selectedFiles();
}

