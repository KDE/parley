/***************************************************************************
    Copyright 2007-2008 Frederik Gladhorn <gladhorn@kde.org>
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

#include "ui_practicesummary.h"
#include <KDialog>

class Statistics;
class PracticeEntryManager;

class PracticeSummary : public KDialog, Ui::PracticeSummary
{
    Q_OBJECT

public:
    PracticeSummary(Statistics* statistics, PracticeEntryManager* entryManager, QWidget *parent = 0);
    ~PracticeSummary();

private:
//     Statistics* m_statistics;
};

#endif
