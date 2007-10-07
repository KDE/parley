/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICESUMMARYDIALOG_H
#define PRACTICESUMMARYDIALOG_H

#include "ui_practicesummarywidget.h"
#include <KDialog>

class TestEntryManager;

class PracticeSummaryDialog : public KDialog, Ui::PracticeSummaryWidget
{
    Q_OBJECT

public:
    PracticeSummaryDialog(TestEntryManager* testEntryManager, QWidget *parent);
    ~PracticeSummaryDialog();

private:
    TestEntryManager* m_testEntryManager;
};

#endif
