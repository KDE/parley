/***************************************************************************

                   document language dialog class

    -----------------------------------------------------------------------

    begin         : Sat Jun 2 20:50:53 MET 1999

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

#include "DocPropLangDlg.h"

#include <QStringList>
#include <QLayout>
#include <QList>
#include <QPixmap>
#include <QVBoxLayout>
#include <QFrame>

#include <klocale.h>
#include <kpagewidgetmodel.h>
#include <kicon.h>

#include "LangPropPage.h"
#include <keduvocdocument.h>
#include <kvtlanguages.h>

DocPropsLangDlg::DocPropsLangDlg(KEduVocDocument *doc, const KVTLanguageList &langset, QWidget *parent) : KPageDialog(parent)
{
    setCaption(i18n("Language Properties"));
    setButtons(Ok|Cancel);
    setDefaultButton(Ok);
    setModal(true);
    setFaceType(KPageDialog::Tabbed);
    QFrame *page;
    QVBoxLayout *topLayout;
    LangPropPage *lpp;

    for (int i = 0; i < (int) doc->identifierCount(); i++) {
        QString s;
        if (i == 0)
            s = doc->originalIdentifier();
        else
            s = doc->identifier(i);

        int idx = langset.indexShortId(s);

        QString tabCaption;
        if (idx >= 0)
            tabCaption = (langset[idx].longId());
        else
            tabCaption = (s);

        page = new QFrame();
        KPageWidgetItem *pageItem = new KPageWidgetItem(page, tabCaption);
        pageItem->setHeader(tabCaption);
        pageItem->setIcon(KIcon(QPixmap(langset[idx].pixmapFile())));
        addPage(pageItem);
        topLayout = new QVBoxLayout(page);
        topLayout->setMargin(0);
        topLayout->setSpacing(KDialog::spacingHint());
        lpp = new LangPropPage(doc, s, doc->conjugation(i), doc->article(i), page);
        topLayout->addWidget(lpp);

        langPages.append(lpp);
    }
}


KEduVocConjugation DocPropsLangDlg::getConjugation(int idx) const
{
    if (idx < (int) langPages.count())
        return langPages[idx]->getConjugation();
    else
        return KEduVocConjugation();
}


KEduVocArticle DocPropsLangDlg::getArticle(int idx) const
{
    if (idx < (int) langPages.count())
        return langPages[idx]->getArticle();
    else
        return KEduVocArticle();
}

#include "DocPropLangDlg.moc"
