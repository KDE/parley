/***************************************************************************

                     query dialog for articles

    -----------------------------------------------------------------------

    begin          : Fri Dec 3 18:28:18 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef ArtQueryDlg_included
#define ArtQueryDlg_included

#include <QKeyEvent>

#include "ui_ArtQueryDlgForm.h"
#include "QueryDlgBase.h"

class ArtQueryDlg : public QueryDlgBase
{
    Q_OBJECT

public:
    ArtQueryDlg(
        const QString &type,
        int entry,
        int col,
        int query_cycle,
        int query_num,
        int query_startnum,
        KEduVocExpression *exp,
        KEduVocDocument  *doc,
        const KEduVocArticle &articles,
        QWidget *parent);

    ~ArtQueryDlg();

    void setQuery(const QString &type,
                  int entry,
                  int col,
                  int query_cycle,
                  int query_num,
                  int query_startnum,
                  KEduVocExpression *exp,
                  KEduVocDocument  *doc,
                  const KEduVocArticle &articles);

public slots:
    virtual void initFocus() const;

protected:
    void keyPressEvent(QKeyEvent *e);

protected slots:
    void showMoreClicked();
    void showAllClicked();
    void slotFemClicked();
    void slotUser2();
    void knowItClicked();
    void slotNaturalClicked();
    void slotMaleClicked();
    void dontKnowClicked();
    void verifyClicked();
    void returnPressed();
    void timeoutReached();

protected:
    KEduVocArticle   articles;

private:
    Ui::ArtQueryDlgForm * mw;
};

#endif // ArtQueryDlg_included
