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

#include "ui_exportdialog.h"

#include <KDialog>

class ParleyDocument;
class KUrl;

class ExportDialog : public KDialog
{
    Q_OBJECT
public:
    ExportDialog(ParleyDocument *doc, QWidget *parent = 0);

private slots:
    void accept();

private:
    KUrl getFileName(const QString& filter);

    Ui::ExportOptions* ui;
    ParleyDocument *m_doc;
    QWidget *m_parent;
};

#endif
