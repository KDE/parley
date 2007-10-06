/***************************************************************************

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 11:45:53 MET 1999

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

#ifndef MCQuery_Dlg_H
#define MCQuery_Dlg_H

#include "ui_MCQueryDlgForm.h"
#include "practicedialog.h"

class MCQueryDlg : public PracticeDialog
{
    Q_OBJECT

public:
    MCQueryDlg(KEduVocDocument *doc, QWidget *parent);

    ~MCQueryDlg();

    void setEntry(TestEntry* entry);
    void setProgressCounter(int current, int total);

public slots:
    void showSolution();
    void verifyClicked();

private:
    void showContinueButton(bool show);
    void setStatusText(const QString &statusText) { mw->status->setText(statusText); }
    QProgressBar* timebar() { return mw->timebar; }

    QList<QPair<QRadioButton *, QLabel *> > button_ref;
    Ui::MCQueryDlgForm * mw;
};

#endif // MCQuery_Dlg_H
