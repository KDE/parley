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

#ifndef DOCPROPLANGDLG_H
#define DOCPROPLANGDLG_H

#include <QList>

#include <kpagedialog.h>

#include <keduvocgrammar.h>
#include <keduvocconjugation.h>

class KEduVocDocument;
class LangPropPage;

class DocPropsLangDlg : public KPageDialog
{
    Q_OBJECT
public:
    DocPropsLangDlg(KEduVocDocument *doc, QWidget *parent);

    ~DocPropsLangDlg();

    KEduVocConjugation getConjugation(int idx) const;
    KEduVocArticle     getArticle(int idx) const;

private:
    typedef QList<LangPropPage *> LangPagesList;
    LangPagesList langPages;
};

#endif // DocPropsLangDlg_included
