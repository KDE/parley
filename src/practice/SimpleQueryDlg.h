/***************************************************************************

                    query dialog for properties

    -----------------------------------------------------------------------

    begin          : Sun Apr 9 18:55:34 2000

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef SimpleQueryDlg_included
#define SimpleQueryDlg_included

#include "ui_SimpleQueryDlgForm.h"
#include "practicedialog.h"

class SimpleQueryDlg : public PracticeDialog
{
    Q_OBJECT

public:
    SimpleQueryDlg(KEduVocDocument *doc, QWidget *parent);

    ~SimpleQueryDlg();

    void setEntry(TestEntry* entry);
    void setProgressCounter(int current, int total);

    void initFocus();

public slots:
    void showMoreClicked();
    void showSolution();
    void knowItClicked();
    void dontKnowClicked();
    void slotAnswerChanged();
    void verifyClicked();

protected:
    void setStatusText(const QString &statusText) { mw->status->setText(statusText); }
    QProgressBar* timebar() { return mw->timebar; }

    QString answerstring;

private:
    Ui::SimpleQueryDlgForm * mw;
    void setQueryFieldWordwrap();
};

#endif // SimpleQueryDlg_included
