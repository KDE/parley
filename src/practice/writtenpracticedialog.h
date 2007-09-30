/***************************************************************************

                   dialog when in random query mode

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#ifndef WRITTENPRACTICEDIALOG_H
#define WRITTENPRACTICEDIALOG_H

#include <QKeyEvent>
#include <QList>

#include "ui_writtenpracticedialog.h"
#include "practicedialog.h"

class KEduVocDocument;

class KComboBox;
class KLineEdit;

class WrittenPracticeDialog : public PracticeDialog
{
    Q_OBJECT

public:
    WrittenPracticeDialog(KEduVocDocument *doc, QWidget *parent);

    ~WrittenPracticeDialog();

    void setEntry( TestEntry* entry );
    void setProgressCounter(int current, int total);

public slots:
    void verifyClicked();
    void showMoreClicked();
    void showSolution();
    void knowItClicked();
    void dontKnowClicked();
    void slotTransChanged(const QString&);
    void slotTransLostFocus();
    void slotFFClicked(bool show);
    void slotTypeClicked(bool show);
    void slotRemClicked(bool show);

private:
    void setStatusText(const QString &statusText) { mw->status->setText(statusText); }
    QProgressBar* timebar() { return mw->timebar; }

    void keyPressEvent(QKeyEvent *e);

    void setHintFields();
    QStringList extractTranslations(const QString &);

    QList<KComboBox*> transCombos;
    QList<KLineEdit*> transFields;

    QStringList translations;
    QStringList vocabulary;

    ///
    bool suggestion_hint;
    ///
    int  fields;

    Ui::WrittenPracticeDialog * mw;
};

#endif
