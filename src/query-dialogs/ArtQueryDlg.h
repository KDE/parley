/***************************************************************************

                     query dialog for articles

    -----------------------------------------------------------------------

    begin          : Fri Dec 3 18:28:18 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "ui_ArtQueryDlgForm.h"
#include "QueryDlgBase.h"

class ArtQueryDlg : public QueryDlgBase
{
    Q_OBJECT

public:
    ArtQueryDlg(KEduVocDocument *doc, QWidget *parent);

    ~ArtQueryDlg();

    void setQuery(int entry,
                  int col,
                  int query_cycle,
                  int query_num,
                  int query_startnum,
                  KEduVocExpression *exp,
                  const KEduVocArticle &articles);

public slots:
    virtual void initFocus() const;

protected slots:
    void showMoreClicked();
    void showSolution();
    void slotUser1();
    void knowItClicked();
    void dontKnowClicked();
    void verifyClicked();

protected:
    void setStatusText(const QString &statusText) { mw->status->setText(statusText); }
    QProgressBar* timebar() { return mw->timebar; }

    KEduVocArticle   articles;

private:
    Ui::ArtQueryDlgForm * mw;
};

#endif // ArtQueryDlg_included
