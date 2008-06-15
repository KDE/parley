/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "StatisticsDialog.h"

#include <KLocale>
#include <KConfig>
#include <KGlobal>

#include "lessonstatistics.h"
#include "keduvocdocument.h"
#include "statisticsmodel.h"

StatisticsDialog::StatisticsDialog(KEduVocDocument *doc, QWidget *parent) : KDialog(parent), m_doc(doc)
{
    setCaption(i18n("Document Statistics"));
    setButtons(Ok);
    setModal(true);

    StatisticsModel *statisticsModel = new StatisticsModel(this);
    statisticsModel->setDocument(doc);

    LessonStatisticsView *lessonStatistics = new LessonStatisticsView(this);
    setMainWidget(lessonStatistics);

    lessonStatistics->setModel(statisticsModel);
    lessonStatistics->expandToDepth(0);

    KConfigGroup cg(KGlobal::config(), "StatisticsDialog");
    restoreDialogSize(cg);
}

StatisticsDialog::~StatisticsDialog()
{
    KConfigGroup cg(KGlobal::config(), "StatisticsDialog");
    KDialog::saveDialogSize(cg);
}

#include "StatisticsDialog.moc"
