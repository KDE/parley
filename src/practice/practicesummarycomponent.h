/***************************************************************************
    Copyright 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
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

#include "practiceold/testentrymanager.h"

#include "ui_practicesummarywidget.h"
#include <KXmlGuiWindow>

namespace Practice {

class PracticeSummaryComponent : public KXmlGuiWindow, Ui::PracticeSummaryWidget
{
    Q_OBJECT

public:
    PracticeSummaryComponent(const TestEntryManager& testEntryManager, QWidget *parent);
    ~PracticeSummaryComponent();

private:
    TestEntryManager m_testEntryManager;
};
}

#endif
