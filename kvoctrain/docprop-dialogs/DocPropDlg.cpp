/***************************************************************************

                       options dialog class

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

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
#include <QFrame>

#include <KLocale>

#include "DocPropDlg.h"

DocPropsDlg::DocPropsDlg(KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent)
{
  setCaption(i18n("Document Properties"));
  setButtons(Ok|Cancel);
  setDefaultButton(Ok);
  setModal(true);
  setFaceType(KPageDialog::Tabbed);

  QFrame *page = new QFrame();
  addPage(page, i18n("&General"));
  QVBoxLayout *topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  titleOptPage = new TitlePage(doc, page);
  topLayout->addWidget(titleOptPage);

  page = new QFrame();
  addPage(page, i18n("L&essons"));
  topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  lessOptPage = new LessOptPage(doc, page);
  topLayout->addWidget(lessOptPage);

  page = new QFrame();
  addPage(page, i18nc("word types","T&ypes"));
  topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  typeOptPage = new TypeOptPage(doc, page);
  topLayout->addWidget(typeOptPage);

  page = new QFrame();
  addPage(page, i18n("Te&nses"));
  topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  tenseOptPage = new TenseOptPage(doc, page);
  topLayout->addWidget(tenseOptPage);

  page = new QFrame();
  addPage(page, i18nc("usage (area) of an expression", "&Usage"));
  topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  useOptPage = new UsageOptPage(doc, page);
  topLayout->addWidget(useOptPage);

  page = new QFrame();
  addPage(page, i18n("&Options"));
  topLayout = new QVBoxLayout(page);
  topLayout->setMargin(0);
  topLayout->setSpacing(KDialog::spacingHint());
  docOptPage = new DocOptionsPage(doc->isSortingEnabled(), page);
  topLayout->addWidget(docOptPage);
}

#include "DocPropDlg.moc"
