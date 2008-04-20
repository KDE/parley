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

#include "exportdialog.h"

ExportDialog::ExportDialog(const KUrl& file, QWidget * parent)
{
    ui = new Ui::ExportOptions();
    ui->setupUi(mainWidget());

    ui->fileName->setUrl(file);
}

KUrl ExportDialog::fileName()
{
    return ui->fileName->url();
}


#include "exportdialog.moc"
