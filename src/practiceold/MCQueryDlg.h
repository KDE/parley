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

class KEduVocExpression;

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

    QStringList createAdditionalChoices(int numberChoices);

    QLayout* createChoiceWidget(const QString& name, KEduVocExpression* expression);

    /// the correct solution
    int m_solution;

    /** pointers to the entries belonging to the choices or 0 if only string
        used for sound
       (false friend and user supplied default choices do not have a KEduVocExpression for themselves) */
//     QList< KEduVocExpression* > m_expressions;

    QList<QRadioButton*> m_choiceRadioButtons;

    Ui::MCQueryDlgForm * mw;
};

#endif // MCQuery_Dlg_H
