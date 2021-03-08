/*
    SPDX-FileCopyrightText: 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICESUMMARYCOMPONENT_H
#define PRACTICESUMMARYCOMPONENT_H

#include "practice/sessionmanagerbase.h"

#include "ui_practicesummarywidget.h"
#include <KXmlGuiWindow>

namespace Practice
{

class PracticeSummaryComponent : public KXmlGuiWindow, Ui::PracticeSummaryWidget
{
    Q_OBJECT

public:
    PracticeSummaryComponent(SessionManagerBase* sessionManager, QWidget *parent);
    ~PracticeSummaryComponent();

public slots:
    void exportResults();

private:
    void initActions(QWidget* parleyMainWindow);
    void setupDetailsTable();

    SessionManagerBase* m_sessionManager;

    class SortedAttemptTableWidgetItem;
};
}

#endif
