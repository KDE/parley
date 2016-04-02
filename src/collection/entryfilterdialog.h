/***************************************************************************
 * Copyright 2008  Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * Copyright 2016  Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ENTRYFILTERDIALOG_H
#define ENTRYFILTERDIALOG_H

#include <QScopedPointer>
#include <QDialog>

class EntryFilter;
namespace Ui
{
class EntryFilterDialog;
}

class EntryFilterDialog : public QDialog
{
    Q_OBJECT
public:
    EntryFilterDialog(EntryFilter &entryFilter);
    ~EntryFilterDialog();

    bool ignoreBlocked() const;

private:
    void setupEntryCounts();
    void setupCheckBoxes();
    void setupConnections();

private slots:
    void updateTotal();

private:
    QScopedPointer<Ui::EntryFilterDialog> m_ui;
    EntryFilter &m_entryFilter;
};

#endif // ENTRYFILTERDIALOG_H
