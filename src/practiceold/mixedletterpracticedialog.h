/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MIXEDLETTERPRACTICEDIALOG_H
#define MIXEDLETTERPRACTICEDIALOG_H

#include "ui_mixedletterpracticedialog.h"
#include "practicedialog.h"

class MixedLetterPracticeDialog : public PracticeDialog, Ui::MixedLetterPracticeDialog
{
    Q_OBJECT

public:
    MixedLetterPracticeDialog(KEduVocDocument *doc, QWidget *parent);

    ~MixedLetterPracticeDialog();

    void setEntry(TestEntry* entry);
    void setProgressCounter(int current, int total);

public slots:
    void showSolution();
    void slotAnswerChanged();
    void verifyClicked();

private:
    void setStatusText(const QString &statusText) {}

    QProgressBar* timebar();

    QList <QGraphicsTextItem*> m_answerTextItems;
};

#endif
