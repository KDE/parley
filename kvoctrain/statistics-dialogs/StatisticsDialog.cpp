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

#include <QLayout>
#include <QVBoxLayout>

#include <klocale.h>
#include <kconfig.h>
#include <kglobal.h>

#include "StatisticsDialog.h"
#include "StatisticsPage.h"
#include "GenStatPage.h"
#include <kvtlanguages.h>
#include <kvttablemodel.h>

KVTStatisticsDialog::KVTStatisticsDialog(KVTLanguages &languages, KVTTableModel *model, QWidget *parent) : KPageDialog(parent)
{
  setCaption(i18n("Document Statistics"));
  setButtons(Ok | Apply | Cancel);
  setDefaultButton(Ok);
  setModal(true);
  setFaceType(KPageDialog::Tabbed);

  m_model = model;

  QFrame * page;
  QVBoxLayout * topLayout;
  StatisticsPage *spage;

  page = new QFrame();
  addPage(page,i18n("General"));
  topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  GenStatPage *gspage = new GenStatPage(m_model->document(), page);
  topLayout->addWidget(gspage);

  for (int i = 1; i < (int) m_model->document()->numIdentifiers(); i++)
  {
    QString s = languages.findLongId(m_model->document()->identifier(i));
    if (s.isEmpty())
      s = m_model->document()->identifier(i);

    page = new QFrame();
    addPage(page,s);
    topLayout = new QVBoxLayout(page);
    topLayout->setMargin(0);
    topLayout->setSpacing(KDialog::spacingHint());
    spage = new StatisticsPage(i, m_model->document(), page);
    topLayout->addWidget(spage);
    pageList.append(spage);
  }

  connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));

  KSharedConfig::Ptr cfg = KGlobal::config();
  cfg->setGroup("StatisticsDialog");
  restoreDialogSize(cfg.data());
}

KVTStatisticsDialog::~KVTStatisticsDialog()
{
  KSharedConfig::Ptr cfg = KGlobal::config();
  cfg->setGroup("StatisticsDialog");
  KDialog::saveDialogSize(cfg.data());
}

void KVTStatisticsDialog::slotApply()
{
  foreach(StatisticsPage * page, pageList)
    page->resetStatistics();
  m_model->reset();
}


void KVTStatisticsDialog::accept()
{
  slotApply();
  KPageDialog::accept();
}

#include "StatisticsDialog.moc"
