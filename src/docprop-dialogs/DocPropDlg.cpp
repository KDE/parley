/***************************************************************************

                       options dialog class

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "DocPropDlg.h"

#include <KLocale>
#include <KConfig>
#include <KGlobal>

DocPropsDlg::DocPropsDlg(KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent)
{
    setCaption(i18n("Document Properties"));
    setButtons(Ok|Cancel);
    setDefaultButton(Ok);
    setModal(true);
//     setFaceType(KPageDialog::Tabbed);
    setFaceType(KPageDialog::List);

    typeOptPage = new WordTypeOptionPage(doc, 0);
    addPage(typeOptPage, i18nc("word types","Types"));

    tenseOptPage = new TenseOptPage(doc, 0);
    addPage(tenseOptPage, i18n("Tenses"));

    useOptPage = new UsageOptPage(doc, 0);
    addPage(useOptPage, i18nc("usage (area) of an expression", "Usage"));

    KConfigGroup cg(KGlobal::config(), "DocumentPropertiesDialog");
    restoreDialogSize(cg);
}

DocPropsDlg::~DocPropsDlg()
{
    KConfigGroup cg(KGlobal::config(), "DocumentPropertiesDialog");
    KDialog::saveDialogSize(cg);
}


void DocPropsDlg::commitData()
{
    useOptPage->commitData();
}


#include "DocPropDlg.moc"


