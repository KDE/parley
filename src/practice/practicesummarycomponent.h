/*
    SPDX-FileCopyrightText: 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICESUMMARYCOMPONENT_H
#define PRACTICESUMMARYCOMPONENT_H

#include "practice/sessionmanagerbase.h"

#include "ui_practicesummarywidget.h"
#include <KXMLGUIClient>

namespace Practice
{
class PracticeSummaryComponent : public QWidget, public KXMLGUIClient, Ui::PracticeSummaryWidget
{
    Q_OBJECT

public:
    PracticeSummaryComponent(SessionManagerBase *sessionManager, QWidget *parent);

public Q_SLOTS:
    void exportResults();

private:
    void initActions(QWidget *parleyMainWindow);
    void setupDetailsTable();

    SessionManagerBase *m_sessionManager{nullptr};

    class SortedAttemptTableWidgetItem;
};
}

#endif
