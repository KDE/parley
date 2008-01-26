/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ENTRYFILTER_H
#define ENTRYFILTER_H

#include "ui_entryfilter.h"

#include <KDialog>

class TestEntryManager;

class EntryFilter : public KDialog
{
    Q_OBJECT
public:
    EntryFilter(QWidget* parent, TestEntryManager* manager);
    ~EntryFilter();

private:
    Ui::EntryFilter ui;
    TestEntryManager* m_manager;
};

#endif

