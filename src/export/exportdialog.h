/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "ui_exportoptions.h"

#include <KDialog>

class ParleyDocument;

class ExportDialog : public KDialog
{
Q_OBJECT
public:
    ExportDialog(const KUrl& file, QWidget *parent = 0);

    KUrl fileName();

private:
    Ui::ExportOptions* ui;
    ParleyDocument* m_doc;
};

#endif
