/***************************************************************************

                   document properties dialog class

    -----------------------------------------------------------------------

    begin         : Fri Sep 10 20:50:53 MET 1999

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

#ifndef DOCPROPDLG_H
#define DOCPROPDLG_H


#include "TypeOptPage.h"
#include "TenseOptPage.h"
#include "UsageOptPage.h"
#include "TitlePage.h"

#include <KPageDialog>

/**
  * This is the KPageDialog for all the document option pages. You get it by clicking Vocabulary->Properties.
  * It contains the pages for General, Lesson, Types, Tenses, Usage and Options.
  */
class DocPropsDlg : public KPageDialog
{
    Q_OBJECT
public:
    DocPropsDlg(KEduVocDocument *doc, QWidget *parent);

    ~DocPropsDlg();

    inline QString getTitle()
    {
        return titleOptPage->getTitle();
    }
    inline QString getAuthor()
    {
        return titleOptPage->getAuthor();
    }
    inline QString getLicense()
    {
        return titleOptPage->getLicense();
    }
    inline QString getDocRemark()
    {
        return titleOptPage->getDocRemark();
    }

    inline void getTenseNames(QStringList &tenses, QList<int>& ret_index) const
    {
        tenseOptPage->getTenseNames(tenses, ret_index);
    }

    void commitData();

private:
    UsageOptPage   *useOptPage;
    WordTypeOptionPage    *typeOptPage;
    TenseOptPage   *tenseOptPage;
    TitlePage      *titleOptPage;
};

#endif // DocPropsDlg_included

