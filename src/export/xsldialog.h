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

#ifndef XSLDIALOG_H
#define XSLDIALOG_H

#include "ui_xsldialog.h"

#include <KDialog>

class KEduVocDocument;

class XslDialog : public KDialog
{
Q_OBJECT
public:
    XslDialog(const KUrl &file, KEduVocDocument *doc, QWidget *parent = 0);

private slots:
    void accept();

private:
    Ui::ExportOptions* ui;
    KUrl m_targetFile;
    KEduVocDocument *m_doc;
};

#endif
