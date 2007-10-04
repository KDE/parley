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

#include "showsolutiondialog.h"
#include "prefs.h"

#include "testentry.h"
#include <keduvocexpression.h>
#include <QTimer>

ShowSolutionDialog::ShowSolutionDialog(TestEntry* entry, QWidget* parent)
    :KDialog(parent)
{
    setupUi(mainWidget());
    setButtons(KDialog::Ok);

    if ( Prefs::showSolutionTime() > 0 ) {
        QTimer::singleShot(Prefs::showSolutionTime() * 1000, this, SLOT(accept()));
    }

    QFont font = questionLabel->font();
    font.setPointSize(font.pointSize()*2);
    questionLabel->setFont(font);
    dividerLabel->setFont(font);
    solutionLabel->setFont(font);

    questionLabel->setText(entry->exp->translation(Prefs::fromIdentifier()).text());
    solutionLabel->setText(entry->exp->translation(Prefs::toIdentifier()).text());

    playQuestionSoundButton->setIcon(KIcon("media-playback-start"));
    playSolutionSoundButton->setIcon(KIcon("media-playback-start"));

    KConfigGroup cg(KGlobal::config(), "ShowSolutionDialog");
    restoreDialogSize(cg);
}


ShowSolutionDialog::~ShowSolutionDialog()
{
    KConfigGroup cg(KGlobal::config(), "ShowSolutionDialog");
    KDialog::saveDialogSize(cg);
}


#include "showsolutiondialog.moc"

