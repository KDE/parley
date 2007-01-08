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

#include "StatisticsDialog.h"
#include "StatisticsPage.h"
#include "GenStatPage.h"
#include <kvtlanguages.h>
#include <keduvocdocument.h>

KVTStatisticsDialog::KVTStatisticsDialog(KVTLanguages &languages, KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent)
{
  setCaption(i18n("Document Statistics"));
  setButtons(Close);
  setDefaultButton(Close);
  setModal(true);
  setFaceType(KPageDialog::Tabbed);
  QFrame * page;
  QVBoxLayout * topLayout;
  StatisticsPage *spage;

  page = new QFrame();
  addPage(page,i18n("General"));
  topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  GenStatPage *gspage = new GenStatPage(doc, page);
  topLayout->addWidget(gspage);

  for (int i = 1; i < (int) doc->numIdentifiers(); i++)
  {
    QString s = languages.findLongId(doc->identifier(i));
    if (s.isEmpty() )
      s = doc->identifier(i);

    page = new QFrame();
    addPage(page,s);
    topLayout = new QVBoxLayout(page);
    topLayout->setMargin(0);
    topLayout->setSpacing(KDialog::spacingHint());
    spage = new StatisticsPage(i, doc, page);
    topLayout->addWidget(spage);
  }
}

#include "StatisticsDialog.moc"
