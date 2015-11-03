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

#include "StringHistoryComboBox.h"
#include "Settings/SettingsManager.h"
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QItemDelegate>
#include <QStringListModel>
#include <QDebug>

class StringHistoryComboBox::ComboBoxDelegate : public QItemDelegate
{
public:
    explicit ComboBoxDelegate(QObject * parent = nullptr) : QItemDelegate(parent) {}
    ~ComboBoxDelegate() = default;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize size = QItemDelegate::sizeHint(option, index);
        size.setHeight(qMax(18, size.height()));

        return size;
    }
};

StringHistoryComboBox::StringHistoryComboBox(QComboBox *comboBox, QObject *parent) :
    QObject(parent),
    comboBox(comboBox)
{
    Q_CHECK_PTR(comboBox);

    comboBox->setModel(new QStringListModel(comboBox));
    comboBox->setItemDelegate(new ComboBoxDelegate());
    comboBox->view()->installEventFilter(this);

    comboBox->setMaxCount(30);
}

void StringHistoryComboBox::setMaxHistoryCount(int count)
{
    comboBox->setMaxCount(count);
}

void StringHistoryComboBox::setStringList(const QStringList &stringList)
{
    auto model = qobject_cast<QStringListModel *>(comboBox->model());
    model->setStringList(stringList);
}

void StringHistoryComboBox::setCurrentText(const QString &text)
{
    comboBox->setCurrentText(text);
}

void StringHistoryComboBox::updateHistory()
{
    if (currentText().isEmpty())
        return;

    auto model = qobject_cast<QStringListModel *>(comboBox->model());
    auto stringList = model->stringList();

    stringList.removeAll(currentText());
    stringList.prepend(currentText());

    if (stringList.size() > comboBox->maxCount())
        stringList.removeLast();

    model->setStringList(stringList);
    comboBox->setCurrentIndex(0);
}

QString StringHistoryComboBox::currentText() const
{
    return comboBox->currentText();
}

QStringList StringHistoryComboBox::history() const
{
    auto model = qobject_cast<QStringListModel *>(comboBox->model());
    return model->stringList();
}

void StringHistoryComboBox::saveHistory() const
{
    auto model = qobject_cast<QStringListModel *>(comboBox->model());
    Settings::Manager::save("comboBoxHistory", model->stringList());
}

void StringHistoryComboBox::loadHistory()
{
    auto model = qobject_cast<QStringListModel *>(comboBox->model());
    model->setStringList(Settings::Manager::load("comboBoxHistory", model->stringList()).toStringList());
}

bool StringHistoryComboBox::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != comboBox->view() || event->type() != QEvent::KeyPress)
        return QObject::eventFilter(obj, event);

    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

    if (!keyEvent->matches(QKeySequence::Delete))
        return QObject::eventFilter(obj, event);

    removeHistory(comboBox->view()->currentIndex().row());

    return true;
}

void StringHistoryComboBox::removeHistory(int index)
{
    auto model = qobject_cast<QStringListModel *>(comboBox->model());

    model->removeRow(index);

    if (comboBox->view()->isVisible())
        comboBox->showPopup(); // ポップアップのリサイズのため
}

