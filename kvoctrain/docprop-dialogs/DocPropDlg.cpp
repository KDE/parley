/***************************************************************************

                       options dialog class

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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

#include <qlayout.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kapplication.h>

#include "DocPropDlg.h"
#include <langset.h>


class kvoctraindoc;

DocPropsDlg::DocPropsDlg
(
  kvoctrainDoc    *doc,
  const char      * /*start_page*/,
  QComboBox       *lessons,
  QString          title,
  QString          author,
  QString          license,
  QString          doc_remark,
  vector<QString>  types,
  vector<QString>  tenses,
  vector<QString>  usages,
  QWidget         *parent,
  const char      *name,
  bool             modal
)
  :
  KDialogBase(Tabbed, i18n("Document Properties"), Ok|Cancel, Ok, parent, name, modal)
{
  QFrame *page = addPage( i18n("&General") );
  QVBoxLayout *topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  titleOptPage = new TitlePage (title, author, license, doc_remark, page, name);
  topLayout->addWidget( titleOptPage );

  page = addPage( i18n("L&essons"));
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  lessOptPage = new LessOptPage (lessons, doc, page, name);
  topLayout->addWidget( lessOptPage );

  page = addPage( i18n("word types","T&ypes"));
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  typeOptPage = new TypeOptPage (types, doc, page, name);
  topLayout->addWidget( typeOptPage );

  page = addPage( i18n("Te&nses"));
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  tenseOptPage = new TenseOptPage (tenses, doc, page, name);
  topLayout->addWidget( tenseOptPage );

  page = addPage( i18n("usage (area) of an expression", "&Usage"));
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  useOptPage = new UsageOptPage (usages, doc, page, name);
  topLayout->addWidget( useOptPage );

  page = addPage( i18n("&Options"));
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  docOptPage = new DocOptionsPage (doc->isAllowedSorting(), page, name);
  topLayout->addWidget( docOptPage );
}


#include "DocPropDlg.moc"
