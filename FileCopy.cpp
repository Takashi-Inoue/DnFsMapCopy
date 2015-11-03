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

#include "FileCopy.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#ifdef Q_OS_WIN

#include <Shobjidl.h>
#include <comdef.h>

_COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));
_COM_SMARTPTR_TYPEDEF(IFileOperation, __uuidof(IFileOperation));

class ComInitialize
{
public:
    ComInitialize()
    {
        id = GetCurrentThreadId();
        result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    }

    ~ComInitialize()
    {
        Q_ASSERT(id == GetCurrentThreadId());

        if (SUCCEEDED(result))
            CoUninitialize();
    }

    bool isSucceeded()
    {
        return SUCCEEDED(result);
    }

private:
    HRESULT result;
    DWORD id;
};

class ComFileOperation
{
public:
    ComFileOperation(const QString &source, const QString &destDir) :
        isInitOk(false)
    {
        HRESULT result = pfo.CreateInstance(CLSID_FileOperation);
        if (FAILED(result))
            return;

        result = SHCreateItemFromParsingName(QDir::toNativeSeparators(source).toStdWString().c_str(), nullptr, IID_PPV_ARGS(&psiFrom));
        if (FAILED(result))
            return;

        result = SHCreateItemFromParsingName(QDir::toNativeSeparators(destDir).toStdWString().c_str(), nullptr, IID_PPV_ARGS(&psiTo));
        if (FAILED(result))
            return;

        isInitOk = true;
    }

    bool isInitSucceeded()
    {
        return isInitOk;
    }

    bool copyItem(const QString &newName)
    {
        Q_ASSERT(isInitOk);

        HRESULT result = pfo->CopyItem(psiFrom, psiTo, newName.toStdWString().c_str(), nullptr);

        if (FAILED(result))
            return false;

        result = pfo->PerformOperations();

        return SUCCEEDED(result);
    }

    bool moveItem(const QString &newName)
    {
        Q_ASSERT(isInitOk);

        HRESULT result = pfo->MoveItem(psiFrom, psiTo, newName.toStdWString().c_str(), nullptr);

        if (FAILED(result))
            return false;

        result = pfo->PerformOperations();

        return SUCCEEDED(result);
    }

private:
    IFileOperationPtr pfo;
    IShellItemPtr psiFrom;
    IShellItemPtr psiTo;

    bool isInitOk;
};

bool FileCopy::copy(const QString &source, const QString &destDir)
{
    if (!QFileInfo::exists(source) || !QFileInfo::exists(destDir))
        return false;

    ComInitialize comInit;

    if (!comInit.isSucceeded())
        return false;

    ComFileOperation comFileOperation(source, destDir);

    if (!comFileOperation.isInitSucceeded())
        return false;

    return comFileOperation.copyItem(QFileInfo(source).fileName());
}

bool FileCopy::move(const QString &source, const QString &destDir)
{
    if (!QFileInfo::exists(source) || !QFileInfo::exists(destDir))
        return false;

    ComInitialize comInit;

    if (!comInit.isSucceeded())
        return false;

    ComFileOperation comFileOperation(source, destDir);

    if (!comFileOperation.isInitSucceeded())
        return false;

    return comFileOperation.moveItem(QFileInfo(source).fileName());
}

#endif
