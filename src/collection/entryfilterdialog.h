/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2016 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ENTRYFILTERDIALOG_H
#define ENTRYFILTERDIALOG_H

#include <QDialog>
#include <QScopedPointer>

class EntryFilter;
namespace Ui
{
class EntryFilterDialog;
}

class EntryFilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EntryFilterDialog(EntryFilter &entryFilter);
    ~EntryFilterDialog() override;

    bool ignoreBlocked() const;

private:
    void setupEntryCounts();
    void setupCheckBoxes();
    void setupConnections();

private Q_SLOTS:
    void updateTotal();

private:
    QScopedPointer<Ui::EntryFilterDialog> m_ui;
    EntryFilter &m_entryFilter;
};

#endif // ENTRYFILTERDIALOG_H
