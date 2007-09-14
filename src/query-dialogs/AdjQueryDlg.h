/***************************************************************************

                   query dialog for adjectives

    -----------------------------------------------------------------------

    begin          : Sat Dec 4 15:09:18 1999

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

#ifndef AdjQueryDlg_included
#define AdjQueryDlg_included

#include "ui_AdjQueryDlgForm.h"
#include "QueryDlgBase.h"

class AdjQueryDlg : public QueryDlgBase
{
    Q_OBJECT

public:
    AdjQueryDlg(KEduVocDocument *doc, QWidget *parent);

    ~AdjQueryDlg();

    void setQuery(TestEntry* entry);

public slots:
    void initFocus() const;

private:
    void resetAllFields();

    void setStatusText(const QString &statusText) { mw->status->setText(statusText); }
    QProgressBar* timebar() { return mw->timebar; }

private slots:
    void lev1Changed(const QString&);
    void lev2Changed(const QString&);
    void lev3Changed(const QString&);
    void showSolution();
    void slotUser1();
    void knowItClicked();
    void dontKnowClicked();
    void verifyClicked();

private:
    Ui::AdjQueryDlgForm * mw;

    KEduVocComparison comp;
};
#endif // AdjQueryDlg_included
