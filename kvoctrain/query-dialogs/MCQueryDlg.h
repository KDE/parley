/***************************************************************************

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 11:45:53 MET 1999

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

#ifndef MCQuery_Dlg_H
#define MCQuery_Dlg_H

#include "ui_MCQueryDlgForm.h"
#include "QueryDlgBase.h"

class MCQueryDlg : public QueryDlgBase
{
    Q_OBJECT

public:
    MCQueryDlg(KEduVocDocument *doc, QWidget *parent);

    ~MCQueryDlg();

    void setQuery(const QString &org, int entry, int orgcol, int transcol, int query_cycle, int query_num, int query_startnum, KEduVocDocument *doc);

public slots:
    virtual void initFocus() const;
    void showSolution();
    void knowItClicked();
    void dontKnowClicked();
    void slotUser1();
    void verifyClicked();

protected:
    void setStatusText(const QString &statusText) { mw->status->setText(statusText); }
    QProgressBar* timebar() { return mw->timebar; }

private:
    QList<QPair<QRadioButton *, QLabel *> > button_ref;
    Ui::MCQueryDlgForm * mw;
};

#endif // MCQuery_Dlg_H
