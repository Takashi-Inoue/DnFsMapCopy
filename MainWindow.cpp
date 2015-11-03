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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "FileCopy.h"
#include "Log.h"
#include "StringAnalyzer/RegExPathAnalyzer.h"
#include "WidgetAction/ViewActionRemoveSelected.h"
#include "WidgetAction/WidgetActions.h"
#include "UI/SelectMultiFilesDialog.h"
#include "UI/StringHistoryComboBox.h"

#include <QApplication>
#include <QClipboard>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMimeData>
#include <QStyle>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButtonOpenDirForRegEx->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon));
    ui->pushButtonRegisterDirs->setIcon(ui->pushButtonOpenDirForRegEx->icon());
    ui->pushButtonRegisterSourceDirs->setIcon(ui->pushButtonOpenDirForRegEx->icon());
    ui->pushButtonRegisterFiles->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));

    comboBoxDirPath = new StringHistoryComboBox(ui->comboBoxDirPath, this);
    comboBoxDirPath->loadHistory();

    ui->comboBoxDirPath->installEventFilter(this);

    ui->listWidgetDest->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidgetSource->setContextMenuPolicy(Qt::CustomContextMenu);

    actionsDestWidget = new WidgetActions(ui->listWidgetDest);
    actionsSourceWidget = new WidgetActions(ui->listWidgetSource);

    actionsDestWidget->append(new ViewActionRemoveSelected(ui->listWidgetDest));
    actionsSourceWidget->append(new ViewActionRemoveSelected(ui->listWidgetSource));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (obj != ui->comboBoxDirPath || e->type() != QEvent::KeyPress)
        return QMainWindow::eventFilter(obj, e);

    auto event = static_cast<QKeyEvent *>(e);

    if (!event->matches(QKeySequence::Paste))
        return QMainWindow::eventFilter(obj, e);

    const QString clipText = QApplication::clipboard()->text();

    if (QFileInfo::exists(clipText))
        QApplication::clipboard()->setText(QString(clipText).replace('\\', '/'));

    // setText() を使うとアンドゥの挙動がおかしくなるので paste() で
    ui->comboBoxDirPath->lineEdit()->paste();

    QApplication::clipboard()->setText(clipText);

    return true;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    e->mimeData()->hasFormat("text/uri-list") ? e->acceptProposedAction()
                                              : e->ignore();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *e)
{
    auto widget = childAt(e->pos());

    if (widget != ui->listWidgetDest->viewport()
     && widget != ui->listWidgetSource->viewport()
     && widget != ui->comboBoxDirPath->lineEdit())
    {
        e->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    auto widget = childAt(e->pos());

    QStringList paths;

    for (const QUrl &url : e->mimeData()->urls())
        paths << url.toLocalFile();

    if (widget == ui->listWidgetSource->viewport()) {
        addSource(paths);
        return;
    }

    auto itr = std::remove_if(paths.begin(), paths.end(), [](const QString &path) {
        return QFileInfo(path).isFile();
    });

    paths.erase(itr, paths.end());

    if (widget == ui->listWidgetDest->viewport()) {
        addDestination(paths);
        return;
    }

    if (widget == ui->comboBoxDirPath->lineEdit())
        comboBoxDirPath->setCurrentText(paths.first().replace('\\', '/'));
}

void MainWindow::on_pushButtonOpenDirForRegEx_clicked()
{
    QString initPath = ui->comboBoxDirPath->currentText();
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Please select a directory."), initPath);

    if (dirPath.isEmpty())
        return;

    comboBoxDirPath->setCurrentText(dirPath);
}

void MainWindow::on_pushButtonApply_clicked()
{
    QString path = comboBoxDirPath->currentText();

    if (path.isEmpty())
        return;

    if (path.endsWith('/'))
        path.chop(1);

    addDestination(RegExPathAnalyzer(path).resultList());

    comboBoxDirPath->updateHistory();
    comboBoxDirPath->saveHistory();
}

void MainWindow::on_pushButtonRegisterDirs_clicked()
{
    QString initPath = ui->comboBoxDirPath->currentText();

    SelectMultiFilesDialog dialog(this, initPath, SelectMultiFilesDialog::DirsOnly);

    if (dialog.exec() == QDialog::Rejected)
        return;

    addDestination(dialog.selectedFiles());
}

void MainWindow::on_pushButtonRegisterFiles_clicked()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Please select any files"), "", "", nullptr, QFileDialog::HideNameFilterDetails);

    addSource(paths);
}

void MainWindow::on_pushButtonRegisterSourceDirs_clicked()
{
    SelectMultiFilesDialog dialog(this, "", SelectMultiFilesDialog::DirsOnly);

    if (dialog.exec() == QDialog::Rejected)
        return;

    QStringList paths = dialog.selectedFiles();

    addSource(paths);
}

void MainWindow::on_pushButtonCopy_clicked()
{
    int count = ui->listWidgetSource->count();

    if (count != ui->listWidgetDest->count())
        return;

    for (int i = 0; i < count; ++i) {
        auto sourceItem = ui->listWidgetSource->item(i);

        QString source = sourceItem->data(Qt::UserRole).toString();
        QString destDir = ui->listWidgetDest->item(i)->text();

        bool result = FileCopy::copy(source, destDir);

        result ? Log::log(tr("Succeeded to copy. ") + source + " >>> " + destDir)
               : Log::log(tr("Failed to copy. ") + source + " >>> " + destDir);

        updateItemInfo(sourceItem, result);
    }
}

void MainWindow::on_pushButtonMove_clicked()
{
    int count = ui->listWidgetSource->count();

    if (count != ui->listWidgetDest->count())
        return;

    for (int i = 0; i < count; ++i) {
        auto sourceItem = ui->listWidgetSource->item(i);

        QString source = sourceItem->data(Qt::UserRole).toString();
        QString destDir = ui->listWidgetDest->item(i)->text();

        bool result = FileCopy::move(source, destDir);

        result ? Log::log(tr("Succeeded to move. ") + source + " >>> " + destDir)
               : Log::log(tr("Failed to move. ") + source + " >>> " + destDir);

        if (result)
            sourceItem->setData(Qt::UserRole, destDir + "/" + sourceItem->text());

        updateItemInfo(sourceItem, result);
    }
}

void MainWindow::addSource(const QStringList &paths)
{
    for (const auto &path : paths) {
        QFileInfo fi(path);

        auto items = ui->listWidgetSource->findItems(fi.fileName(), Qt::MatchFixedString);

        auto itr = std::find_if(items.begin(), items.end(), [&](const QListWidgetItem *item) {
            return item->data(Qt::UserRole) == path;
        });

        if (itr == items.end()) {
            QListWidgetItem *item = new QListWidgetItem(QFileIconProvider().icon(fi), fi.fileName());
            item->setData(Qt::UserRole, path);
            item->setToolTip(path);

            ui->listWidgetSource->addItem(item);
        }
    }

    ui->labelSourceCount->setText(QString(tr("%1 source(s)")).arg(ui->listWidgetSource->count()));

    checkTargetCount();
}

void MainWindow::addDestination(const QStringList &paths)
{
    for (const auto &path : paths) {
        auto items = ui->listWidgetDest->findItems(path, Qt::MatchFixedString);

        if (items.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(path);
            item->setToolTip(path);

            ui->listWidgetDest->addItem(item);
        }
    }

    ui->labelDestCount->setText(QString(tr("%1 destination(s)")).arg(ui->listWidgetDest->count()));

    checkTargetCount();
}

void MainWindow::checkTargetCount()
{
    bool enabled = ui->listWidgetDest->count() == ui->listWidgetSource->count();

    ui->pushButtonCopy->setEnabled(enabled);
    ui->pushButtonMove->setEnabled(enabled);
}

void MainWindow::updateItemInfo(QListWidgetItem *item, bool isOperationSucceeded)
{
    QFont font(item->font());

    font.setBold(isOperationSucceeded);
    font.setItalic(!isOperationSucceeded);

    item->setFont(font);

    isOperationSucceeded ? item->setToolTip(tr("[Succeeded]  -  ") + item->data(Qt::UserRole).toString())
                         : item->setToolTip(tr("[Failed]  -  ")    + item->data(Qt::UserRole).toString());
}
