/***************************************************************************

                      statistics dialog class

    -----------------------------------------------------------------------

    begin         : Sun Sep 19 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

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

#include "StatisticsPage.h"
#include "generalstatisticspage.h"
#include "lessonstatistics.h"
#include "keduvocdocument.h"

#include "statisticsmodel.h"

#include "modeltest/modeltest.h"

KVTStatisticsDialog::KVTStatisticsDialog(KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent), m_doc(doc)
{
    setCaption(i18n("Document Statistics"));
    setButtons(Ok | Apply | Cancel);
    setDefaultButton(Ok);
    setModal(true);
    setFaceType(KPageDialog::Tabbed);

    StatisticsPage *spage;

    GeneralStatisticsPage *gspage = new GeneralStatisticsPage(doc, this);
    addPage(gspage, i18nc("general statistics page", "General"));

/*
    for (int i = 1; i < (int) doc->identifierCount(); i++) {
        QString s;
        s = doc->identifier(i).name();

        spage = new StatisticsPage(i, doc, this);
        addPage(spage, s);
        pageList.append(spage);
    }
*/
    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));

    StatisticsModel *statisticsModel = new StatisticsModel(this);
    statisticsModel->setDocument(doc);

    LessonStatisticsView *lessonStatistics = new LessonStatisticsView(this);
    addPage(lessonStatistics, i18n("Lesson Statistics"));

    lessonStatistics->setModel(statisticsModel);


    KConfigGroup cg(KGlobal::config(), "StatisticsDialog");
    restoreDialogSize(cg);
}

KVTStatisticsDialog::~KVTStatisticsDialog()
{
    KConfigGroup cg(KGlobal::config(), "StatisticsDialog");
    KDialog::saveDialogSize(cg);
}

void KVTStatisticsDialog::slotApply()
{
    foreach(StatisticsPage *page, pageList) {
        page->resetStatistics();
    }
    //m_doc->lesson()->resetGrades();
}


void KVTStatisticsDialog::accept()
{
    slotApply();
    KPageDialog::accept();
}

#include "StatisticsDialog.moc"
