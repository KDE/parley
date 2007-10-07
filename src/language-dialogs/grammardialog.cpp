/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "grammardialog.h"

#include "docprop-dialogs/UsageOptPage.h"
#include "docprop-dialogs/TypeOptPage.h"
#include "docprop-dialogs/TenseOptPage.h"
#include "docprop-dialogs/LangPropPage.h"
#include "languagesettings.h"
#include <keduvocdocument.h>

#include <KLocale>
#include <KPageWidgetModel>
#include <KIcon>
#include <KConfig>
#include <KGlobal>

#include <QList>
#include <QPixmap>


GrammarDialog::GrammarDialog(KEduVocDocument *doc, QWidget *parent) : KPageDialog(parent)
{
    setCaption(i18n("Grammar Properties"));
    setButtons(Ok|Cancel);
    setDefaultButton(Ok);
    setModal(true);

    setFaceType(KPageDialog::List);

    typeOptPage = new WordTypeOptionPage(doc, 0);
    addPage(typeOptPage, i18nc("word types","Types"));

    tenseOptPage = new TenseOptPage(doc, 0);
    addPage(tenseOptPage, i18n("Tenses"));

    useOptPage = new UsageOptPage(doc, 0);
    addPage(useOptPage, i18nc("usage (area) of an expression", "Usage"));

    // add the language specific pages
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

    KConfigGroup cg(KGlobal::config(), "GrammarDialog");
    restoreDialogSize(cg);
}

GrammarDialog::~GrammarDialog()
{
    KConfigGroup cg(KGlobal::config(), "GrammarDialog");
    KDialog::saveDialogSize(cg);
}


void GrammarDialog::accept()
{
    useOptPage->accept();
    typeOptPage->accept();
    tenseOptPage->accept();

    foreach ( LangPropPage* langPage, langPages ) {
        langPage->accept();
    }

    QDialog::accept();
}

#include "grammardialog.moc"
