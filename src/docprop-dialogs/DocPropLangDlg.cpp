/***************************************************************************

                   document language dialog class

    -----------------------------------------------------------------------

    begin         : Sat Jun 2 20:50:53 MET 1999

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

#include "DocPropLangDlg.h"

#include "LangPropPage.h"
#include "languagesettings.h"
#include <keduvocdocument.h>

#include <KLocale>
#include <KPageWidgetModel>
#include <KIcon>
#include <KConfig>
#include <KGlobal>

#include <QList>
#include <QPixmap>


DocPropsLangDlg::DocPropsLangDlg(KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent)
{
    setCaption(i18n("Language Properties"));
    setButtons(Ok|Cancel);
    setDefaultButton(Ok);
    setModal(true);

    setFaceType(KPageDialog::List);

    LangPropPage *lpp;

    for (int i = 0; i < doc->identifierCount(); i++) {
        QString s;
        s = doc->identifier(i).name();

        lpp = new LangPropPage(doc, i, 0);
        KPageWidgetItem *pageItem = new KPageWidgetItem(lpp, s);
        pageItem->setHeader(s);

        LanguageSettings currentSettings(doc->identifier(i).locale());
        currentSettings.readConfig();
        QString icon = currentSettings.icon();
        if ( !icon.isEmpty() ) {
            pageItem->setIcon(KIcon(icon));
        }
        addPage(pageItem);
        langPages.append(lpp);
    }

    KConfigGroup cg(KGlobal::config(), "LanguagePropertiesDialog");
    restoreDialogSize(cg);
}


DocPropsLangDlg::~DocPropsLangDlg()
{
    KConfigGroup cg(KGlobal::config(), "LanguagePropertiesDialog");
    KDialog::saveDialogSize(cg);
}

void DocPropsLangDlg::accept()
{
    QDialog::accept();
    foreach ( LangPropPage* langPage, langPages ) {
        langPage->accept();
    }
}


#include "DocPropLangDlg.moc"
