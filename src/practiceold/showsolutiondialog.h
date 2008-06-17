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

#ifndef SHOWSOLUTIONDIALOG_H
#define SHOWSOLUTIONDIALOG_H

#include "ui_showsolutionwidget.h"
#include <KDialog>

class TestEntry;

namespace Phonon
{
    class MediaObject;
}

class ShowSolutionDialog : public KDialog, Ui::ShowAnswerWidget
{
    Q_OBJECT

public:
    ShowSolutionDialog(TestEntry* entry, QWidget *parent);
    ~ShowSolutionDialog();

private slots:
    void audioPlayFromIdentifier();
    void audioPlayToIdentifier();
    void audioPlayFile(const QString & soundFile);

private:
    TestEntry* m_entry;
    Phonon::MediaObject* m_player;
};

#endif
