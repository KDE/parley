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
#include "GenStatPage.h"
#include "lessonstatistics.h"
#include "keduvocdocument.h"

#include "vocabulary/containermodel.h"

#include "modeltest/modeltest.h"

KVTStatisticsDialog::KVTStatisticsDialog(KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent), m_doc(doc)
{
    setCaption(i18n("Document Statistics"));
    setButtons(Ok | Apply | Cancel);
    setDefaultButton(Ok);
    setModal(true);
    setFaceType(KPageDialog::Tabbed);

    StatisticsPage *spage;

    GenStatPage *gspage = new GenStatPage(doc, 0);
    addPage(gspage, i18nc("general statistics page", "General"));

    for (int i = 1; i < (int) doc->identifierCount(); i++) {
        QString s;
        s = doc->identifier(i).name();

        spage = new StatisticsPage(i, doc, 0);
        addPage(spage, s);
        pageList.append(spage);
    }

    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));


    ContainerModel *lessonModel = new ContainerModel(KEduVocLesson::Lesson, this);
    lessonModel->setDocument(doc);

    new ModelTest(lessonModel, this);

    LessonStatisticsView *lessonStatistics = new LessonStatisticsView(0);
    addPage(lessonStatistics, i18n("Lesson Statistics"));

    lessonStatistics->setModel(lessonModel);


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
