/***************************************************************************

                       options dialog class

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <klocale.h>
#include <kstandarddirs.h>
#include <kapplication.h>

#include "DocPropDlg.h"
#include <kvtlanguages.h>

class kvoctraindoc;

DocPropsDlg::DocPropsDlg
(
  KEduVocDocument *doc,
  const char      * /*start_page*/,
  QComboBox       *lessons,
  QString          title,
  QString          author,
  QString          license,
  QString          doc_remark,
  QStringList      types,
  QStringList      tenses,
  QStringList      usages,
  QWidget         *parent,
  const char      *name,
  bool             modal
)
  :
  KPageDialog(parent)
{
  setCaption(i18n("Document Properties"));
  setButtons(Ok|Cancel);
  setDefaultButton(Ok);
  setModal(modal);
  setFaceType(KPageDialog::Tabbed);

  QFrame *page = new QFrame();
  addPage( page,i18n("&General") );
  QVBoxLayout *topLayout = new QVBoxLayout( page );
  topLayout->setMargin( KDialog::marginHint() );
  topLayout->setSpacing( KDialog::spacingHint() );
  titleOptPage = new TitlePage (title, author, license, doc_remark, page);
  topLayout->addWidget( titleOptPage );

  page = new QFrame();
  addPage(page, i18n("L&essons"));
  topLayout = new QVBoxLayout( page );
  topLayout->setMargin( KDialog::marginHint() );
  topLayout->setSpacing( KDialog::spacingHint() );
  lessOptPage = new LessOptPage (lessons, doc, page);
  topLayout->addWidget( lessOptPage );

  page = new QFrame();
  addPage(page, i18nc("word types","T&ypes"));
  topLayout = new QVBoxLayout( page );
  topLayout->setMargin( KDialog::marginHint() );
  topLayout->setSpacing( KDialog::spacingHint() );
  typeOptPage = new TypeOptPage (types, doc, page);
  topLayout->addWidget( typeOptPage );

  page = new QFrame();
  addPage( page,i18n("Te&nses"));
  topLayout = new QVBoxLayout( page );
  topLayout->setMargin( KDialog::marginHint() );
  topLayout->setSpacing( KDialog::spacingHint() );
  tenseOptPage = new TenseOptPage (tenses, doc, page);
  topLayout->addWidget( tenseOptPage );

  page = new QFrame();
  addPage( page,i18nc("usage (area) of an expression", "&Usage"));
  topLayout = new QVBoxLayout( page );
  topLayout->setMargin( KDialog::marginHint() );
  topLayout->setSpacing( KDialog::spacingHint() );
  useOptPage = new UsageOptPage (usages, doc, page);
  topLayout->addWidget( useOptPage );

  page = new QFrame();
  addPage( page, i18n("&Options"));
  topLayout = new QVBoxLayout( page );
  topLayout->setMargin( KDialog::marginHint() );
  topLayout->setSpacing( KDialog::spacingHint() );
  docOptPage = new DocOptionsPage (doc->isSortingEnabled(), page);
  topLayout->addWidget( docOptPage );
}

#include "DocPropDlg.moc"
