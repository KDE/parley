/***************************************************************************

                      statistics dialog class

    -----------------------------------------------------------------------

    begin         : Sun Sep 19 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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
#include "kvtlanguages.h"
#include "kvttablemodel.h"

KVTStatisticsDialog::KVTStatisticsDialog(KVTTableModel *model, QWidget *parent) : KPageDialog(parent)
{
    setCaption(i18n("Document Statistics"));
    setButtons(Ok | Apply | Cancel);
    setDefaultButton(Ok);
    setModal(true);
    setFaceType(KPageDialog::Tabbed);

    m_model = model;

    StatisticsPage *spage;

    GenStatPage *gspage = new GenStatPage(m_model->document(), 0);
    addPage(gspage, i18n("General"));

    for (int i = 1; i < (int) m_model->document()->identifierCount(); i++) {
        QString s;
        s = m_model->document()->identifier(i).name();

        spage = new StatisticsPage(i, m_model->document(), 0);
        addPage(spage, s);
        pageList.append(spage);
    }

    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));

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
    foreach(StatisticsPage *page, pageList)
        page->resetStatistics();
    m_model->reset();
}


void KVTStatisticsDialog::accept()
{
    slotApply();
    KPageDialog::accept();
}

#include "StatisticsDialog.moc"
